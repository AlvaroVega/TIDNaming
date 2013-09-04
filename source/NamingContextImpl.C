// -*- c++ -*-
/*
 *  File name: NamingContextImpl.C
 *  File type: Body file.
 */
/*
// (C) Copyright 2009 Telefonica Investigacion y Desarrollo
 *
//     S.A.Unipersonal (Telefonica I+D)
//
// This file is part of Morfeo CORBA Platform.
//
// Morfeo CORBA Platform is free software: you can redistribute it and/or
// modify it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// Morfeo CORBA Platform is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with Morfeo CORBA Platform. If not, see
//
//   http://www.gnu.org/licenses
//
// Info about members and contributors of the MORFEO project
// is available at
//
//   http://morfeo-project.org
 */
#include "NamingContextImpl.h"
#include "NamingContextIOFactory.h"

// POSIX headers needed for persistence
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


TIDorb::core::util::Counter TIDNaming::NamingContextImpl::m_oid_count;


TIDNaming::NamingContextImpl::NamingContextImpl(TIDorb::core::TIDORB* orb, 
                                                PortableServer::POA_ptr namingcontext_poa,
                                                PortableServer::POA_ptr binding_iterators_poa,
                                                int max_binding_iterators,
                                                const char* string_oid)
  : m_orb(orb),
    m_destroying(false),
    m_namingcontext_poa(namingcontext_poa),
    m_binding_iterators_poa(binding_iterators_poa),
    m_binding_iterators(max_binding_iterators),
    m_binding_iterators_count(1),
    m_modified(true)
{
  m_string_oid = strdup(string_oid);

  //
  // Obtains POACurrent
  //
  CORBA::Object_var current_obj = m_orb->resolve_initial_references("POACurrent");
  m_namingcontext_current = PortableServer::Current::_narrow(current_obj);

  if (m_orb->trace != NULL){
    TIDorb::util::StringBuffer msg;
    msg << "NamingContextImpl created OID=" << string_oid;
    m_orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
  }

}


TIDNaming::NamingContextImpl::~NamingContextImpl()
{
  //
  // First call to destroy to all child context
  // 
  NamingContextTable::iterator nc_it  = m_naming_contexts.begin();
  NamingContextTable::iterator nc_end = m_naming_contexts.end();
  while (nc_it != nc_end) {
    const char * ior = nc_it->second;
    
    CORBA::Object_var obj = m_orb->string_to_object(ior);
    CosNaming::NamingContext_var nc_child = 
        CosNaming::NamingContext::_narrow(obj);
    try {
      nc_child->destroy();
    } catch (const CosNaming::NamingContext::NotEmpty& ex) {
      if (m_orb->trace != NULL){
        TIDorb::util::StringBuffer msg;
        msg << "~NamingContextImpl() NotEmpty exception destroying ";
        msg << nc_it->first.id << "." << nc_it->first.kind;
        m_orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
      }
    } catch (...) {
      if (m_orb->trace != NULL){
        TIDorb::util::StringBuffer msg;
        msg << "~NamingContextImpl() unknown exception destroying ";
        msg << nc_it->first.id << "." << nc_it->first.kind;
        m_orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
      }
    }
    nc_it++;
  } 

  if (!m_destroying) {
    try {
      this->destroy();
    } catch (const CosNaming::NamingContext::NotEmpty& ex) {
      if (m_orb->trace != NULL){
        TIDorb::util::StringBuffer msg;
        msg << "~NamingContextImpl() NotEmpty exception destroying ";
        msg << m_string_oid;
        m_orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
      }
    } catch (...) {
      if (m_orb->trace != NULL){
        TIDorb::util::StringBuffer msg;
        msg << "~NamingContextImpl() unknown exception destroying ";
        msg << m_string_oid;
        m_orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
      }
    } 
  }

  if (m_orb->trace != NULL){
    TIDorb::util::StringBuffer msg;
    msg << "~NamingContextImpl() deleting... " << m_string_oid;
    m_orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
  }

  delete m_string_oid;



  //
  // Deletes content of m_naming_contexts and m_object_refs tables
  // 
  nc_it  = m_naming_contexts.begin();
  nc_end = m_naming_contexts.end();
  while (nc_it != nc_end) {
    delete nc_it->second;
    m_naming_contexts.erase(nc_it);
    nc_it++;
  }
  m_naming_contexts.clear();


  ObjectRefTable::iterator ob_it  = m_object_refs.begin();
  ObjectRefTable::iterator ob_end = m_object_refs.end();
  while (ob_it != ob_end) {
    delete ob_it->second;
    m_object_refs.erase(ob_it);
    ob_it++;
  }
  m_object_refs.clear();

}


CORBA::Object_ptr 
TIDNaming::NamingContextImpl::resolve (const ::CosNaming::Name& n)
  throw (CORBA::SystemException, 
         CosNaming::NamingContext::NotFound, 
         CosNaming::NamingContext::CannotProceed, 
         CosNaming::NamingContext::InvalidName)
{
  TIDThr::Synchronized sync(m_recursive_mutex);

  if (m_destroying)
    return CORBA::Object::_nil();

  CORBA::ULong name_length = n.length();

  if (name_length > 0) {

    if (name_length > 1) {
      // In this case (see "2.1.1 Resolution of Compound Names"), resolution must
      // be done over m_naming_contexts table only!
      NamingContextTable::iterator it = m_naming_contexts.find(n[0]);
      
      if (it != m_naming_contexts.end()) {
        
        CORBA::Object_var object_ref = m_orb->string_to_object((*it).second);
        CosNaming::NamingContext_var nc_ref = NULL;

        try {
          nc_ref = CosNaming::NamingContext::_narrow(object_ref);
        } catch (const CORBA::Exception& ex) {
          // Is not a NamingContext
          if (m_orb->trace != NULL){
            TIDorb::util::StringBuffer msg;
            msg << "NamingContextImpl::resolve() found name " << n[0].id << "." << n[0].kind;
            msg << " but is not a NamingContext";
            m_orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
          }
          throw CosNaming::NamingContext::NotFound(CosNaming::NamingContext::not_context, n);
        }
        // Is a NamingContext
        
        // Continues propagating resolve request
        CosNaming::Name n_next;
        n_next.length(name_length - 1);
        for (CORBA::ULong i=0; i < (name_length-1); ++i) {
          n_next[i] = n[i+1];
        }
        return nc_ref->resolve(n_next);
      }
      else // Name not found 
        throw CosNaming::NamingContext::NotFound(CosNaming::NamingContext::missing_node, n);
      
    }
    else { // (name_length = 1)
      
      // The Name must only corresponded to one of m_naming_context or m_object_refs
      NamingContextTable::iterator nc_it = m_naming_contexts.find(n[0]);
      char* string_ref;

      if (nc_it != m_naming_contexts.end())
        string_ref = strdup((*nc_it).second);
      else{ 
        ObjectRefTable::iterator ob_it = m_object_refs.find(n[0]);
        if (ob_it != m_object_refs.end()) 
          string_ref = strdup((*ob_it).second);
        else{
          throw CosNaming::NamingContext::NotFound(CosNaming::NamingContext::missing_node, n);
        }
      }

      CORBA::Object_ptr object_ref = m_orb->string_to_object(string_ref); 
      free(string_ref);
      return object_ref;
    }
    
  }
  else // (name_length =< 0)
    throw CosNaming::NamingContext::InvalidName();
  
}


void 
TIDNaming::NamingContextImpl::bind (const ::CosNaming::Name& n, CORBA::Object_ptr obj)
  throw (CORBA::SystemException, 
         CosNaming::NamingContext::NotFound, 
         CosNaming::NamingContext::CannotProceed, 
         CosNaming::NamingContext::InvalidName, 
         CosNaming::NamingContext::AlreadyBound)
{
  TIDThr::Synchronized sync(m_recursive_mutex);

  if (m_destroying)
    return;

  CORBA::ULong name_length = n.length();

  if (name_length > 0) {
    
    if (name_length > 1) {
      // In this case (see "2.1.1 Resolution of Compound Names"), resolution must
      // be done over m_naming_contexts table only!
      NamingContextTable::iterator it = m_naming_contexts.find(n[0]);
      
      if (it != m_naming_contexts.end()) {
        
        CORBA::Object_var object_ref = m_orb->string_to_object((*it).second);
        CosNaming::NamingContext_var nc_ref = NULL;
        
        try {
          nc_ref = CosNaming::NamingContext::_narrow(object_ref);
        } catch (const CORBA::Exception& ex) {
          // Is not a NamingContext
          if (m_orb->trace != NULL){
            TIDorb::util::StringBuffer msg;
            msg << "NamingContextImpl::bind() found name " << n[0].id << "." << n[0].kind;
            msg << " but is not a NamingContext";
            m_orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
          }
          throw CosNaming::NamingContext::NotFound(CosNaming::NamingContext::not_context, n);
        }
        // Is a correct NamingContext
        
        // Continues propagating resolve request
        CosNaming::Name n_next;
        n_next.length(name_length - 1);
        for (CORBA::ULong i=0; i < (name_length-1); ++i) {
          n_next[i] = n[i+1];
        }

        return nc_ref->bind(n_next, obj);
      }
      else // (it != m_naming_contexts.end())
        throw CosNaming::NamingContext::NotFound(CosNaming::NamingContext::missing_node, n);
      
    }
    else { // (name_length == 1)
      
      // Name couldn't be in m_naming_contexts or m_object_refs tables yet
      NamingContextTable::iterator nc_it = m_naming_contexts.find(n[0]);
      if (nc_it != m_naming_contexts.end()) 
        throw CosNaming::NamingContext::AlreadyBound();
      
      NamingContextTable::iterator ob_it = m_object_refs.find(n[0]);
      if (ob_it == m_object_refs.end()) {
        char* str_obj = m_orb->object_to_string(obj);
        m_object_refs[n[0]] = str_obj;        
        
        
        if (NamingContextIOFactory::getFactory().isPersistentFactory()) {
          // Operate the persistence
          try
          {
            NamingInternals::ReferenceSeq refs;
            
            refs.length(1);
            refs[0] = NamingInternals::Reference(
                  n[0], CosNaming::nobject, str_obj);
            
            NamingContextIOFactory::getFactory().getWriter().store(
                  m_string_oid, refs);
          }
          catch (NamingInternals::PersistenceException & e)
          {
            if (m_orb->trace != NULL){
              TIDorb::util::StringBuffer msg;
              msg << "NamingContextImpl::bind() Error: " << e.msg;
              m_orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
            }
            
            throw CosNaming::NamingContext::CannotProceed(NULL, n);
          }
        }

      } 
      else
        throw CosNaming::NamingContext::AlreadyBound();
    }
  }
  else // (name_length =< 0)
    throw CosNaming::NamingContext::InvalidName();

}


void 
TIDNaming::NamingContextImpl::rebind (const ::CosNaming::Name& n, CORBA::Object_ptr obj)
  throw (CORBA::SystemException, 
         CosNaming::NamingContext::NotFound, 
         CosNaming::NamingContext::CannotProceed, 
         CosNaming::NamingContext::InvalidName)
{
  TIDThr::Synchronized sync(m_recursive_mutex);

  if (m_destroying)
    return;

  CORBA::ULong name_length = n.length();

  if (name_length > 0) {
    
    if (name_length > 1) {
      // In this case (see "2.1.1 Resolution of Compound Names"), resolution must
      // be done over m_naming_contexts table only!
      NamingContextTable::iterator it = m_naming_contexts.find(n[0]);
      
      if (it != m_naming_contexts.end()) {
        
        CORBA::Object_var object_ref = m_orb->string_to_object((*it).second);
        CosNaming::NamingContext_var nc_ref = NULL;
        
        try {
          nc_ref = CosNaming::NamingContext::_narrow(object_ref);
        } catch (const CORBA::Exception& ex) {
          // Is not a NamingContext
          if (m_orb->trace != NULL){
            TIDorb::util::StringBuffer msg;
            msg << "NamingContextImpl::rebind() found name " << n[0].id << "." << n[0].kind;
            msg << " but is not a NamingContext";
            m_orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
          }
          throw CosNaming::NamingContext::NotFound(CosNaming::NamingContext::not_context, n);
        }
        // Is a NamingContext
        
        // Continues propagating resolve request
        CosNaming::Name n_next;
        n_next.length(name_length - 1);
        for (CORBA::ULong i=0; i < (name_length-1); ++i) {
          n_next[i] = n[i+1];
        }

        return nc_ref->rebind(n_next, obj);
      }
      else // (it != m_naming_contexts.end())
        throw CosNaming::NamingContext::NotFound(CosNaming::NamingContext::missing_node, n);
      
    }
    else { // (name_length == 1)
      
      // Name couldn't be in m_naming_contexts: ERROR
      NamingContextTable::iterator nc_it = m_naming_contexts.find(n[0]);
      if (nc_it != m_naming_contexts.end()) 
        throw CosNaming::NamingContext::InvalidName();
      
      NamingContextTable::iterator ob_it = m_object_refs.find(n[0]);
      if (ob_it == m_object_refs.end()) {
        char* str_obj = m_orb->object_to_string(obj);
        m_object_refs[n[0]] = str_obj;

        if (NamingContextIOFactory::getFactory().isPersistentFactory()) {
          // Operate the persistence
          try
          {
            NamingInternals::ReferenceSeq refs;
            
            refs.length(1);
            refs[0] = NamingInternals::Reference(
                  n[0], CosNaming::nobject, str_obj);
            
            NamingContextIOFactory::getFactory().getWriter().store(
                  m_string_oid, refs);
          }
          catch (NamingInternals::PersistenceException & e)
          {
            if (m_orb->trace != NULL){
              TIDorb::util::StringBuffer msg;
              msg << "NamingContextImpl::rebind() Error: " << e.msg;
              m_orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
            }
            throw CosNaming::NamingContext::CannotProceed(NULL, n);
          }
        }

      } 
      else // Name is in m_object_refs tables yet: OK
        return;
    }
  }
  else // (name_length =< 0)
    throw CosNaming::NamingContext::InvalidName();
  
}


void 
TIDNaming::NamingContextImpl::unbind (const ::CosNaming::Name& n)
  throw (CORBA::SystemException, 
         CosNaming::NamingContext::NotFound, 
         CosNaming::NamingContext::CannotProceed, 
         CosNaming::NamingContext::InvalidName)
{
  TIDThr::Synchronized sync(m_recursive_mutex);

  if (m_destroying)
    return;

  CORBA::ULong name_length = n.length();

  if (name_length > 0) {
    
    if (name_length > 1) {
      // In this case (see "2.1.1 Resolution of Compound Names"), resolution must
      // be done over m_naming_contexts table only!
      NamingContextTable::iterator it = m_naming_contexts.find(n[0]);
      
      if (it != m_naming_contexts.end()) {
        
        CORBA::Object_var object_ref = m_orb->string_to_object((*it).second);
        CosNaming::NamingContext_var nc_ref = NULL;
        
        try {
          nc_ref = CosNaming::NamingContext::_narrow(object_ref);
        } catch (const CORBA::Exception& ex) {
          // Is not a NamingContext
          if (m_orb->trace != NULL){
            TIDorb::util::StringBuffer msg;
            msg << "NamingContextImpl::unbind() found name " << n[0].id << "." << n[0].kind;
            msg << " but is not a NamingContext";
            m_orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
          }
          throw CosNaming::NamingContext::NotFound(CosNaming::NamingContext::not_context, n);
        }
        // Is a NamingContext
          
        // Continues propagating resolve request
        CosNaming::Name n_next;
        n_next.length(name_length - 1);
        for (CORBA::ULong i=0; i < (name_length-1); ++i) {
          n_next[i] = n[i+1];
        }
        
        return nc_ref->unbind(n_next);
      }
      else // (it != m_naming_contexts.end())
        throw CosNaming::NamingContext::NotFound(CosNaming::NamingContext::missing_node, n);
      
    }
    else { // (name_length == 1)
      
      NamingContextTable::iterator nc_it = m_naming_contexts.find(n[0]);
      if (nc_it != m_naming_contexts.end()) {
        delete nc_it->second;
        m_naming_contexts.erase(nc_it);
      } else {
        ObjectRefTable::iterator ob_it = m_object_refs.find(n[0]);
        if (ob_it != m_object_refs.end()) {     
          delete ob_it->second;
          m_object_refs.erase(ob_it);
        }else
          throw CosNaming::NamingContext::NotFound(CosNaming::NamingContext::not_object, n);
      }

      if (NamingContextIOFactory::getFactory().isPersistentFactory()) {
        // Operate the persistence
        try
          {
            NamingContextIOFactory::getFactory().getWriter().removeReference(
                m_string_oid, n[0]);
          }
        catch (NamingInternals::PersistenceException & e)
        {
          if (m_orb->trace != NULL){
            TIDorb::util::StringBuffer msg;
            msg << "NamingContextImpl::unbind() Error: " << e.msg;
            m_orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
          }
          throw CosNaming::NamingContext::CannotProceed(NULL, n);
        }  
      }

    }
  }
  else // (name_length =< 0)
    throw CosNaming::NamingContext::InvalidName();
  
}


CosNaming::NamingContext_ptr 
TIDNaming::NamingContextImpl::new_context ()
  throw (CORBA::SystemException)
{
  TIDThr::Synchronized sync(m_recursive_mutex);

  if (m_destroying)
    return CosNaming::NamingContext::_nil();

  char string_oid[40];
  sprintf(string_oid, "nc-%u.ctx", m_oid_count.inc());

  if (m_orb->trace != NULL){
    TIDorb::util::StringBuffer msg;
    msg << "Creating new context with id '" << string_oid << "'... ";
    m_orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
  }

  CORBA::Object_var obj_reference = 0;

  try{ 

    PortableServer::ObjectId_var oid = 
      PortableServer::string_to_ObjectId(string_oid);
    
    obj_reference = 
      m_namingcontext_poa->create_reference_with_id(oid, 
                                     "IDL:omg.org/CosNaming/NamingContextExt:1.0");
    
  } catch(const PortableServer::POA::ServantAlreadyActive& ex) {
    if (m_orb->trace != NULL)
      m_orb->print_trace(TIDorb::util::TR_ERROR, 
                         "Error creating new context: ServantAlreadyActive");
    throw;
  } catch(const PortableServer::POA::ObjectAlreadyActive& ex) {
    if (m_orb->trace != NULL)
      m_orb->print_trace(TIDorb::util::TR_ERROR, 
                         "Error creating new context: ObjectAlreadyActive");
    throw;
  } catch(const PortableServer::POA::WrongPolicy& ex) {
    if (m_orb->trace != NULL)
      m_orb->print_trace(TIDorb::util::TR_ERROR, 
                         "Error creating new context: WrongPolicy");
    throw;
  } catch(...) {
    if (m_orb->trace != NULL)
      m_orb->print_trace(TIDorb::util::TR_ERROR, 
                         "Unexpected error creating new context");
    throw;
  }

  try
  {
    CosNaming::NamingContext_ptr result = 
          CosNaming::NamingContext::_narrow(obj_reference);

    if (CORBA::is_nil(result) || !result) {
      if (m_orb->trace != NULL)
        m_orb->print_trace(TIDorb::util::TR_ERROR, 
                           "Error: cannot narrow just created naming context object");

    } else {

      if (NamingContextIOFactory::getFactory().isPersistentFactory()) {
        // Operate the persistence
        NamingContextIOFactory::getFactory().getWriter().store(result);
      } 
    }

    if (m_orb->trace != NULL)
      m_orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, "Done");

    return result;
  }
  catch (NamingInternals::PersistenceException & e)
  {
    if (m_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "Error: " << e.msg;
      m_orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
    }
    throw;
  }
}


void 
TIDNaming::NamingContextImpl::bind_context (const CosNaming::Name& n,
                                            CosNaming::NamingContext_ptr nc)
  throw (CORBA::SystemException, 
         CosNaming::NamingContext::NotFound, 
         CosNaming::NamingContext::CannotProceed,
         CosNaming::NamingContext::InvalidName, 
         CosNaming::NamingContext::AlreadyBound)
{
  TIDThr::Synchronized sync(m_recursive_mutex);

  if (m_destroying)
    return;

  CORBA::ULong name_length = n.length();

  if (name_length > 0) {

    if (name_length > 1) {
      // In this case (see "2.1.1 Resolution of Compound Names"), resolution must
      // be done over m_naming_contexts table only!
      NamingContextTable::iterator it = m_naming_contexts.find(n[0]);
      
      if (it != m_naming_contexts.end()) {
        
        CORBA::Object_var object_ref = m_orb->string_to_object((*it).second);
        CosNaming::NamingContext_var nc_ref = NULL;
        
        try {
          nc_ref = CosNaming::NamingContext::_narrow(object_ref);
        } catch (const CORBA::Exception& ex) {
          // Is not a NamingContext
          if (m_orb->trace != NULL){
            TIDorb::util::StringBuffer msg;
            msg << "NamingContextImpl::bind_context() found name " << n[0].id << "." << n[0].kind;
            msg << " but is not a NamingContext";
            m_orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
          }
          throw CosNaming::NamingContext::NotFound(CosNaming::NamingContext::not_context, n);
        }
        // Is a correct NamingContext
        
        // Continues propagating resolve request
        CosNaming::Name n_next;
        n_next.length(name_length - 1);
        for (CORBA::ULong i=0; i < (name_length-1); ++i) {
          n_next[i] = n[i+1];
        }
        
        return nc_ref->bind_context(n_next, nc);
      }
      else // (it != m_naming_contexts.end())
        throw CosNaming::NamingContext::NotFound(CosNaming::NamingContext::missing_node, n);
      
    }
    else { // (name_length == 1)
      
      // Name couldn't be in m_naming_contexts or m_object_refs tables yet
      ObjectRefTable::iterator ob_it = m_object_refs.find(n[0]);
      if (ob_it != m_object_refs.end()) 
        throw CosNaming::NamingContext::AlreadyBound();
      else {
        NamingContextTable::iterator nc_it = m_naming_contexts.find(n[0]);
        if (nc_it == m_naming_contexts.end()) {
          char* str_obj = m_orb->object_to_string(nc);
          m_naming_contexts[n[0]] = str_obj;

          if (NamingContextIOFactory::getFactory().isPersistentFactory()) {
            // Operate the persistence
            try
            {
              NamingInternals::ReferenceSeq refs;
              
              refs.length(1);
              refs[0] = NamingInternals::Reference(
                  n[0], CosNaming::ncontext, str_obj);
            
              NamingContextIOFactory::getFactory().getWriter().store(
                  m_string_oid, refs);
            }
            catch (NamingInternals::PersistenceException & e)
            {
              if (m_orb->trace != NULL) {
                TIDorb::util::StringBuffer msg;
                msg << "NamingContextImpl::bind_context() error: " << e.msg;
                m_orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
              }
              throw CosNaming::NamingContext::CannotProceed(NULL, n);
            }
          }
          
        }
        else
          throw CosNaming::NamingContext::AlreadyBound();
      }
    }
  }
  else // (name_length =< 0)
    throw CosNaming::NamingContext::InvalidName();
  
}


void 
TIDNaming::NamingContextImpl::rebind_context (const CosNaming::Name& n,
                                              CosNaming::NamingContext_ptr nc)
  throw (CORBA::SystemException, 
         CosNaming::NamingContext::NotFound, 
         CosNaming::NamingContext::CannotProceed,
         CosNaming::NamingContext::InvalidName)
{
  TIDThr::Synchronized sync(m_recursive_mutex);

  if (m_destroying)
    return;

  CORBA::ULong name_length = n.length();

  if (name_length > 0) {

    if (name_length > 1) {
      // In this case (see "2.1.1 Resolution of Compound Names"), resolution must
      // be done over m_naming_contexts table only!
      NamingContextTable::iterator it = m_naming_contexts.find(n[0]);
      
      if (it != m_naming_contexts.end()) {
        
        CORBA::Object_var object_ref = m_orb->string_to_object((*it).second);
        CosNaming::NamingContext_var nc_ref = NULL;
        
        try {
          nc_ref = CosNaming::NamingContext::_narrow(object_ref);
        } catch (const CORBA::Exception& ex) {
          // Is not a NamingContext
          if (m_orb->trace != NULL){
            TIDorb::util::StringBuffer msg;
            msg << "NamingContextImpl::rebind_context() found name " << n[0].id << "." << n[0].kind;
            msg << " but is not a NamingContext";
            m_orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
          }
          throw CosNaming::NamingContext::NotFound(CosNaming::NamingContext::not_context, n);
        }
        // Is a correct NamingContext
        
        // Continues propagating resolve request
        CosNaming::Name n_next;
        n_next.length(name_length - 1);
        for (CORBA::ULong i=0; i < (name_length-1); ++i) {
          n_next[i] = n[i+1];
        }
       
        return nc_ref->rebind_context(n_next, nc);
      }
      else // (it != m_naming_contexts.end())
        throw CosNaming::NamingContext::NotFound(CosNaming::NamingContext::missing_node, n);
      
    }
    else { // (name_length == 1)
      
      // Name can be in m_naming_contexts or m_object_refs tables yet
      ObjectRefTable::iterator ob_it = m_object_refs.find(n[0]);
      if (ob_it != m_object_refs.end()) 
        return; // OK
      else {
        NamingContextTable::iterator nc_it = m_naming_contexts.find(n[0]);
        if (nc_it == m_naming_contexts.end()) {
          char* str_obj = m_orb->object_to_string(nc);
          m_naming_contexts[n[0]] = str_obj;

          if (NamingContextIOFactory::getFactory().isPersistentFactory()) {
            // Operate the persistence
            try
            {
              NamingInternals::ReferenceSeq refs;
            
              refs.length(1);
              refs[0] = NamingInternals::Reference(
                  n[0], CosNaming::ncontext, str_obj);
            
              NamingContextIOFactory::getFactory().getWriter().store(
                  m_string_oid, refs);
            }
            catch (NamingInternals::PersistenceException & e)
            {
              if (m_orb->trace != NULL) {
                TIDorb::util::StringBuffer msg;
                msg << "NamingContextImpl::rebind_context() Error: " << e.msg;
                m_orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
              }
              throw CosNaming::NamingContext::CannotProceed(NULL, n);
            }
          }

        }
        else
          return; // OK
      }
    }
  }
  else // (name_length =< 0)
    throw CosNaming::NamingContext::InvalidName();

}


CosNaming::NamingContext_ptr 
TIDNaming::NamingContextImpl::bind_new_context (const CosNaming::Name& n)
  throw (CORBA::SystemException, 
         CosNaming::NamingContext::NotFound,
         CosNaming::NamingContext::CannotProceed, 
         CosNaming::NamingContext::InvalidName,
         CosNaming::NamingContext::AlreadyBound)
{
  TIDThr::Synchronized sync(m_recursive_mutex);

  if (m_destroying)
    return CosNaming::NamingContext::_nil();

  CosNaming::NamingContext_ptr new_nc;
  try {
    new_nc = this->new_context();
  } catch(const CORBA::SystemException& ex) {
    if (m_orb->trace != NULL){
      TIDorb::util::StringBuffer msg;
      msg << "NamingContextImpl::bind_new_context() cannot create a new context ";
      m_orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data(), ex);
    }
    throw CosNaming::NamingContext::CannotProceed(NULL, n);
  }
  bind_context(n, new_nc);
  return new_nc;
}


void 
TIDNaming::NamingContextImpl::list (CORBA::ULong how_many,
                                    CosNaming::BindingList*& bl,
                                    CosNaming::BindingIterator_ptr& bi)
  throw (CORBA::SystemException)
{
  TIDThr::Synchronized sync(m_recursive_mutex);

  if (m_destroying)
    return;

  if (how_many == 0)
    throw CORBA::BAD_PARAM();

  char string_oid[40];
  sprintf(string_oid, "bi-%li-%u.ctx", (long)time(0), m_oid_count.inc());

  // Creates a new servant for the BindingIterator
  TIDNaming::BindingIteratorImpl* bi_servant = 
    new TIDNaming::BindingIteratorImpl(m_orb,
                                       m_binding_iterators_poa,
                                       m_namingcontext_current,
                                       this,
                                       m_binding_iterators_count);

  // Activate bi_servant in his POA
  PortableServer::ObjectId_var oid = 
    PortableServer::string_to_ObjectId(string_oid);
  
  m_binding_iterators_poa->activate_object_with_id(oid, bi_servant);


  // Obtain bi object references
  CORBA::Object_var obj_reference =  
    m_binding_iterators_poa->id_to_reference(oid);
  bi = CosNaming::BindingIterator::_narrow(obj_reference);

  // Add BindingIterator to m_binding_iterators UseTable
  m_binding_iterators.append(m_binding_iterators_count, 
                             new TIDNaming::BindingIterator_ref(bi));

  m_binding_iterators_count++;
  if (m_binding_iterators_count == TIDorb::core::util::ULong::MAX_VALUE) 
    m_binding_iterators_count++;

  
  // Call to destroy() all iterators to be removed from the m_binding_iterators table
  vector<TIDNaming::BindingIterator_ref*>* removed = m_binding_iterators.getRemovedObjects();
  if (removed != NULL) {
    if (m_orb->trace != NULL)
      m_orb->print_trace(TIDorb::util::TR_DEBUG, 
                         "NamingContextImpl::list removing removed objects");

    TIDNaming::BindingIterator_ref* biter = NULL;
    vector<TIDNaming::BindingIterator_ref*>::iterator i   = removed->begin();
    vector<TIDNaming::BindingIterator_ref*>::iterator end = removed->end();
    while (i != end){
      biter = (TIDNaming::BindingIterator_ref*) (*i);
      if (*biter != NULL) {
        try {
          (*(*i))->destroy();
        } catch (...) {
          if (m_orb->trace != NULL)
            m_orb->print_trace(TIDorb::util::TR_ERROR, 
                               "NamingContextImpl::list unexpected exception destroying biter");
        }
      }
      i++;
    }
    removed->clear();
    delete removed;
  }



  if (how_many < 1) {
    bl = new CosNaming::BindingList;
    bl->length(0);
  }
  else {

    // Call to next_n in order to obtain the binding list
    CORBA::Boolean ret = bi->next_n(how_many, bl);

    // Anything to do if ret is false?
    // Redundant:
//     if (!ret) {
//       delete bl;
//       bl = new CosNaming::BindingList;
//       bl->length(0);
//     }

  }

}  


void 
TIDNaming::NamingContextImpl::destroy ()  
  throw (CORBA::SystemException, CosNaming::NamingContext::NotEmpty)
{
  if (m_destroying)
    return;

  m_destroying = true;

  if (m_orb->trace != NULL){
    TIDorb::util::StringBuffer msg;
    msg << "Destroying naming context '" << m_string_oid << "'... ";
    m_orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
  }


  {
    TIDThr::Synchronized synchro(m_recursive_mutex);
    
    size_t number_bindings = m_naming_contexts.size() + m_object_refs.size();

    if (number_bindings > 0)
    {
      if (m_orb->trace != NULL)
        m_orb->print_trace(TIDorb::util::TR_ERROR, "Failed: not empty");
      m_destroying = false;
      throw CosNaming::NamingContext::NotEmpty();
    }
    

    //
    // Delete and desactivate m_binding_iterators
    //
    vector<TIDNaming::BindingIterator_ref*>* elements = 
      m_binding_iterators.elements();

    if (elements != NULL) {
      if (m_orb->trace != NULL)
        m_orb->print_trace(TIDorb::util::TR_DEBUG, 
                           "NamingContextImpl::destroy removing binding iterators");
      TIDNaming::BindingIterator_ref* biter = NULL;
      vector<TIDNaming::BindingIterator_ref*>::iterator it  = elements->begin();
      vector<TIDNaming::BindingIterator_ref*>::iterator end = elements->end();
      while (it != end){
        biter = (TIDNaming::BindingIterator_ref*) (*it);
        if (*biter != NULL) {
          try {
            (*biter)->destroy();
          } catch (...) {
            if (m_orb->trace != NULL)
              m_orb->print_trace(TIDorb::util::TR_ERROR, 
                                 "NamingContextImpl::destroy unexpected exception destroying biter");
          }
        }
        it++;
      }
      m_binding_iterators.clear();
      elements->clear();
      delete elements;
    }


    // Remove reference from the Active Object Map
    try {
      
      PortableServer::ObjectId_var oid =
        m_namingcontext_current->get_object_id();
      
      CORBA::String_var str_oid = 
        PortableServer::ObjectId_to_string(oid);
      
      if (m_orb->trace != NULL){
        TIDorb::util::StringBuffer msg;
        msg << "NamingContextImpl::destroying NameContext with OID=" << str_oid;;
        m_orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
      }    
      
      m_namingcontext_poa->deactivate_object(oid);
      
      if (NamingContextIOFactory::getFactory().isPersistentFactory()) {
        NamingContextIOFactory::getFactory().getWriter().removeContext(
                  m_string_oid);
      }
      
    } catch (NamingInternals::PersistenceException & e) {
      if (m_orb->trace != NULL){
        TIDorb::util::StringBuffer msg;
        msg << "NamingContextImpl::destroy() Exception " << e.msg;
        m_orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
      }
    } catch (const CORBA::Exception& ex) {
      if (m_orb->trace != NULL){
        TIDorb::util::StringBuffer msg;
        msg << "NamingContextImpl::destroy() Exception ";
        m_orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data(), ex);
      }
    } catch (...) {
      if (m_orb->trace != NULL){
        TIDorb::util::StringBuffer msg;
        msg << "NamingContextImpl::destroy() Unexpected exception";
        m_orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
      }    
    } 
    
    if (m_orb->trace != NULL){
      TIDorb::util::StringBuffer msg;
      msg << "Destroying naming context '" << m_string_oid << "' done ";
      m_orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
    }
  }
  
}

char * 
TIDNaming::NamingContextImpl::getObjectId()
{
  TIDThr::Synchronized sync(m_recursive_mutex);

  return strdup(m_string_oid);
}

void 
TIDNaming::NamingContextImpl::getModifiedReferences(
      NamingInternals::ReferenceSeq*& refs)
{
  TIDThr::Synchronized sync(m_recursive_mutex);

  refs = new NamingInternals::ReferenceSeq(0);
  refs->length(m_mods.size());
  for (size_t i = 0; i < m_mods.size(); i++)
    (*refs)[i] = m_mods[i];
}


void 
TIDNaming::NamingContextImpl::setObjectIdCounter(CORBA::ULong value)
{
  CORBA::ULong val = m_oid_count.get_value();
  while (val < value)
  {
    m_oid_count.inc();
    val = m_oid_count.get_value();
  }
}

// -*- c++ -*-
/*
 *  File name: BindingIteratorImpl.C
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
#include "BindingIteratorImpl.h"


TIDNaming::BindingIteratorImpl::BindingIteratorImpl(
                                    TIDorb::core::TIDORB* orb, 
                                    PortableServer::POA_ptr binding_iterators_poa,
                                    PortableServer::Current_ptr current,
                                    TIDNaming::NamingContextImpl* nc_impl,
                                    CORBA::ULong order_counter)
  : m_bindingiterators_poa(binding_iterators_poa), 
    m_bindingiterators_current(current),
    m_nc_impl(nc_impl),
    m_destroying(false),
    m_orb(orb),
    m_order_counter(order_counter)
{
  m_nc_it = nc_impl->m_naming_contexts.begin();
  m_ob_it = nc_impl->m_object_refs.begin();
  m_nc_original_size = nc_impl->m_naming_contexts.size();
  m_ob_original_size = nc_impl->m_object_refs.size();
}

  
TIDNaming::BindingIteratorImpl::~BindingIteratorImpl()
{
  if (!m_destroying)
    this->destroy();
}


CORBA::Boolean 
TIDNaming::BindingIteratorImpl::next_one (CosNaming::Binding*& b)
  throw (CORBA::SystemException)
{
  TIDThr::Synchronized sync(m_recursive_mutex);
  
  m_nc_impl->m_binding_iterators.use(m_order_counter);

  CosNaming::BindingList* bl;
  CORBA::Boolean ret = this->next_n((CORBA::ULong)1, bl); 
  if (ret) {
    b = new CosNaming::Binding((*bl)[0]);
    delete bl;
  }
  else {
    delete bl; // bl not needed any more
    b = new CosNaming::Binding;
  }
  
  return ret;
}

  
CORBA::Boolean 
TIDNaming::BindingIteratorImpl::next_n (CORBA::ULong how_many,
                                        CosNaming::BindingList*& bl)
  throw (CORBA::SystemException)
{
  TIDThr::Synchronized sync(m_recursive_mutex);

  m_nc_impl->m_binding_iterators.use(m_order_counter);

  if (how_many == 0)
    throw CORBA::BAD_PARAM();

  CORBA::ULong i = 0;
  bl = new CosNaming::BindingList;
  bl->length(i);
  
  if (m_destroying)
    return false;

  
  //
  // Check that NamingContextTable and ObjectRefTable haven't been modified
  // Iterators are invalided
  if ( (m_nc_original_size != m_nc_impl->m_naming_contexts.size()) ||
       (m_ob_original_size != m_nc_impl->m_object_refs.size())      ) {
    if (m_orb->trace != NULL){
      TIDorb::util::StringBuffer msg;
      msg << "NamingContextTable and/or ObjectRefTable have been modified ";
      msg << "after create this BindingIterator. ";
      msg << "BindingIterator invalided!";
      m_orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
    }
    return false;
  }

  //
  // Obtains next elements from NamingContextTable
  //
  NamingContextImpl::NamingContextTable::iterator nc_end =
    m_nc_impl->m_naming_contexts.end();

  while ( (i < how_many) && (m_nc_it != nc_end) ) {

    // Create a new Binding from the next element
    CosNaming::Name binding_name;
    binding_name.length(1);
    binding_name[0].id   = CORBA::string_dup(((*m_nc_it).first).id);
    binding_name[0].kind = CORBA::string_dup(((*m_nc_it).first).kind);

    // Add next element to the BindingList
    bl->length(i+1);
    (*bl)[i] = CosNaming::Binding(binding_name, CosNaming::ncontext);

    // Inc the iterators and counters
    m_nc_it++;
    i++;
  }

  
  //
  // Continue obtaining elements from ObjectRefTable  
  //
  NamingContextImpl::NamingContextTable::iterator ob_end =
    m_nc_impl->m_object_refs.end();

  while ( (i < how_many) && (m_ob_it != ob_end)) {

    // Crear el Binding del nuevo elemento
    CosNaming::Name binding_name;
    binding_name.length(1);
    binding_name[0].id   = CORBA::string_dup(((*m_ob_it).first).id);
    binding_name[0].kind = CORBA::string_dup(((*m_ob_it).first).kind);

    // Add next element to the BindingList
    bl->length(i+1);
    (*bl)[i] = CosNaming::Binding(binding_name, CosNaming::nobject);

    // Inc the iterators and counters
    m_ob_it++;
    i++;
  }
    

  // Returns true if bl is a non-zero length sequence and false if
  // there are no more bindings and bl is a zero-length sequence
  return (i > 0);
}

void TIDNaming::BindingIteratorImpl::destroy ()
  throw (CORBA::SystemException)
{
  if (m_destroying)
    return;

  m_destroying = true;

  if (m_orb->trace != NULL){
    TIDorb::util::StringBuffer msg;
    msg << "Destroying binding iterator #'" << m_order_counter << "'... ";
    m_orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
  }

  try { 

    TIDThr::Synchronized synchro(m_recursive_mutex);

    PortableServer::ObjectId_var oid = 
      m_bindingiterators_poa->servant_to_id(this);
    
    // Deactivate object: remove form Active Object Map
    m_bindingiterators_poa->deactivate_object(oid);
    
    // Remove from m_binding_iterators table of NamingContext
    TIDNaming::BindingIterator_ref* removed = 
      m_nc_impl->m_binding_iterators.remove(m_order_counter);

    if (removed != NULL)
      delete removed;
    
    // Remove the current servant
    this->_remove_ref();

  } catch (const CORBA::Exception& ex) {
    if (m_orb->trace != NULL){
      TIDorb::util::StringBuffer msg;
      msg << "BindingIteratorImpl::destroy() Exception ";
      m_orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data(), ex);
    } 
  } catch (...) {
    if (m_orb->trace != NULL){
      TIDorb::util::StringBuffer msg;
      msg << "BindingIteratorImpl::destroy() Unexpected exception";
      m_orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
    }    
  }

}

// -*- c++ -*-
/*
 *  File name: NamingContextExtImpl.C
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
#include "NamingContextExtImpl.h"


TIDNaming::NamingContextExtImpl::NamingContextExtImpl(
                                             TIDorb::core::TIDORB* orb, 
                                             PortableServer::POA_ptr namingcontext_poa,
                                             PortableServer::POA_ptr binding_iterators_poa,
                                             int max_binding_iterators,
                                             const char* string_oid)
  : TIDNaming::NamingContextImpl::NamingContextImpl(orb, 
                                                    namingcontext_poa,
                                                    binding_iterators_poa,
                                                    max_binding_iterators,
                                                    string_oid)
{
 
  if (m_orb->trace != NULL){
    TIDorb::util::StringBuffer msg;
    msg << "NamingContextExtImpl() OID=" << string_oid;
    m_orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
  }

}


TIDNaming::NamingContextExtImpl::~NamingContextExtImpl()
{

}



CosNaming::NamingContextExt::StringName 
TIDNaming::NamingContextExtImpl::to_string(const ::CosNaming::Name& n) 
  throw (CosNaming::NamingContext::InvalidName, 
         CORBA::SystemException)
{
  return TIDorb::core::util::Corbaname::to_string((const ::CosNaming::Name*)&n);
}


CosNaming::Name* 
TIDNaming::NamingContextExtImpl::to_name (const char* sn ) 
  throw (CORBA::SystemException, 
         CosNaming::NamingContext::InvalidName)
{
  return TIDorb::core::util::Corbaname::to_name(sn);
}

CosNaming::NamingContextExt::URLString  
TIDNaming::NamingContextExtImpl::to_url (const char* addr,
                                         const char* sn ) 
  throw (CORBA::SystemException, 
         CosNaming::NamingContextExt::InvalidAddress, 
         CosNaming::NamingContext::InvalidName)
{
  if (strcmp(addr, "") == 0)
    throw CosNaming::NamingContextExt::InvalidAddress();
  else{
    TIDorb::util::StringBuffer url;
    char* addr_translated = 
      TIDorb::util::TranslateURLEscapes::translate_escapes(addr);
    char* sn_translated = 
      TIDorb::util::TranslateURLEscapes::translate_escapes(sn);
    url << "corbaname:";
    url << addr_translated;
    url << "#";
    url << sn_translated;
    CORBA::string_free(addr_translated);
    CORBA::string_free(sn_translated);
    return strdup(url.str().data());
  }
}

CORBA::Object_ptr 
TIDNaming::NamingContextExtImpl::resolve_str (const char* sn ) 
  throw (CORBA::SystemException, 
         CosNaming::NamingContext::NotFound, 
         CosNaming::NamingContext::CannotProceed, 
         CosNaming::NamingContext::InvalidName)
{
  CosNaming::Name_var name = TIDorb::core::util::Corbaname::to_name(sn);
  return NamingContextImpl::resolve(*name);
}



//
// Inherited methods from NamingContext interface
// TODO improvement: change to inlined 

CORBA::Object_ptr 
TIDNaming::NamingContextExtImpl::resolve (const ::CosNaming::Name& n)
  throw (CORBA::SystemException, 
         CosNaming::NamingContext::NotFound, 
         CosNaming::NamingContext::CannotProceed, 
         CosNaming::NamingContext::InvalidName)
{
  return TIDNaming::NamingContextImpl::resolve (n);
}


void 
TIDNaming::NamingContextExtImpl::bind (const ::CosNaming::Name& n, CORBA::Object_ptr obj)
  throw (CORBA::SystemException, 
         CosNaming::NamingContext::NotFound, 
         CosNaming::NamingContext::CannotProceed, 
         CosNaming::NamingContext::InvalidName, 
         CosNaming::NamingContext::AlreadyBound)
{
  TIDNaming::NamingContextImpl::bind (n, obj);
}


void 
TIDNaming::NamingContextExtImpl::rebind (const ::CosNaming::Name& n, CORBA::Object_ptr obj)
  throw (CORBA::SystemException, 
         CosNaming::NamingContext::NotFound, 
         CosNaming::NamingContext::CannotProceed, 
         CosNaming::NamingContext::InvalidName)
{
  TIDNaming::NamingContextImpl::rebind (n, obj);
}



void 
TIDNaming::NamingContextExtImpl::unbind (const ::CosNaming::Name& n)
  throw (CORBA::SystemException, 
         CosNaming::NamingContext::NotFound, 
         CosNaming::NamingContext::CannotProceed, 
         CosNaming::NamingContext::InvalidName)
{
  TIDNaming::NamingContextImpl::unbind (n);
}


CosNaming::NamingContext_ptr 
TIDNaming::NamingContextExtImpl::new_context ()
  throw (CORBA::SystemException)
{
  return TIDNaming::NamingContextImpl::new_context ();
}

void 
TIDNaming::NamingContextExtImpl::bind_context (const CosNaming::Name& n,
                                            CosNaming::NamingContext_ptr nc)
  throw (CORBA::SystemException, 
         CosNaming::NamingContext::NotFound, 
         CosNaming::NamingContext::CannotProceed,
         CosNaming::NamingContext::InvalidName, 
         CosNaming::NamingContext::AlreadyBound)
{
  TIDNaming::NamingContextImpl::bind_context (n,nc);
}


void 
TIDNaming::NamingContextExtImpl::rebind_context (const CosNaming::Name& n,
                                              CosNaming::NamingContext_ptr nc)
  throw (CORBA::SystemException, 
         CosNaming::NamingContext::NotFound, 
         CosNaming::NamingContext::CannotProceed,
         CosNaming::NamingContext::InvalidName)
{
  TIDNaming::NamingContextImpl::rebind_context (n,nc);
}

CosNaming::NamingContext_ptr 
TIDNaming::NamingContextExtImpl::bind_new_context (const CosNaming::Name& n)
  throw (CORBA::SystemException, 
         CosNaming::NamingContext::NotFound,
         CosNaming::NamingContext::CannotProceed, 
         CosNaming::NamingContext::InvalidName,
         CosNaming::NamingContext::AlreadyBound)
{
  return TIDNaming::NamingContextImpl::bind_new_context ( n);
}


void 
TIDNaming::NamingContextExtImpl::list (CORBA::ULong how_many,
                                    CosNaming::BindingList*& bl,
                                    CosNaming::BindingIterator_ptr& bi)
  throw (CORBA::SystemException)
{
  TIDNaming::NamingContextImpl::list (how_many,bl,bi);
}


void 
TIDNaming::NamingContextExtImpl::destroy ()  
  throw (CORBA::SystemException, CosNaming::NamingContext::NotEmpty)
{
  TIDNaming::NamingContextImpl::destroy ();
}


char * 
TIDNaming::NamingContextExtImpl::getObjectId()
{
  return NamingContextImpl::getObjectId();
}

void 
TIDNaming::NamingContextExtImpl::getModifiedReferences(
      NamingInternals::ReferenceSeq*& refs)
{
  NamingContextImpl::getModifiedReferences(refs);
}

//
// Redefinition of some internal methods
// TODO improvement: change to inlined 

void TIDNaming::NamingContextExtImpl::invoke(CORBA::ServerRequest* request)
{
  this->POA_NamingInternals::InternalNamingContextExt::invoke(request);
}


char* 
TIDNaming::NamingContextExtImpl::_primary_interface(
                                           const PortableServer::ObjectId& oid, 
                                           PortableServer::POA* poa)
{
  return this->POA_NamingInternals::InternalNamingContextExt::_primary_interface(oid, poa);
}


const CORBA::RepositoryIdSeq_ptr
TIDNaming::NamingContextExtImpl::_all_interfaces(PortableServer::POA* poa, 
                                                   const PortableServer::ObjectId& oid)
{
  return this->POA_NamingInternals::InternalNamingContextExt::_all_interfaces(poa, oid);
}


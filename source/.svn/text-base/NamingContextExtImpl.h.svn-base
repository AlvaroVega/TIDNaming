// -*- c++ -*-
/*
 *  File name: NamingContextExtImpl.h
 *  File type: Header file.
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
#include "TIDNaming.h"

#ifndef _NAMING_CONTEXT_EXTImpl_H_
#define _NAMING_CONTEXT_EXTImpl_H_

#include "POA_NamingInternals.h"

#include <map>

using namespace std;


namespace TIDNaming{

class NamingContextExtImpl: public virtual POA_NamingInternals::InternalNamingContextExt,
                            public virtual TIDNaming::NamingContextImpl {
private:

  
public:
  NamingContextExtImpl(TIDorb::core::TIDORB* orb, 
                       PortableServer::POA_ptr namingcontext_poa,
                       PortableServer::POA_ptr binding_iterators_poa,
                       int max_binding_iterators,
                       const char* string_oid);
  
  ~NamingContextExtImpl();



  CosNaming::NamingContextExt::StringName 
  to_string(const ::CosNaming::Name& n) 
    throw (CosNaming::NamingContext::InvalidName, 
           CORBA::SystemException);

  CosNaming::Name* 
  to_name (const char* sn ) 
    throw (CORBA::SystemException, 
           CosNaming::NamingContext::InvalidName);

  CosNaming::NamingContextExt::URLString  
  to_url (const char* addr,
          const char* sn ) 
    throw (CORBA::SystemException, 
           CosNaming::NamingContextExt::InvalidAddress, 
           CosNaming::NamingContext::InvalidName);

  CORBA::Object_ptr 
  resolve_str (const char* sn ) 
    throw (CORBA::SystemException, 
           CosNaming::NamingContext::NotFound, 
           CosNaming::NamingContext::CannotProceed, 
           CosNaming::NamingContext::InvalidName);
  
  //
  // Inherited methods from NamingContext interface
  // TODO improvement: change to inlined 
  CORBA::Object* resolve(const CosNaming::Name&)
    throw (CORBA::SystemException, 
           CosNaming::NamingContext::NotFound, 
           CosNaming::NamingContext::CannotProceed, 
           CosNaming::NamingContext::InvalidName);

  void bind (const ::CosNaming::Name& n, CORBA::Object_ptr obj)
    throw (CORBA::SystemException, 
           CosNaming::NamingContext::NotFound, 
           CosNaming::NamingContext::CannotProceed, 
           CosNaming::NamingContext::InvalidName, 
           CosNaming::NamingContext::AlreadyBound);
  
  void rebind (const ::CosNaming::Name& n, CORBA::Object_ptr obj)
    throw (CORBA::SystemException, 
           CosNaming::NamingContext::NotFound, 
           CosNaming::NamingContext::CannotProceed, 
           CosNaming::NamingContext::InvalidName);

  void unbind (const ::CosNaming::Name& n)
    throw (CORBA::SystemException, 
           CosNaming::NamingContext::NotFound, 
           CosNaming::NamingContext::CannotProceed, 
           CosNaming::NamingContext::InvalidName);
  
  CosNaming::NamingContext_ptr new_context ()
    throw (CORBA::SystemException);
  
  void bind_context (const CosNaming::Name& n,
                     CosNaming::NamingContext_ptr nc)
    throw (CORBA::SystemException, 
           CosNaming::NamingContext::NotFound, 
           CosNaming::NamingContext::CannotProceed,
           CosNaming::NamingContext::InvalidName, 
           CosNaming::NamingContext::AlreadyBound);
  
  void rebind_context (const CosNaming::Name& n,
                       CosNaming::NamingContext_ptr nc)
    throw (CORBA::SystemException, 
           CosNaming::NamingContext::NotFound, 
           CosNaming::NamingContext::CannotProceed,
           CosNaming::NamingContext::InvalidName);
  
  CosNaming::NamingContext_ptr bind_new_context (const CosNaming::Name& n)
    throw (CORBA::SystemException, 
           CosNaming::NamingContext::NotFound,
           CosNaming::NamingContext::CannotProceed, 
           CosNaming::NamingContext::InvalidName,
           CosNaming::NamingContext::AlreadyBound);
  
  void list (CORBA::ULong how_many,
             CosNaming::BindingList*& bl,
             CosNaming::BindingIterator_ptr& bi)
    throw (CORBA::SystemException);
  
  void destroy ()  
    throw (CORBA::SystemException, 
           CosNaming::NamingContext::NotEmpty);


  //
  // Internal function members
  //
  
  char * getObjectId();
  
  void getModifiedReferences(NamingInternals::ReferenceSeq*& refs);

  
  //
  // Redefinition of some internal methods
  // TODO improvement: change to inlined 
  void invoke(CORBA::ServerRequest* request);
  
  char* _primary_interface(const PortableServer::ObjectId& oid, 
                           PortableServer::POA* poa);

  const CORBA::RepositoryIdSeq_ptr _all_interfaces(PortableServer::POA* poa, 
                                                    const PortableServer::ObjectId& oid);



};

} //TIDNaming


#endif

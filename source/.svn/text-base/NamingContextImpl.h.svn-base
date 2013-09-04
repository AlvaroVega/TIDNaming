// -*- c++ -*-
/*
 *  File name: NamingContextImpl.h
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

#ifndef _NAMING_CONTEXTImpl_H_
#define _NAMING_CONTEXTImpl_H_

#include "POA_NamingInternals.h"

#include <string.h>
#include <map>

using namespace std;


namespace TIDNaming{

class NamingContextImpl: public virtual POA_NamingInternals::InternalNamingContext
{
protected:
  char*                       m_string_oid;
  TIDorb::core::TIDORB*       m_orb;
  bool                        m_destroying; 
  static TIDorb::core::util::Counter m_oid_count;

private:

  TIDThr::RecursiveMutex      m_recursive_mutex;

  PortableServer::POA_ptr     m_namingcontext_poa;
  PortableServer::POA_ptr     m_binding_iterators_poa;
  PortableServer::Current_ptr m_namingcontext_current;


  //
  // NameComponent comparator function
  //
  struct nc_compare {
    bool operator()(const CosNaming::NameComponent& s1, 
                    const CosNaming::NameComponent& s2) const
    {
      int cmp_id = strcmp(s1.id.in(), s2.id.in());
      if (cmp_id == 0)
        return (strcmp(s1.kind.in(), s2.kind.in()) < 0);
      else
        return cmp_id < 0;
    }
  };


  //
  // Table of NamingContext
  // Due to stores stringfied persistents IORs , need able to be serialized
  //
  typedef map<CosNaming::NameComponent, const char *, nc_compare> NamingContextTable;

  NamingContextTable             m_naming_contexts;


  //
  // Table of ObjectRef
  // Due to stores stringfied persistents IORs , need able to be serialized
  //
  typedef map<CosNaming::NameComponent, const char *, nc_compare> ObjectRefTable;

  ObjectRefTable                 m_object_refs;


  //
  // All modifications are stored here and removed when stored
  //
  typedef vector<NamingInternals::Reference_var> ReferenceVector;
  ReferenceVector                m_mods;


  // TODO?: Implement a wrapper to ObjectRefTable and NamingContextTable
  // to be used from NamingContextImpl and BindingIteratorImpl


  //
  // Table of BindingIterators (not need able to be serialized)
  // Due to stores stringfied transients IORs, not need able to be serialized
  // "2.3.2 Garbage Collection of Iterators"
  //
  typedef TIDorb::core::util::UseTable<CORBA::ULong, TIDNaming::BindingIterator_ref> 
                                                              BindingIteratorsTable;

  BindingIteratorsTable          m_binding_iterators;

  CORBA::ULong                   m_binding_iterators_count; 

  //
  // Flag of modified state
  // Whenever any bind* operation is done, this flag is set to true
  // This check is used to determine wheter this context needs to be
  // stored in disk
  bool m_modified;

public:

  NamingContextImpl(TIDorb::core::TIDORB* orb, 
                    PortableServer::POA_ptr namingcontext_poa,
                    PortableServer::POA_ptr binding_iterators_poa,
                    int max_binding_iterators,
                    const char* string_oid);

  virtual ~NamingContextImpl();

  CORBA::Object_ptr resolve (const ::CosNaming::Name& n)
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
  // Methods to manipulate object Id counter
  ///

  static void setObjectIdCounter(CORBA::ULong value);
  
  friend class BindingIteratorImpl;
};

} //TIDNaming

  
#endif

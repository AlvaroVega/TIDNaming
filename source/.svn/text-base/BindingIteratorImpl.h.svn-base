// -*- c++ -*-
/*
 *  File name: BindingIteratorImpl.h
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

#ifndef _BINDING_ITERATORImpl_H_
#define _BINDING_ITERATORImpl_H_

#include "POA_CosNaming.h"

#include <map>

using namespace std;


namespace TIDNaming{

class BindingIteratorImpl: public virtual POA_CosNaming::BindingIterator
{
private:

  TIDThr::RecursiveMutex      m_recursive_mutex;
  
  PortableServer::POA_ptr     m_bindingiterators_poa;
  PortableServer::Current_ptr m_bindingiterators_current;

  TIDNaming::NamingContextImpl*    m_nc_impl;

  NamingContextImpl::NamingContextTable::iterator m_nc_it;
  NamingContextImpl::ObjectRefTable::iterator     m_ob_it;

  size_t m_nc_original_size;
  size_t m_ob_original_size;

  bool                        m_destroying; 
  TIDorb::core::TIDORB*       m_orb;
  CORBA::ULong                m_order_counter; // Id in BindingIteratorsTable

public:
  
  BindingIteratorImpl(TIDorb::core::TIDORB* orb, 
                      PortableServer::POA_ptr binding_iterators_poa,
                      PortableServer::Current_ptr pcurrent,
                      TIDNaming::NamingContextImpl* nc_impl,
                      CORBA::ULong m_order_counter);
  
  ~BindingIteratorImpl();
  
  CORBA::Boolean next_one (CosNaming::Binding*& b)
    throw (CORBA::SystemException);
  
  CORBA::Boolean next_n (CORBA::ULong how_many,
                         CosNaming::BindingList*& bl)
    throw (CORBA::SystemException);
  
  void destroy ()
    throw (CORBA::SystemException);

};



class BindingIterator_ref: 
    public TIDorb::templates::InterfaceT_var<CosNaming::BindingIterator>,
    public TIDorb::core::util::RemovableObject {

public:
  
  BindingIterator_ref(CosNaming::BindingIterator* binding_iterator)
    : TIDorb::templates::InterfaceT_var<CosNaming::BindingIterator>(binding_iterator) {
  }

  ~BindingIterator_ref() {}

  bool canBeRemoved() {
    return true;
  }
};


} //TIDNaming


#endif

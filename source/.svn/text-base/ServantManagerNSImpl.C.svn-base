// -*- c++ -*-
/*
 *  File name: ServantManagerNSImpl.C
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
#include "ServantManagerNSImpl.h"

TIDNaming::ServantManagerNSImpl::ServantManagerNSImpl(
                                      CORBA::ORB_ptr orb,
                                      PortableServer::POA_ptr binding_iterators_poa,
                                      int max_binding_iterators)
{
  m_orb = dynamic_cast<TIDorb::core::TIDORB*> (CORBA::ORB::_duplicate(orb));
  m_binding_iterators_poa = binding_iterators_poa;
  m_max_binding_iterators = max_binding_iterators;
}

TIDNaming::ServantManagerNSImpl::~ServantManagerNSImpl()
{ 
  CORBA::release(m_orb);
}


PortableServer::Servant 
TIDNaming::ServantManagerNSImpl::incarnate (const PortableServer::ObjectId& oid, 
                                            PortableServer::POA_ptr adapter)
  throw (CORBA::SystemException, PortableServer::ForwardRequest)
{
  char* oid_str = PortableServer::ObjectId_to_string(oid);

  if (m_orb->trace != NULL){
    TIDorb::util::StringBuffer msg;
    msg << "ServantManagerNSImpl::incarnate oid=" << oid_str;
    m_orb->print_trace(TIDorb::util::TR_USER, msg.str().data());
  }

  int l = strlen(oid_str);
  if (l > 3) {
    if (oid_str[l-3] == 'c' && oid_str[l-2] == 't' && oid_str[l-1] == 'x') { // Is a NC?
      

      // TODO: Check if old nc_servant must be deleted

      // Creates it
      PortableServer::Servant nc_servant = 
        new TIDNaming::NamingContextExtImpl(m_orb, adapter, m_binding_iterators_poa, 
                                            m_max_binding_iterators, oid_str);

      if (m_orb->trace != NULL)
        m_orb->print_trace(TIDorb::util::TR_USER, "NamingContext servant created");

      delete oid_str;
      return nc_servant;
    }
  }

  if (m_orb->trace != NULL)
    m_orb->print_trace(TIDorb::util::TR_ERROR, "Not valid NamingContext. Not created");

  delete oid_str;
  throw CORBA::OBJECT_NOT_EXIST();
  return 0;
}

void TIDNaming::ServantManagerNSImpl::etherealize(const PortableServer::ObjectId& oid,
                                                  PortableServer::POA_ptr adapter,
                                                  PortableServer::Servant servant,
                                                  CORBA::Boolean cleanup_in_progress,
                                                  CORBA::Boolean remaining_activations)
  throw (CORBA::SystemException)
{
  char* oid_str = PortableServer::ObjectId_to_string(oid);

  if (m_orb->trace != NULL){
    TIDorb::util::StringBuffer msg;
    msg << "ServantManagerNSImpl::etherealize OID=" << oid_str;
    m_orb->print_trace(TIDorb::util::TR_USER, msg.str().data());
  }

  delete oid_str;
  
  // Servant could be deleted by destroy() because it calls to deactivate_object(oid)
  if (!remaining_activations) {
    try {
      servant->_remove_ref(); 
    } catch (...) {
      if (m_orb->trace != NULL){
        TIDorb::util::StringBuffer msg;
        msg << "ServantManagerNSImpl::etherealize: unknown exception removing servant ";
        m_orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
      }
    }
  }

}

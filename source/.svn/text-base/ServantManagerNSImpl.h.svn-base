// -*- c++ -*-
/*
 *  File name: ServantManagerNSImpl.h
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

#ifndef _SERVANT_MANAGER_NSImpl_H_
#define _SERVANT_MANAGER_NSImpl_H_


#include "POA_CosNaming.h"

#include <CORBA.h>
#include <PortableServer.h>
#include <IOP.h>


namespace TIDNaming{

class ServantManagerNSImpl:  
    public PortableServer::ServantActivator,
    public CORBA::LocalObject
{
private:

  TIDorb::core::TIDORB* m_orb;
  PortableServer::POA_ptr m_binding_iterators_poa;
  int m_max_binding_iterators;

public:

  ServantManagerNSImpl(CORBA::ORB_ptr orb,
                       PortableServer::POA_ptr binding_iterators_poa,
                       int max_binding_iterators);
  
  ~ServantManagerNSImpl();

  PortableServer::Servant incarnate (const PortableServer::ObjectId& oid,
                                     PortableServer::POA_ptr adapter)
    throw (CORBA::SystemException, PortableServer::ForwardRequest);
  
  void etherealize (const PortableServer::ObjectId& oid,
                    PortableServer::POA_ptr adapter,
                    PortableServer::Servant servant,
                    CORBA::Boolean cleanup_in_progress,
                    CORBA::Boolean remaining_activations)
    throw (CORBA::SystemException);

};

} //TIDNaming

#endif

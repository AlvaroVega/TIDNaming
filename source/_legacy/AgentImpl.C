// -*- c++ -*-
/*
 *  File name: AgentImpl.C
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
#include "AgentImpl.h"


TIDNamingAdmin::AgentImpl::AgentImpl(CORBA::ORB_ptr orb, const char* path)
{
  m_orb = dynamic_cast<TIDorb::core::TIDORB*> (CORBA::ORB::_duplicate(orb));
}


TIDNamingAdmin::AgentImpl::~AgentImpl()
{
  CORBA::release(m_orb);
}


void TIDNamingAdmin::AgentImpl::set_initial_reference (
                                   const char* object_name,
                                   CORBA::Object_ptr obj ) 
  throw (CORBA::SystemException, TIDNamingAdmin::InvalidName)
{
    m_orb->register_initial_reference(object_name, obj);

}



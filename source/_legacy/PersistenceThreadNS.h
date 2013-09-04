// -*- c++ -*-
/*
 *  File name: PersistenceThreadNS.h
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

#ifndef _PERSISTENCE_THREAD_NS_H_
#define _PERSISTENCE_THREAD_NS_H_


#include "TIDThr.h"

#include <CORBA.h>
#include <PortableServer.h>
#include <IOP.h>


namespace TIDNaming{


class PersistenceThreadNS : public TIDThr::Thread {
 private:  
  TIDorb::core::TIDORB*                          m_orb;
  NamingInternals::InternalNamingContextExt_ptr  internal_root_nc;

 public:

  PersistenceThreadNS(TIDorb::core::TIDORB* orb, CORBA::Object_ptr obj);
  ~PersistenceThreadNS() throw();

  void run();

};

} //TIDNaming

#endif


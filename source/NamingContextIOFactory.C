// -*- c++ -*-
/*
 *  File name: NamingContextIOFactory.C
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

#include "NamingContextIOFactory.h"


TIDNaming::NamingContextIOFactory * 
TIDNaming::NamingContextIOFactory::m_singleton = NULL;

TIDNaming::NamingContextIOFactory & 
TIDNaming::NamingContextIOFactory::getFactory()
throw (NotInitializedException)
{
  if (m_singleton)
    return *m_singleton;
  
  throw NotInitializedException(
      "No naming context factory was initialized. Use "\
      "any NamingContextIOFactory child class to invoke "\
      "its init() member");
}


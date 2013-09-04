// -*- c++ -*-
/*
 *  File name: NamingContextNullIOFactory.C
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

#include "NamingContextNullIOFactory.h"

void 
TIDNaming::NamingContextNullIOFactory::init()
throw (AlreadyInitializedException)
{
  if (m_singleton)
    throw AlreadyInitializedException(
      "A naming context IO factory is already initialized");
  
  m_singleton = new NamingContextNullIOFactory();
}
    
TIDNaming::NamingContextReader &
TIDNaming::NamingContextNullIOFactory::getReader()
{
  return *m_reader;
}
    
TIDNaming::NamingContextWriter & 
TIDNaming::NamingContextNullIOFactory::getWriter()
{
  return *m_writer;
}

bool TIDNaming::NamingContextNullIOFactory::isPersistentFactory()
{
  return false;
}


TIDNaming::NamingContextNullIOFactory::NamingContextNullIOFactory()
{
 try
  {
    m_writer = new NamingContextNullWriter();
    m_reader = new NamingContextNullReader();
  }
  catch (NamingInternals::PersistenceException&)
  {
    
  }
}

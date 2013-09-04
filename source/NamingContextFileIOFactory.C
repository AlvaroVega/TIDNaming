// -*- c++ -*-
/*
 *  File name: NamingContextFileIOFactory.C
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
#include "NamingContextFileIOFactory.h"

void 
TIDNaming::NamingContextFileIOFactory::init(const char * path)
  throw (AlreadyInitializedException, InvalidPathException)
{
  if (m_singleton)
    throw AlreadyInitializedException(
      "A naming context IO factory is already initialized");
  
  m_singleton = new NamingContextFileIOFactory(path);
}
    
TIDNaming::NamingContextReader &
TIDNaming::NamingContextFileIOFactory::getReader()
{
  return *m_reader;
}
    
TIDNaming::NamingContextWriter & 
TIDNaming::NamingContextFileIOFactory::getWriter()
{
  return *m_writer;
}

bool TIDNaming::NamingContextFileIOFactory::isPersistentFactory()
{
  return true;
}


TIDNaming::NamingContextFileIOFactory::NamingContextFileIOFactory(
                                                          const char * path)
  throw (InvalidPathException)
{
  try
  {
    m_writer = new TIDNaming::NamingContextFileWriter(path);
    m_reader = new TIDNaming::NamingContextFileReader(path);
  }
  catch (NamingInternals::PersistenceException&)
  {
    throw InvalidPathException(path);
  }
}

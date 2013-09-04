// -*- c++ -*-
/*
 *  File name: NamingContextFileIOFactory.h
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

#ifndef _NAMING_CONTEXT_FILE_IO_FACTORY_H_
#define _NAMING_CONTEXT_FILE_IO_FACTORY_H_

#include "NamingContextIOFactory.h"
#include "NamingContextFileReader.h"
#include "NamingContextFileWriter.h"

namespace TIDNaming
{
  
  class NamingContextFileIOFactory : public NamingContextIOFactory
  {
  
    public:
    
      typedef struct InvalidPathException
      {
        const char * path;
        InvalidPathException(const char * _path) : path(_path) {}
      } InvalidPathException;
      
      virtual ~NamingContextFileIOFactory() {}

      static void init(const char * path)
          throw (AlreadyInitializedException,
                 InvalidPathException);
    
      virtual NamingContextReader & getReader();
    
      virtual NamingContextWriter & getWriter();

      virtual bool isPersistentFactory();
      
    private:
    
      NamingContextFileIOFactory(const char * path)
          throw (InvalidPathException);
      
      NamingContextReader * m_reader;
      NamingContextWriter * m_writer;
    
  };
  
}

#endif /*_NAMING_CONTEXT_FILE_IO_FACTORY_H_*/

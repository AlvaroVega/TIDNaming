// -*- c++ -*-
/*
 *  File name: NamingContextFileReader.h
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

#ifndef _NAMING_CONTEXT_FILE_READER_H_
#define _NAMING_CONTEXT_FILE_READER_H_

#include "NamingContextReader.h"

#include <vector>
#include <string>

using namespace std;

namespace TIDNaming
{

  class NamingContextFileReader : public NamingContextReader
  {
    
    public:
    
      NamingContextFileReader(const char * path);
      
      virtual NamingInternals::ContextInfo_ptr loadAll(
          NamingInternals::ContextInfoSeq_ptr ctxs)
            throw (NamingInternals::PersistenceException);
      
    private:
    
      void readDirectory(const char * path, vector<string> & dir_contents)
          throw (NamingInternals::PersistenceException);
    
      string m_path;
    
  };
  
}

#endif /*_NAMING_CONTEXT_FILE_READER_H_*/

// -*- c++ -*-
/*
 *  File name: NamingContextFileReader.C
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
#include "NamingContextFileReader.h"

// POSIX headers needed for persistence
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#include "TIDorb/util.h"

TIDNaming::NamingContextFileReader::NamingContextFileReader(
      const char * path
) : m_path(path)
{
}
      
NamingInternals::ContextInfo_ptr
TIDNaming::NamingContextFileReader::loadAll(
      NamingInternals::ContextInfoSeq_ptr ctxs)

throw (NamingInternals::PersistenceException)
{
  vector<string> dirnames;
  readDirectory(m_path.c_str(), dirnames);
  
  ctxs->length(dirnames.size());
  
  NamingInternals::ContextInfo_ptr root_ctx = NULL;
  for (size_t i = 0; i < dirnames.size(); i++)
  {
    const string & dname = dirnames[i].c_str(); // ctx name
    
    char subdname[1024];
    sprintf(subdname, "%s/%s", m_path.c_str(), dname.c_str());
    vector<string> filenames;
    readDirectory(subdname, filenames);
    
    NamingInternals::ReferenceSeq refs;
    int ref_counter = 0;
    bool valid_reference = false;
    for (size_t j = 0; j < filenames.size(); j++)
    {
      const string & fname = filenames[j];
      char id[1024], kind[1024];
      memset(id, 0, 1024);
      memset(kind, 0, 1024);
      string::size_type dot_pos = fname.find('.', 0);
      if (dot_pos == string::npos)
      {
        strcpy(id, fname.c_str());
        strcpy(kind, "\0");
      }
      else
      {
        strncpy(id, fname.c_str(), dot_pos);
        strcpy(kind, &(fname.c_str()[dot_pos + 1]));
      }
      
      CosNaming::NameComponent refname(id, kind); // ref name
      
      char fpath[1024];
      sprintf(fpath, "%s/%s", subdname, fname.c_str());
      FILE * fd;
      if (!(fd = fopen(fpath, "r")))
      {
        TIDorb::util::StringBuffer msg;
        msg << "cannot open reference file '" << fpath << "'";
        throw NamingInternals::PersistenceException(msg.str().data());
      }
      char str_type[1024]; // ref type
      char ref_ior[2048];  // ref ior
      
      fscanf(fd, "Type: %s\n", str_type);
      fscanf(fd, "%s\n", ref_ior);
      fclose(fd);
      
      CosNaming::BindingType ref_type;
      if (strcmp(str_type, "Object") == 0) {
        ref_type = CosNaming::nobject;
        valid_reference = true;
      }
      else if (strcmp(str_type, "Context") == 0) {
        ref_type = CosNaming::ncontext;
        valid_reference = true;
      }
      else
      {
        cerr << "Warning: unknown binding type in file '" <<
                fpath << "'" << endl;
        valid_reference = false;
      }
      
      if (valid_reference) {
        ref_counter++;
        refs.length(ref_counter);
        refs[ref_counter-1] = NamingInternals::Reference(refname, ref_type, ref_ior);
      }

    } 

    if (dname == NamingInternals::RootNamingContextOID)
      root_ctx = &((*ctxs)[i]);    

    (*ctxs)[i] = NamingInternals::ContextInfo(dname.c_str(), refs);
  }
  return root_ctx;
}

void 
TIDNaming::NamingContextFileReader::readDirectory(
      const char * path,
      vector<string> & dir_contents)
throw (NamingInternals::PersistenceException)
{
  DIR * dir;
  if (dir = opendir(path))
  {
    struct dirent * ctx_dir;
    
    dir_contents.clear();
    while (ctx_dir = readdir(dir)) {
      if (strcmp(ctx_dir->d_name, "." ) != 0 &&
          strcmp(ctx_dir->d_name, "..") != 0)
        dir_contents.push_back(ctx_dir->d_name);
    }
    closedir(dir);
  }
  else
  {
    TIDorb::util::StringBuffer msg;
    msg << "failed while opening persistence directory '" << path << "'";
    throw NamingInternals::PersistenceException(msg.str().data());
  }
}


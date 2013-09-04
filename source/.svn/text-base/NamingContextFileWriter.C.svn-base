// -*- c++ -*-
/*
 *  File name: NamingContextFileWriter.C
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

#include "NamingContextFileWriter.h"

// POSIX headers needed for persistence
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#include "TIDorb/util.h"

TIDNaming::NamingContextFileWriter::NamingContextFileWriter(
      const char * path) 
throw (NamingInternals::PersistenceException) 
  : m_path(path)
{
  // Create path if does not exists
  createDirectory(path);
}
    
void
TIDNaming::NamingContextFileWriter::store(
      CosNaming::NamingContext_ptr ctx)
throw (NamingInternals::PersistenceException)
{
  bool ext = false;

  NamingInternals::InternalNamingContext_var    ictx;
  NamingInternals::InternalNamingContextExt_var ictx_ext;
  
  ictx = NamingInternals::InternalNamingContext::_narrow(ctx);

  if (CORBA::is_nil(ictx))
  {
    ext = true;
    ictx_ext = NamingInternals::InternalNamingContextExt::_narrow(ctx);
        
    if (CORBA::is_nil(ictx_ext))
      throw NamingInternals::PersistenceException(
          "cannot cast NamingContext into InternalNamingContext");
  }

  char * ctx_name;      
  ctx_name = ext ? ictx_ext->getObjectId() : ictx->getObjectId();
     
  NamingInternals::ReferenceSeq_ptr refs_ptr;
  
  if (ext)
    ictx_ext->getModifiedReferences(refs_ptr);
  else
    ictx->getModifiedReferences(refs_ptr);
  
  store(ctx_name, *refs_ptr);

  delete ctx_name;
  delete refs_ptr;
}

void
TIDNaming::NamingContextFileWriter::store(
      const char * ctx_oid,
      NamingInternals::ReferenceSeq & refs)
throw (NamingInternals::PersistenceException)
{
  // Create context directory (if does not exists)
  char dir_name[1024];
  sprintf(dir_name, "%s/%s", m_path.c_str(), ctx_oid);
  createDirectory(dir_name);
  
  // Store each reference in a file
  for (size_t i = 0; i < refs.length(); i++)
  {
    NamingInternals::Reference & ref = refs[i];
    
    char file_name[1024];
    const char * id = ref.name.id.in();
    const char * kind = ref.name.kind.in();
    if (strcmp(kind, "") == 0)
      sprintf(file_name, "%s", id);
    else
      sprintf(file_name, "%s.%s", id, kind);
    
    char * type;
    switch (ref.type)
    {
      case CosNaming::nobject:        
        type = "Object";
        createFile(dir_name, file_name, type, ref.ior);
        break;
      case CosNaming::ncontext:       
        type = "Context";
        createFile(dir_name, file_name, type, ref.ior);
        break;
      default:
        type = "Unknown"; 
        break;
    }
  }
}

void
TIDNaming::NamingContextFileWriter::removeReference(
      const char * ctx_oid,
      const CosNaming::NameComponent & ref)
throw (NamingInternals::PersistenceException)
{
  char dir_name[1024];
  sprintf(dir_name, "%s/%s", m_path.c_str(), ctx_oid);
  
  char file_name[1024];
  const char * id = ref.id.in();
  const char * kind = ref.kind.in();
  if (strcmp(kind, "") == 0)
    sprintf(file_name, "%s", id);
  else
    sprintf(file_name, "%s.%s", id, kind);
  removeFile(dir_name, file_name);
}

void
TIDNaming::NamingContextFileWriter::removeContext(
      const char * ctx_oid)
throw (NamingInternals::PersistenceException)
{
  char dir_name[1024];
  sprintf(dir_name, "%s/%s", m_path.c_str(), ctx_oid);
  
  removeDirectory(dir_name);
}

void
TIDNaming::NamingContextFileWriter::createDirectory(const char * path)
throw (NamingInternals::PersistenceException)
{
  struct stat path_stat;
  
  if (stat(path, &path_stat))
  {
    if (mkdir(path, S_IRUSR | S_IWUSR | S_IXUSR)){
      TIDorb::util::StringBuffer msg;
      msg <<"cannot create persistence directory '" << path << "\'";
      throw NamingInternals::PersistenceException(msg.str().data());
    }
  }
  else 
    if (!S_ISDIR(path_stat.st_mode))
    {
      TIDorb::util::StringBuffer msg;
      msg <<"try to create a directory in a non-directory path '" << path << "\'";
      throw NamingInternals::PersistenceException(msg.str().data());
      return;
    }
}
void
TIDNaming::NamingContextFileWriter::createFile(
          const char * ctx_dir,
          const char * ref_file,
          const char * type,
          const char * ior)
throw (NamingInternals::PersistenceException)
{
  char fname[1024];
  sprintf(fname, "%s/%s", ctx_dir, ref_file);
  
  int fd;
  if ((fd = creat(fname, S_IRWXU)) == -1)
  {
    TIDorb::util::StringBuffer msg;
    msg <<"cannot remove reference file '" << fname << "'";
    throw NamingInternals::PersistenceException(msg.str().data());
  }
  
  // Write data
  char line[1024];
  sprintf(line, "Type: %s\n", type); 
  write(fd, line, strlen(line));
  write(fd, ior, strlen(ior));
  write(fd, "\n", 1); 
  
  close(fd);
}

void
TIDNaming::NamingContextFileWriter::removeFile(
          const char * ctx_dir,
          const char * ref_file)
throw (NamingInternals::PersistenceException)
{
  char fname[1024];
  sprintf(fname, "%s/%s", ctx_dir, ref_file);
  
  if (unlink(fname))
  {
    TIDorb::util::StringBuffer msg;
    msg <<"cannot remove reference file '" << fname << "'";
    throw NamingInternals::PersistenceException(msg.str().data());
  }
}

void
TIDNaming::NamingContextFileWriter::removeDirectory(
          const char * ctx_dir)
throw (NamingInternals::PersistenceException)
{
  if (rmdir(ctx_dir))
  {
    TIDorb::util::StringBuffer msg;
    msg << "cannot remove context directory '" << ctx_dir << "'";
    throw NamingInternals::PersistenceException(msg.str().data());
  }
}

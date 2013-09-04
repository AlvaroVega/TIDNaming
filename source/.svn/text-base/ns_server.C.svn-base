// -*- c++ -*-
/*
 *  File name: ns_server.C
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

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include <fstream>
#include <iostream>

#include "TIDNaming.h"
#include "NamingContextFileIOFactory.h"
#include "NamingContextNullIOFactory.h"

#include <SSLIOP.h>

using namespace std;


#ifdef DEBUG_ENABLED
  #define DMSG(s) cerr << s << endl
  #define DMSG_P(s1, s2, s3) cerr << s1 << s2 << s3 << endl
  #define DMSG_(s) cerr << s << flush
  #define DMSG_P_(s1, s2, s3) cerr << s1 << s2 << s3 << flush
#else
  #define DMSG(s)
  #define DMSG_P(s1, s2, s3)
  #define DMSG_(s)
  #define DMSG_P_(s1, s2, s3)
#endif

// This ORB instance is used globaly in whole service
CORBA::ORB_var my_global_orb;

void handler(int r)
{
  DMSG_("Stoping service... ");
  my_global_orb->shutdown(true);
  DMSG("Done");
  exit(1);
}

void
printWelcome()
{
  cerr << "TIDNaming, Telefonica I+D CORBA Naming Service" << endl;
  cerr << "Copyright (c) 1998-2009 Telefonica "\
          "Investigacion y Desarrollo S.A." << endl;
  cerr << "All rights reserved" << endl << endl;
}

void
printUsage(const char * bin_name)
{
  string bn(bin_name);
  string::size_type p = bn.find_last_of('/');
  string bf = (p == string::npos) ? bn : bn.substr(p + 1);
  
  cerr << "Usage: " << bf << " [options] [ORB args]" << endl << endl;
  cerr << "Options: " << endl;
  cerr << "   --help                         Print this help" << endl;
  cerr << "   --version                      Print version" << endl;
  cerr << "   --persistence-dir <path>       Set persistence "\
          "directory to <path>" << endl;
  cerr << "   --max_binding_iterators <max>  Set max number of "\
          "binding iterators supported" << endl;
  cerr << "                                  per context" << endl;
  cerr << "   --ior <IOR_file>               Print IOR Naming Service ";
  cerr << "in IOR_File" << endl;
  cerr << endl;
}

CosNaming::NamingContext_ptr
createNamingContextReference(
      PortableServer::POA_ptr poa,
      const char * oid)
{
  PortableServer::ObjectId_var id =
    PortableServer::string_to_ObjectId(oid);

  CORBA::Object_var obj = poa->create_reference_with_id(
        id,"IDL:omg.org/CosNaming/NamingContextExt:1.0");

  return CosNaming::NamingContext::_narrow(obj);
}

CosNaming::NamingContext_ptr
getNamingContextReference(
      PortableServer::POA_ptr poa,
      const char * oid)
{
  PortableServer::ObjectId_var id =
    PortableServer::string_to_ObjectId(oid);

  CORBA::Object_var obj = poa->id_to_reference(id);

  return CosNaming::NamingContext::_narrow(obj);
}

void
registerNameService(
      CORBA::ORB_ptr orb,
      CORBA::Object_ptr obj,
      const char* ior_file)
{
  TIDorb::core::TIDORB* m_orb = dynamic_cast<TIDorb::core::TIDORB*> (
        CORBA::ORB::_duplicate(orb));

  TIDorb::core::ORBServices& services = 
        const_cast<TIDorb::core::ORBServices&>(m_orb->services());

  services.set_service("NameService", CORBA::Object::_duplicate(obj));

  //////////////////////////////////////////////////////////////////
  if (strcmp(ior_file,"") != 0) {
    CORBA::String_var str = m_orb->object_to_string (obj); 
    int fd;
    if (!(fd = creat(ior_file, S_IRWXU)) == -1)
      {
        TIDorb::util::StringBuffer msg;
        msg << "cannot open reference file '" << ior_file << "'";
        throw NamingInternals::PersistenceException(msg.str().data());
      }
    write(fd, str, strlen(str));
    write(fd, "\n", 1); 
    close(fd);
  }
  //////////////////////////////////////////////////////////////////

}

void
loadPersistence(PortableServer::POA_ptr namingContextsPOA, 
                PortableServer::POA_ptr bindingIteratorsPOA,
                const char* ior_file) 
{
  TIDorb::core::TIDORB* m_orb = 
    dynamic_cast<TIDorb::core::TIDORB*> (CORBA::ORB::_duplicate(my_global_orb));
  
  
  TIDNaming::NamingContextIOFactory & iofac =
    TIDNaming::NamingContextIOFactory::getFactory();
  
  try {

    //
    // Read data persistence into ContextInfo sequence
    //
    NamingInternals::ContextInfoSeq_ptr ctxs = 
      new NamingInternals::ContextInfoSeq(0);
    ctxs->length(0);

    NamingInternals::ContextInfo_ptr root_ctx = 
      iofac.getReader().loadAll(ctxs);
    

    if (!ctxs->length()) {
      if (m_orb->trace != NULL)
        m_orb->print_trace(TIDorb::util::TR_DEBUG, 
                           "No naming context found to be recovered");
    } else {
      if (m_orb->trace != NULL)
        m_orb->print_trace(TIDorb::util::TR_DEBUG, 
                           "Preparing for persistent data recovery");
    }

    
    //
    // Create object references for each context read
    // Then the objects will added to Active Object Map
    for (CORBA::ULong i = 0; i < ctxs->length(); i++) {

      const NamingInternals::ContextInfo & ctx_inf = (*ctxs)[i];
      if (m_orb->trace != NULL){
        TIDorb::util::StringBuffer msg;
        msg << "Recovering naming context '" << ctx_inf.oid << "'... ";
        m_orb->print_trace(TIDorb::util::TR_USER, msg.str().data());
      }

      CosNaming::NamingContext_ptr nc = 
        createNamingContextReference(namingContextsPOA,
                                     ctx_inf.oid);     

      if (m_orb->trace != NULL)
        m_orb->print_trace(TIDorb::util::TR_DEBUG, "Done");

      if (strcmp(ctx_inf.oid, NamingInternals::RootNamingContextOID) == 0) {
        if (m_orb->trace != NULL){
          TIDorb::util::StringBuffer msg;
          msg << "Setting '" << ctx_inf.oid << "' as name server... ";
          m_orb->print_trace(TIDorb::util::TR_USER, msg.str().data());
        }
        registerNameService(my_global_orb, nc, ior_file);

        if (m_orb->trace != NULL)
          m_orb->print_trace(TIDorb::util::TR_DEBUG, "Done");

      } else {
        CORBA::ULong count;
        sscanf(ctx_inf.oid, "nc-%u.ctx", &count);
        TIDNaming::NamingContextImpl::setObjectIdCounter(count);
      }
      CORBA::release(nc);
    }
      
    //
    // Set binding information for each context read
    //  
    for (CORBA::ULong i = 0; i < ctxs->length(); i++) {
      const NamingInternals::ContextInfo & ctx_inf = (*ctxs)[i];
      
      CosNaming::NamingContext_ptr nc =
        getNamingContextReference(namingContextsPOA, ctx_inf.oid);
      
      
      for (CORBA::ULong j = 0; j < ctx_inf.refs.length(); j++) {

        const NamingInternals::Reference & ref = ctx_inf.refs[j];
        
        if (m_orb->trace != NULL){
          TIDorb::util::StringBuffer msg;
          msg << "Binding artifact '" << ref.name.id << ".";
          msg << ref.name.kind << "' for context '" << ctx_inf.oid;
          msg << "...";
          m_orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
        }

        CORBA::Object_ptr obj = my_global_orb->string_to_object(ref.ior);
        CosNaming::NamingContext_ptr obj_ctx;
        CosNaming::Name ref_name;
        ref_name.length(1);
        ref_name[0] = ref.name;

        switch (ref.type)  {
        case CosNaming::nobject:
          nc->bind(ref_name, obj);
          CORBA::release(obj);
          break;
        case CosNaming::ncontext:
          obj_ctx = CosNaming::NamingContext::_narrow(obj);
          nc->bind_context(ref_name, obj_ctx);
          CORBA::release(obj_ctx);
          CORBA::release(obj);
          break;
        default:
          if (m_orb->trace != NULL)
            m_orb->print_trace(TIDorb::util::TR_DEBUG, "Error: unknown binding type");
        }
        
        if (m_orb->trace != NULL)
          m_orb->print_trace(TIDorb::util::TR_DEBUG, "Done");
      }
      CORBA::release(nc);
    }
      
    if (ctxs->length()) {
      if (m_orb->trace != NULL)
        m_orb->print_trace(TIDorb::util::TR_DEBUG, 
                           "Persistent data loaded successfully");
    }
    
    
    //
    // Check to set root naming context
    // 
    if (!root_ctx) {
      
      if (m_orb->trace != NULL)
        m_orb->print_trace(TIDorb::util::TR_DEBUG, 
                           "No root context found. Initializing a new one... ");

      CosNaming::NamingContext_ptr root_nc = 
        createNamingContextReference(namingContextsPOA, 
                                     NamingInternals::RootNamingContextOID);
      
      NamingInternals::ReferenceSeq refs;
      TIDNaming::NamingContextIOFactory::getFactory().getWriter().store(
                                                 NamingInternals::RootNamingContextOID, refs);
      
      registerNameService(my_global_orb, root_nc, ior_file);
      
      CORBA::release(root_nc);
      
      if (m_orb->trace != NULL)
        m_orb->print_trace(TIDorb::util::TR_DEBUG, "Done");

    } 

    delete ctxs;

  } catch (NamingInternals::PersistenceException &) {
    if (m_orb->trace != NULL)
      m_orb->print_trace(TIDorb::util::TR_ERROR, "Error: cannot read persistence data");
    throw;
  }
}


int main(int argc, char* argv[]) {

//   // Signals staff
//   signal(SIGINT,  handler);
//   signal(SIGTERM, handler);
//   signal(SIGKILL, handler);
  
  // Print the welcome message
  printWelcome();
  
  //
  // Check TIDNaming arguments
  //
  /*
   * Note: this TIDNaming special arguments may be passed to ORB
   * initialization with no problems. Any unknown argument will be ignored
   * by ORB, so no change is needed for argv.
   */
  const char * persistence_dir = "";
  int max_binding_iterators = 40;
  const char * ior_file = "";

  for (int i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "--help") == 0)
    {
      printUsage(argv[0]);
      _exit(0);
    }

    if (strcmp(argv[i], "--version") == 0)
    {
      cerr << "TIDNaming C++ version " << TIDNaming::st_version << endl;
      _exit(0);
    }
    
    if (strcmp(argv[i], "--persistence-dir") == 0)
     {
       if (i == argc - 1)
       {
         cerr << "Error: --persistence-dir specified but no path given" << endl;
         printUsage(argv[0]);
         _exit(-1);
       }
       persistence_dir = argv[i+1];
     }

    if (strcmp(argv[i], "--max_binding_iterators") == 0) {
      char* ptr;
      long  laux;
      laux = strtol(argv[i+1], &ptr, 10);
      max_binding_iterators = laux;
    }

    if (strcmp(argv[i], "--ior") == 0) {
       if (i == argc - 1)
       {
         cerr << "Error: --ior specified but no file name given" << endl;
         printUsage(argv[0]);
         _exit(-1);
       }
       ior_file = argv[i+1];
    }

  }

  DMSG("Executing in debug mode, a lot of messages will be "\
       "printed to stderr");
  DMSG_P("Using '", persistence_dir, "' as persistence directory"); 
  DMSG_P("Using '", ior_file, "' as ior output file"); 

  char * port = strdup("2809");
  char * ssl_port = strdup("");
  bool port_given = false;
  bool ssl_port_given = false;
  bool ssl_config = false;

  try {
     
     //
     // Start TIDThread library
     //
     TIDThr::init();

     //
     // Check for -ORB_iiop_orb_port to set NameService default
     //
     int new_argc = argc + 2;
     char * new_argv[new_argc];
     for (int i = 1; i < argc; i++)
     {
       new_argv[i] = argv[i]; // No strdup is needed

       if (strcmp(argv[i], "-ORB_iiop_orb_port") == 0)
       {
         port_given = true;
         free(port);
         port = strdup(argv[i+1]);
       }

       if (strcmp(argv[i], "-ORB_ssl_port") == 0)
       {
         ssl_port_given = true; // util??
         free(ssl_port);
         ssl_port = strdup(argv[i+1]);
       }
       if (strcmp(argv[i], "-ORB_ssl_private_key") == 0)
       {
         for (int j = 1; j < argc; j++) {
           if (strcmp(argv[j], "-ORB_ssl_certificate") == 0) {
             ssl_config = true;
             break;
           }
         }
       }
     }
     new_argv[new_argc - 2] = "-ORB_iiop_orb_port";
     new_argv[new_argc - 1] = port;

     int orb_argc     = port_given ? argc : new_argc;
     char ** orb_argv = port_given ? argv : new_argv;

     //
     // ORB initialization
     //
     my_global_orb = CORBA::ORB_init(orb_argc, orb_argv);

     // Get internal ORB 
     TIDorb::core::TIDORB* m_orb = 
       dynamic_cast<TIDorb::core::TIDORB*> (CORBA::ORB::_duplicate(my_global_orb));

     //
     // Getting RootPOA & Manager references
     //
     CORBA::Object_var poa_obj = my_global_orb->resolve_initial_references("RootPOA");
     PortableServer::POA_ptr rootPOA = PortableServer::POA::_narrow(poa_obj);

     PortableServer::POAManager_var manager = rootPOA->the_POAManager();


     //
     // NamingContexts POA creation
     //
     CORBA::PolicyList policies;
     policies.length(4);

     policies[0] = rootPOA->create_servant_retention_policy(PortableServer::RETAIN);
     policies[1] = rootPOA->create_request_processing_policy(
                                                   PortableServer::USE_SERVANT_MANAGER);
     policies[2] = rootPOA->create_id_assignment_policy(PortableServer::USER_ID);
     policies[3] = rootPOA->create_lifespan_policy(PortableServer::PERSISTENT);

     PortableServer::POA_var namingContextsPOA = 
       rootPOA->create_POA("namingContextsPOA", manager, policies);

     PortableServer::POAManager_var namingContextsManager = 
       namingContextsPOA->the_POAManager();


     //
     // BindingIterators POA creation
     //
     CORBA::PolicyList bindingIterators_policies;
     bindingIterators_policies.length(2);

     bindingIterators_policies[0] = 
       rootPOA->create_thread_policy(PortableServer::SINGLE_THREAD_MODEL); 
     bindingIterators_policies[1] = 
       rootPOA->create_id_assignment_policy(PortableServer::USER_ID);

     PortableServer::POA_var bindingIteratorsPOA = 
       namingContextsPOA->create_POA("bindingIteratorsPOA", namingContextsManager, 
                                     bindingIterators_policies);


     // 
     // Creates and sets a new ServantManager
     // 

     TIDNaming::ServantManagerNSImpl* servant_manager = 
       new TIDNaming::ServantManagerNSImpl(my_global_orb, bindingIteratorsPOA,
                                           max_binding_iterators);

     namingContextsPOA->set_servant_manager(servant_manager);


     // Creates the reference to the "root.ctx". Dont create it
     // This fragment of code is not needed anymore. It creates the
     // initial reference to root naming context, now created from
     // PersistenceManager class.
     /*
     PortableServer::ObjectId_var oid =
       PortableServer::string_to_ObjectId("root.ctx"); // Change to "NameService"

     CORBA::Object_var obj = 
       namingContextsPOA->create_reference_with_id(oid,"IDL:omg.org/CosNaming/NamingContextExt:1.0");  
     */

     // Manager activation (ORB/POA staff)
     manager->activate();
     namingContextsManager->activate();


     //
     // Check if has persistence 
     //
     if (strcmp(persistence_dir, "") != 0) {

       if (m_orb->trace != NULL){
         TIDorb::util::StringBuffer msg;
         msg << "Using '" << persistence_dir << "' as persistence directory";
         m_orb->print_trace(TIDorb::util::TR_USER, msg.str().data());
       }

       //
       // Recover the persistent data 
       //
       try {
         TIDNaming::NamingContextFileIOFactory::init(persistence_dir);
       }
       catch (TIDNaming::NamingContextFileIOFactory::InvalidPathException & e) {
         if (m_orb->trace != NULL) {
           TIDorb::util::StringBuffer msg;
           msg << "Error: cannot initialize persistence directory '";
           msg << e.path << "'";
           m_orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
         }
         printUsage(argv[0]);
         _exit(-1);
       }

       loadPersistence(namingContextsPOA.inout(), 
                       bindingIteratorsPOA.inout(),
                       ior_file);


     } else {

       //
       // Start without persistent data 
       //     
       try {
         TIDNaming::NamingContextNullIOFactory::init();
       }
       catch (...) {
         printUsage(argv[0]);
         _exit(-1);
       }
       
       if (m_orb->trace != NULL)
         m_orb->print_trace(TIDorb::util::TR_USER, 
                            "No root context found. Initializing a new one... ");

       CosNaming::NamingContext_var root_nc = 
         createNamingContextReference(namingContextsPOA, 
                                      NamingInternals::RootNamingContextOID);

       // Get ssl system assigned port
       if (ssl_config && (!ssl_port_given)) {
         TIDorb::portable::Stub* stub_obj = 
           dynamic_cast<TIDorb::portable::Stub*> ((CORBA::Object*)root_nc);
         TIDorb::core::ObjectDelegateImpl* obj_delegate = 
           dynamic_cast<TIDorb::core::ObjectDelegateImpl*> (stub_obj->_get_delegate());
         TIDorb::core::iop::IOR* ior = obj_delegate->getReference();
         SSLIOP::SSL* _ssl = ior->get_SSL();
         free(ssl_port);
         ssl_port = (char*)malloc(6);
         sprintf(ssl_port, "%d", _ssl->port);         
       }
       registerNameService(my_global_orb, root_nc, ior_file);

       if (m_orb->trace != NULL)
         m_orb->print_trace(TIDorb::util::TR_DEBUG, "Done");
       
       
     }

     //
     // Run the ORB loop forever
     //
     TIDorb::util::StringBuffer msg;
     msg << "Serving at port " << port << "... " << endl;
     if (ssl_config) {
       msg.seekp(0, ios::beg);
       msg << "Serving at SSL port " << ssl_port << "... " << endl;
     }
     if (m_orb->trace != NULL)
       m_orb->print_trace(TIDorb::util::TR_USER,  msg.str().data());
     cerr << msg.str().data();

     my_global_orb->run();

     my_global_orb->destroy();

  }
  catch (const CORBA::INITIALIZE & ex) {
    cerr << "ERROR: cannot initialize Naming Service at port ";
    if (ssl_config) 
      cerr << ssl_port;
    else
      cerr << port;
    cerr << " due to: " << ex << endl;
  }
  catch (const CORBA::Exception& ex) {
    cerr << "ERROR: Naming Server Exception: " << ex << endl << flush;
    exit (1);
  }
  catch (...) {
    cerr << "ERROR: unexpected exception raised" << endl << flush;
    exit (1);
  }

  free(port);
  free(ssl_port);

}

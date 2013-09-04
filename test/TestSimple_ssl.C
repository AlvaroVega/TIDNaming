#ifdef _TAO
#include "EchoS.h"
#include "orbsvcs/Naming/Naming_Server.h"
#elif defined _MICO
#include "Echo.h"
#include "coss/CosNaming.h"
#elif defined _TIDORB
#include "CosNaming.h"
#include "POA_Echo.h"
#elif defined _OMNI
#include "Echo.hh"
#include "Naming.hh"
#elif defined _ORBACUS
#include <OB/CORBA.h> 
#include <OB/CosNaming.h> 
#include "Echo_skel.h"
#endif


#include <unistd.h>
#include <sys/time.h>

#include <string>
#include <iostream>




using namespace std;

class MyEchoTestImpl: virtual public POA_Echo::Test{
public:
  char* echo (const char* s) 
    throw (CORBA::SystemException)
  {
    return CORBA::string_dup(s);
  }
};



int main(int argc, char *argv[])
{

  //char pwd[256], uri[1000];
  
  try {

    CORBA::ORB_ptr orb = CORBA::ORB_init (argc, argv, "");
    
    /*
     * Obtain a reference to the RootPOA and its Manager
     */

    CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
    PortableServer::POAManager_var mgr = poa->the_POAManager();


    /*
     * Create and activate the servant
     */

    MyEchoTestImpl *servant   = new MyEchoTestImpl;

    PortableServer::ObjectId_var oid  = poa->activate_object (servant);

    CORBA::Object_var ref  = poa->id_to_reference (oid.in());



    /*
     * Obtain NamincContext reference ...
     */

    //    sprintf (uri, "file://%s/ns.ior", getcwd(pwd, 256));
   
    cerr << "Obtaining naming context reference... ";
    CORBA::Object_var naming_context_object;
    CosNaming::NamingContext_ptr naming_context;
    try
    {
       // For an URI given by args
       char * uri = argv[1];

       naming_context_object = 
         //orb->string_to_object(uri);
         orb->string_to_object("corbaloc:ssliop:1.2@localhost:2345/NameService");
       //orb->resolve_initial_references("NameService");

       naming_context =
          CosNaming::NamingContext::_narrow (naming_context_object.in ());
    }
    catch (...)
    {
       cerr << "Error: cannot obtain naming service initial reference" << endl;
       throw;
    }
    cerr << "Done" << endl;

    //
    // Some simple tests
    //


    CosNaming::NamingContext_var naming_context_trunk = NULL;
    CosNaming::NamingContext_var naming_context_branch = NULL;
    CosNaming::NamingContext_var naming_context_subbranch = NULL;

    CosNaming::Name name_trunk(1);
    name_trunk.length (1);
    name_trunk[0].id = CORBA::string_dup ("Trunk1");
    name_trunk[0].kind = CORBA::string_dup ("tronco");

    cerr << "[TestSimple] Binding a new context (Trunk1.tronco)... ";
    naming_context_trunk =  naming_context->bind_new_context(name_trunk);
    cerr << "OK" << endl;
    

    CosNaming::Name name_branch(1);
    name_branch.length (1);
    name_branch[0].id = CORBA::string_dup ("Branch2");
    name_branch[0].kind = CORBA::string_dup ("");

    CosNaming::Name name_subbranch(1);
    name_subbranch.length (1);
    name_subbranch[0].id = CORBA::string_dup ("SubBranch");
    name_subbranch[0].kind = CORBA::string_dup ("");


    /// OK. Trunk1.tronco/Branch2/MyEchoTest must be resolved by client
    cerr << "[TestSimple] Binding a new context (Branch2)... ";
    naming_context_branch =  naming_context_trunk->bind_new_context(name_branch);
    cerr << "OK" << endl;
    // ERROR: #Trunk1.tronco/Branch2/MyEchoTest must not be resolved: nc is binded 
    // as objet, not as nc
    cerr << "[TestSimple] Creating a new context... ";
    naming_context_subbranch = naming_context_trunk->new_context();
    cerr << "OK" << endl;
    cerr << "[TestSimple] Binding last created context (SubBranch)... ";
    naming_context_trunk->bind(name_subbranch, naming_context_subbranch);
    cerr << "OK" << endl;



    CosNaming::Name name(1);
    name.length (1);
    name[0].id = CORBA::string_dup ("MyEchoTest");
    name[0].kind = CORBA::string_dup ("");

    cerr << "[TestSimple] Rebinding (Branch2/MyEchoTest)...";
    naming_context_branch->rebind(name,ref);
    cerr << "OK" << endl;



    CosNaming::Name name_not(1);
    name_not.length (1);
    name_not[0].id = CORBA::string_dup ("MyEchoTest2");
    name_not[0].kind =CORBA::string_dup ("");

    cerr << "[TestSimple] Rebinding2 (SubBranch/MyEchoTest2)...";
    naming_context_subbranch->rebind(name_not,ref);
    cerr << "OK" << endl;




    CORBA::Object_var obj;
    try {
      CosNaming::Name name_search(3);
      name_search.length (3);
      name_search[0].id = CORBA::string_dup ("Trunk1");
      name_search[0].kind = CORBA::string_dup ("tronco");
      name_search[1].id = CORBA::string_dup ("Branch2");
      name_search[1].kind = CORBA::string_dup ("");
      name_search[2].id = CORBA::string_dup ("MyEchoTest");
      name_search[2].kind = CORBA::string_dup ("");

      cerr << "[TestSimple] Resolving (Trunk1.tronco/Branch2/MyEchoTest)...";
      obj = naming_context->resolve(name_search);
      cerr << "OK" << endl;
    } catch (...) {
      cerr << "ERROR" << endl;
    }

    CORBA::Object_var obj_not;
    try {
      CosNaming::Name name_search(3);
      name_search.length (3);
      name_search[0].id = CORBA::string_dup ("Trunk1");
      name_search[0].kind = CORBA::string_dup ("tronco");
      name_search[1].id = CORBA::string_dup ("SubBranch");
      name_search[1].kind = CORBA::string_dup ("");
      name_search[2].id = CORBA::string_dup ("MyEchoTest2");
      name_search[2].kind = CORBA::string_dup ("");
      cerr << "[TestSimple] Resolving (Trunk1.tronco/SubBranch/MyEchoTest2)...";
      obj_not = naming_context->resolve(name_search);
      cerr << "ERROR" << endl;
    } catch (const CosNaming::NamingContext::NotFound& ex) {
      cerr << "OK: NotFound" << endl;
    } catch (const CosNaming::NamingContext::CannotProceed& ex) {
      cerr << "OK: CannotProceed" << endl;
    } catch (const CosNaming::NamingContext::InvalidName& ex) {
      cerr << "OK: InvalidName" << endl;
    } catch (...) {
      cerr << "ERROR" << endl;
    }


    int how_many = 1;
    CosNaming::BindingList* bl;
    CosNaming::BindingIterator_var bi;
    CosNaming::Binding* my_binding2 = NULL;
    cerr << "[TestSimple] Listing (Trunk1.tronco)...";
    naming_context_trunk->list(how_many, bl, bi);
    bi->next_one(my_binding2);
    bi->next_n(2, bl);
    delete my_binding2;
    bi->next_one(my_binding2);
    //bi->destroy();
    cerr << "OK" << endl;


    



    cerr << "[TestSimple] Obtaining naming context ext reference... ";
    CosNaming::NamingContextExt_ptr naming_context_ext;
    try {
      naming_context_ext = 
        CosNaming::NamingContextExt::_narrow (naming_context_object.in());
    } catch (...) {
      cerr << "Error: cannot obtain naming service initial reference" << endl;
      throw;
    }
    cerr << "OK" << endl;

    
    try {
      //
      // Test Extensions of NamingContext...
      //

      CosNaming::Name name_search(3);
      name_search.length (3);
      name_search[0].id = CORBA::string_dup ("Trunk1");
      name_search[0].kind = CORBA::string_dup ("tronco");
      name_search[1].id = CORBA::string_dup ("Branch2");
      name_search[1].kind = CORBA::string_dup ("");
      name_search[2].id = CORBA::string_dup ("MyEchoTest");
      name_search[2].kind = CORBA::string_dup ("");

      CORBA::String_var name_search_string;
      try {
        cerr << "[TestSimple] to_string... ";
        name_search_string = naming_context_ext->to_string(name_search);
        cerr << name_search_string << " ";
        cerr << "OK" << endl;
      } catch (...) {
        cerr << "ERROR" << endl;
      }
      
      CosNaming::Name_var name;
      try {
        cerr << "[TestSimple] to_name... " << endl;
        CORBA::String_var name_string = CORBA::string_dup ("Trunk1.tronco/a.b");
        name = naming_context_ext->to_name(name_string);
        for (CORBA::ULong i = 0; i < name->length(); i++) {
          cerr << "     name[" << i << "].id   = " << (*name)[i].id   << endl;
          cerr << "     name[" << i << "].kind = " << (*name)[i].kind << endl; 
        }
        cerr << "...OK" << endl;
      } catch (...) {
        cerr << "ERROR" << endl;
      }

      CORBA::String_var url;
      try {
        cerr << "[TestSimple] to_url... " << endl;
        CORBA::String_var addr = CORBA::string_dup ("localhost:2809");
        CORBA::String_var sn   = CORBA::string_dup ("Trunk1.tronco/Branch2/MyEchoTest");
        url = naming_context_ext->to_url(addr, sn);
        cerr << "       " << url << " ";
        cerr << "...OK" << endl;
      } catch (...) {
        cerr << "ERROR" << endl;
      }

      
      CORBA::Object_var obj_ref;
      try {  
        cerr << "[TestSimple] resolve_str... ";
        obj_ref = naming_context_ext->resolve_str("Trunk1.tronco/Branch2/MyEchoTest");
        cerr << "OK" << endl;
      } catch (...) {
        cerr << "ERROR" << endl;
      }

      try {  
        cerr << "[TestSimple] resolve_str... ";
        obj_ref = naming_context_ext->resolve_str("Trunk1.tronco/Branch2/MyEchoTest2");
        cerr << "ERROR" << endl;
      } catch (...) {
        cerr << "catch Exception OK" << endl;
      }


    } catch (...) {
      cerr << "ERROR" << endl;
    }
        
    cerr << "Waiting 3 seconds for destroy all..." << endl;
    sleep(3);

    //
    // Unbind and destroy all staff
    //

    cerr << "[TestSimple] Unbinding (Branch2)...";
    naming_context_branch->unbind(name);
    cerr << "OK" << endl;

    cerr << "[TestSimple] Destroying (Branch2)...";
    naming_context_branch->destroy();
    cerr << "OK" << endl;

    try {
      cerr << "[TestSimple] Destroying2 (Trunk1.tronco)..." << endl;
      naming_context_trunk->destroy();
      cerr << "ERROR" << endl;
    } catch(const CosNaming::NamingContext::NotEmpty& ex) {
      cerr << "[TestSimple] catch CosNaming::NamingContext::NotEmpty OK" << endl;
    }


    cerr << "[TestSimple] Unbinding (SubBranch)...";
    naming_context_subbranch->unbind(name_not);
    cerr << "OK" << endl;

    cerr << "[TestSimple] Destroying (SubBranch)...";
    naming_context_subbranch->destroy();
    cerr << "OK" << endl;


    cerr << "[TestSimple] Unbinding (Trunk1.tronco)...";
    naming_context_trunk->unbind(name_branch);
    cerr << "OK" << endl;

    cerr << "[TestSimple] Unbinding (Trunk1.tronco)...";
    naming_context_trunk->unbind(name_subbranch);
    cerr << "OK" << endl;

    cerr << "[TestSimple] Destroying (Trunk1.tronco)...";
    naming_context_trunk->destroy();
    cerr << "OK" << endl;
    
    cerr << "[TestSimple] Unbinding (root.ctx)...";
    naming_context->unbind(name_trunk);
    cerr << "OK" << endl;

    cerr << "[TestSimple] Destroying (root.ctx)...";
    naming_context->destroy();
    cerr << "OK" << endl;



    orb->destroy();

  } catch (const CORBA::SystemException& ex){
    cerr << "ERROR, excepcion raised: " << ex._name() << " "; // << ex.what() << " ";
    cerr << ex.completed() << endl;
    exit (1);
  }

  cout << "[TestSimple] ...Terminado" << endl;
  return 0;
}

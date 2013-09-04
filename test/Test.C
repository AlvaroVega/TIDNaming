#ifdef _TAO
#include "orbsvcs/Naming/Naming_Server.h"
#elif defined _MICO
#include "coss/CosNaming.h"
#elif defined _TIDORB
#include "CosNaming.h"
#elif defined _TIDORBV4IM

#elif defined _OMNI
#include "Naming.hh"
#elif defined _ORBACUS
#include <OB/CORBA.h> 
#include <OB/CosNaming.h> 
#endif


#include <unistd.h>
#include <sys/time.h>

#include <string>
#include <iostream>
#include <sstream>

#include "TestBind.h"
#include "TestBindContext.h"
#include "TestBindNewContext.h"
#include "TestUnbind.h"
#include "TestResolveContext.h"
#include "TestNewContext.h"
#include "TestDestroy.h"
#include "TestList.h"


using namespace std;




class NamingTestThread: public TIDThr::Thread {
private:
  char*                m_id;
  CORBA::Boolean       m_print;
  int                  m_argc;
  char**               m_argv;
  CosNaming::Name_ptr* m_names;
  int                  m_binding_loop;
  CORBA::ORB_ptr       m_orb;

public:
  NamingTestThread(int argc, char *argv[], int binding_loop, 
                   const char *id, CORBA::Boolean print)
  {
    cerr << "[NamingTestThread] NamingTestThread()" << endl;
    m_argc = argc;
    m_argv = argv;
    m_binding_loop = binding_loop;
    m_id = strdup(id);
    m_print = print;
    m_names = generate_names(id);
  }

  ~NamingTestThread() throw (TIDThr::SystemException) {
    cerr << "[NamingTestThread] ~NamingTestThread()" << endl;
    free(m_id);

    m_orb->shutdown(false);
    m_orb->destroy();

  }

  static CORBA::ORB_ptr prepare_orb(int argc, char *argv[], 
                                    CosNaming::NamingContext_ptr & nc);

  static CosNaming::Name_ptr* generate_names(const char* prep);

  static void test_bind(CosNaming::NamingContext_ptr root_context, int binding_size, 
                        CosNaming::Name_ptr* names, const char* id, CORBA::Boolean print);

  static void test_unbind(CosNaming::NamingContext_ptr root_context, int binding_size, 
                        CosNaming::Name_ptr* names, const char* id, CORBA::Boolean print);

  static void test_bind_context(CosNaming::NamingContext_ptr root_context, 
                                CosNaming::Name_ptr* names, const char* id, 
                                CORBA::Boolean print);

  static void test_unbind_context(CosNaming::NamingContext_ptr root_context, 
                                CosNaming::Name_ptr* names, const char* id, 
                                CORBA::Boolean print);

  static void test_list(CosNaming::NamingContext_ptr root_context, 
                        CosNaming::Name_ptr* names, const char* id, 
                        CORBA::Boolean print);


  void run(){
    // TIDThr::Synchronized synchro (*this);
    cerr << "[NamingTestThread] run()" << endl;

    CosNaming::NamingContext_var root_context;

    try {
      m_orb = prepare_orb(m_argc, m_argv, root_context);
    }
    catch (...)
    {
      cerr << "Error: something was wrong in ORB prepared" << endl;
    }

    CosNaming::NamingContext_ptr context;
    TestBindNewContext::test(root_context, m_names[1], context, NULL, m_id, m_print);
    
    test_bind(root_context, m_binding_loop, m_names, m_id, m_print);
    test_bind_context(root_context, m_names, m_id, m_print);
    test_list(root_context, m_names, m_id, m_print);
    test_unbind(root_context, m_binding_loop, m_names, m_id, m_print);
    test_unbind_context(root_context, m_names, m_id, m_print);
     
  } // run()

};





int main(int argc, char *argv[])
{

  
  try {
    
    TIDThr::init();
    
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    TIDThr::ThreadGroupHandle group = new TIDThr::ThreadGroup(NULL, "", &attr);
    TIDThr::ThreadHandle thread;
    
    int threads = 20;
    cerr << "[Test] Starting " << threads << " threads..." << endl;

    for(int i = 0; i < threads; i++) {
      ostringstream id;
      id << "paralel_" << i;
      thread = (TIDThr::Thread *) new NamingTestThread(argc, argv, 5, 
                                                       id.str().data(), true);
      thread->start();
    }
    
    // This line is needed to avoid main program
    // exit before child threads are finished
    sleep(30);


    // yield
    
    cout << "[Test] Terminando..." << endl;

  } catch (const CORBA::SystemException& ex){
    cerr << "[Test] Excepcion: " << ex._name() << " " << ex.what() << " ";
    cerr << ex.completed() << endl;
    exit (1);
  }

  cout << "[Test] ...Terminado" << endl;
  return 0;
}










//
// Static members definition
//


CORBA::ORB_ptr NamingTestThread::prepare_orb(int argc, char *argv[], 
                                             CosNaming::NamingContext_ptr & nc)
{
  //char pwd[256];//, uri[1000];
  // char * uri = argv[1];
  char * uri = "corbaloc:iiop:1.2@127.0.0.1:2809/NameService";

  CORBA::ORB_ptr orb = CORBA::ORB_init (argc, argv, "");

  try {
    CORBA::Object_var naming_context_object = orb->string_to_object(uri);
    if (naming_context_object == NULL) {
      cerr << "Error: NS fetch failed!!!" << endl;
      return NULL;
    }
    
    nc = CosNaming::NamingContext::_narrow (naming_context_object.in ());
    
    if (nc == NULL) {
      cerr << "Error: null narrowed NS object" << endl;
      return NULL;
    }
    
    return orb;
  }
  catch (...) {
    cerr << "Error: cannot obtain NS object reference" << endl;
    return NULL;
  }  
}


CosNaming::Name_ptr* NamingTestThread::generate_names(const char* prep)
{

  CosNaming::Name_ptr* names = new CosNaming::Name_ptr[7];

  string str_aux;

  names[0] = new CosNaming::Name(1);
  names[0]->length(1);
  str_aux = prep;
  str_aux += "_hola";
  (*(names[0]))[0].id   = CORBA::string_dup (str_aux.c_str());
  (*(names[0]))[0].kind = CORBA::string_dup ("");

  names[1] = new CosNaming::Name(1);
  names[1]->length(1);
  str_aux = prep;
  str_aux += "_subcontext";
  (*(names[1]))[0].id   = CORBA::string_dup (str_aux.c_str());
  (*(names[1]))[0].kind = CORBA::string_dup ("");

  names[2] = new CosNaming::Name(1);
  names[2]->length(1);
  str_aux = prep;
  str_aux += "_child";
  (*(names[2]))[0].id   = CORBA::string_dup (str_aux.c_str());
  (*(names[2]))[0].kind = CORBA::string_dup ("");

  names[3] = new CosNaming::Name(2);
  names[3]->length(2);
  str_aux = prep;
  str_aux += "_subcontext";
  (*(names[3]))[0].id   = CORBA::string_dup (str_aux.c_str());
  (*(names[3]))[0].kind = CORBA::string_dup ("");
  str_aux = prep;
  str_aux += "_child";
  (*(names[3]))[1].id   = CORBA::string_dup (str_aux.c_str());
  (*(names[3]))[1].kind = CORBA::string_dup ("");

  names[4] = new CosNaming::Name(2);
  names[4]->length(2);
  str_aux = prep;
  str_aux += "_subcontext";
  (*(names[4]))[0].id   = CORBA::string_dup (str_aux.c_str());
  (*(names[4]))[0].kind = CORBA::string_dup ("");
  str_aux = prep;
  str_aux += "_hola";
  (*(names[4]))[1].id   = CORBA::string_dup (str_aux.c_str());
  (*(names[4]))[1].kind = CORBA::string_dup ("adios");

  names[5] = new CosNaming::Name(1);
  names[5]->length(1);
  str_aux = prep;
  str_aux += "_inexistent";
  (*(names[5]))[0].id   = CORBA::string_dup (str_aux.c_str());
  (*(names[5]))[0].kind = CORBA::string_dup ("");

  names[6] = new CosNaming::Name(2);
  names[6]->length(2);
  str_aux = prep;
  str_aux += "_inexistent";
  (*(names[6]))[0].id   = CORBA::string_dup (str_aux.c_str());
  (*(names[6]))[0].kind = CORBA::string_dup ("");
  str_aux = prep;
  str_aux += "_hola";
  (*(names[6]))[1].id   = CORBA::string_dup (str_aux.c_str());
  (*(names[6]))[1].kind = CORBA::string_dup ("adios");

  return names;
}


void NamingTestThread::test_bind(CosNaming::NamingContext_ptr root_context, 
                                 int binding_size, CosNaming::Name_ptr* names, 
                                 const char* id, CORBA::Boolean print)
{
  CORBA::Object_ptr obj = NULL;

  cerr << "(" << id << ") TestBind ... " << endl;
  
  TestBind::test(root_context, names[0], NULL, NULL, id, print);
  //expected error
  TestBind::test(root_context, names[0], NULL, 
                 new CosNaming::NamingContext::AlreadyBound(), id, print);
  //expected error  
  TestBind::test(root_context, names[4], NULL, 
                 new CosNaming::NamingContext::NotFound(), id, print);
  
  TestResolveContext::test(root_context, names[1], obj, NULL, id, print);
  

  if(obj != NULL) {
    try {
      CosNaming::NamingContext_ptr context = 
        CosNaming::NamingContext::_narrow(obj);
      TestBind::test_loop(context, binding_size, NULL, id, print);
    } catch (...) {
      cerr << "(" << id << ") TestBind ERROR " << endl;
    }
  }

}


void NamingTestThread::test_unbind(CosNaming::NamingContext_ptr root_context, 
                                   int binding_size, CosNaming::Name_ptr* names, 
                                   const char* id, CORBA::Boolean print)
{
  CORBA::Object_ptr obj = NULL;
  
  cerr << "(" << id << ") TestUnbind ... " << endl;
  
  TestResolveContext::test(root_context, names[1], obj, NULL, id, print);
  
  if(obj != NULL) {
    CosNaming::NamingContext_ptr context = 
      CosNaming::NamingContext::_narrow(obj);
    TestUnbind::test_loop(context, binding_size, NULL, id, print);
  }
  
  TestUnbind::test(root_context, names[0], NULL, id, print);
  TestUnbind::test(root_context, names[4], NULL, id, print);
}


void NamingTestThread::test_bind_context(CosNaming::NamingContext_ptr root_context, 
                                         CosNaming::Name_ptr* names, const char* id, 
                                         CORBA::Boolean print)
{
  CORBA::Object_ptr obj = NULL;
  
  cerr << "(" << id << ") TestBindContext ... " << endl;
  
  TestNewContext::test(root_context, obj, NULL, id, print);
  
  CosNaming::NamingContext_ptr context;
  
  if(obj != NULL) {
    context = CosNaming::NamingContext::_narrow(obj);
    TestBindContext::test(root_context, names[1], context, 
                          new CosNaming::NamingContext::AlreadyBound(), 
                          id, print);
    TestDestroy::test(context, NULL, id, print);
  }
  
  TestBindNewContext::test(root_context, names[2], context, NULL, id, print);
  TestBindNewContext::test(root_context, names[3], context, NULL, id, print);
  //expected error
  TestBindNewContext::test(root_context, names[3], context, 
                           new CosNaming::NamingContext::AlreadyBound(), id, print);
  //expected error
  TestBindNewContext::test(root_context, names[6], context, 
                           new CosNaming::NamingContext::NotFound(), id, print);
}


void NamingTestThread::test_unbind_context(CosNaming::NamingContext_ptr root_context, 
                                           CosNaming::Name_ptr* names, const char* id, 
                                           CORBA::Boolean print)
{

  CORBA::Object_ptr obj = NULL;

  cerr << "(" << id << ") TestUnbindContext ... " << endl;

  TestResolveContext::test(root_context, names[1], obj, NULL, id, print);

  CosNaming::NamingContext_ptr context;

  if(context != NULL) {
    context = CosNaming::NamingContext::_narrow(obj);
    TestDestroy::test(context, 
                      new CosNaming::NamingContext::NotEmpty(), id, print);
  }
    
  TestResolveContext::test(root_context, names[3], obj, NULL, id, print);
  if(context != NULL) {

    TestUnbind::test(root_context, names[3], NULL, id, print);
    TestDestroy::test(context, NULL, id, print);
    //expected error
    TestUnbind::test(root_context, names[3], 
                     new CosNaming::NamingContext::NotFound(), id, print);
    // expected error
    TestDestroy::test(context, new CORBA::OBJECT_NOT_EXIST(), id, print);
    }
    
    TestResolveContext::test(root_context, names[2], obj, NULL, id, print);
   
    if(obj != NULL) {
      context = CosNaming::NamingContext::_narrow(obj);
      TestUnbind::test(root_context, names[2], NULL, id, print);
      TestDestroy::test(context, NULL, id,print);
    }

    TestResolveContext::test(root_context, names[1], obj, NULL, id, print);
   
    if(obj != NULL) {
      context = CosNaming::NamingContext::_narrow(obj);
      TestUnbind::test(root_context, names[1], NULL, id, print);
      TestDestroy::test(context, NULL, id,print);
    }
    
    //expected error
    TestUnbind::test(root_context, names[3],  
                     new CosNaming::NamingContext::NotFound(), id, print);



}


void NamingTestThread::test_list(CosNaming::NamingContext_ptr root_context, 
                                 CosNaming::Name_ptr* names, const char* id, 
                                 CORBA::Boolean print)
{
  CORBA::Object_ptr obj = NULL;
  
  cerr << "(" << id << ") TestList ... " << endl;
  
  TestList::test(root_context, 23, NULL, id, print);

  TestResolveContext::test(root_context, names[1], obj, NULL, id, print);

  if(obj != NULL) {
    CosNaming::NamingContext_ptr context = 
      CosNaming::NamingContext::_narrow(obj);
    TestList::test(context, 23, NULL, id, print);
  }
}

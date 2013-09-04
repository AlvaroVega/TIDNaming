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


using namespace std;


class TestResolveContext {
public:
  static CORBA::Boolean test(CosNaming::NamingContext_ptr context, 
                             CosNaming::Name_ptr name,
                             CORBA::Object_ptr& obj,
                             CORBA::Exception* expected_exception,
                             const char* id, CORBA::Boolean print);
  
  static CORBA::Boolean test1(CosNaming::NamingContext_ptr context,
                              CORBA::Exception* expected_exception,
                              const char* id, CORBA::Boolean print);

  static CORBA::Boolean test2(CosNaming::NamingContext_ptr context,
                              CORBA::Exception* expected_exception,
                              const char* id, CORBA::Boolean print);

};

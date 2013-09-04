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

using namespace std;


class NMUtil {
public:

  static const char * print(CosNaming::Name_ptr name); 

  static const char * print(const CosNaming::NamingContext::NotFound & nf);

  static const char * print(const CosNaming::NamingContext::CannotProceed & cp);
  
  static const char * print(const CosNaming::NamingContext::AlreadyBound & ab);
  


};

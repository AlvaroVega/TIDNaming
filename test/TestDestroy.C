#include "TestDestroy.h"
#include "NMUtil.h"


CORBA::Boolean TestDestroy::test(CosNaming::NamingContext_ptr context, 
                                 CORBA::Exception* expected_exception,
                                 const char* id, CORBA::Boolean print)
{
  if (print)
    cerr << "(" << id << ") TestDestroy " << endl; 
  
  if (context == NULL) {
    cerr << "Error: given context is null" << endl;
    return false;
  }
  
  try {
    
    context->destroy(); 
      
    
  } catch (const CORBA::Exception & e) {
    // TODO: check if cp and expected_exception RTTI types matches
  }

  if(print) {
    cerr << "(" << id << ") TestDestroy: OK!!" << endl;
  }
    
  return true;
}
  

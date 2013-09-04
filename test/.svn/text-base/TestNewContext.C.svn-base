#include "TestNewContext.h"
#include "NMUtil.h"


CORBA::Boolean TestNewContext::test(CosNaming::NamingContext_ptr context, 
                                    CORBA::Object_ptr obj,
                                    CORBA::Exception* expected_exception,
                                    const char* id, CORBA::Boolean print)
{
  if (print)
    cerr << "(" << id << ") TestNewContext " << endl; 
  
  if (context == NULL) {
    cerr << "Error: given context is null" << endl;
    return false;
  }
  
  try {
    
    obj = context->new_context();
    
  } catch (const CORBA::Exception & e) {
    // TODO: check if cp and expected_exception RTTI types matches
  }
  
  if(print)
    cerr << "(" << id << ") TestNewContext: OK!!" << endl;
    
  return true;
}
  

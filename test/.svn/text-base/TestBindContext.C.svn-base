#include "TestBindContext.h"
#include "NMUtil.h"


CORBA::Boolean TestBindContext::test(CosNaming::NamingContext_ptr context, 
                                     CosNaming::Name_ptr name,
                                     CosNaming::NamingContext_ptr new_context, 
                                     CORBA::Exception* expected_exception,
                                     const char* id, CORBA::Boolean print)
{
  if (print)
    cerr << "(" << id << ") TestBindContext " << NMUtil::print(name) << endl; 
  
  if (context == NULL) {
    cerr << "Error: given context is null" << endl;
    return false;
  }
  
  try {
    
    context->bind_context(*name, context); 
      
  } catch (const CosNaming::NamingContext::NotFound & nf) {
    // TODO: check if nf and expected_exception RTTI types matches
    if(print) {
        cerr << "(" << id << ") TestBindContext: expected Exception" << 
                NMUtil::print(nf) << endl;
    }
    return true;
    
  } catch (const CosNaming::NamingContext::AlreadyBound & ab) {
    // TODO: check if ab and expected_exception RTTI types matches
    if(print) {
        cerr << "(" << id << ") TestBindContext: expected Exception" << 
                NMUtil::print(ab) << endl;
    }
    return true;
    
  } catch (const CosNaming::NamingContext::CannotProceed & cp) {
    // TODO: check if cp and expected_exception RTTI types matches
    if(print) {
        cerr << "(" << id << ") TestBindContext: expected Exception" << 
                NMUtil::print(cp) << endl;
    }
    return true;
  } catch (const CORBA::Exception & e) {
    // TODO: check if cp and expected_exception RTTI types matches
  }

  if(print) {
    cerr << "(" << id << ") TestBindContext: OK!!" << endl;
  }
    
  return true;
}
  

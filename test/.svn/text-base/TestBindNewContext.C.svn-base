#include "TestBindNewContext.h"
#include "NMUtil.h"


CORBA::Boolean TestBindNewContext::test(CosNaming::NamingContext_ptr context, 
                                        CosNaming::Name_ptr name,
                                        CosNaming::NamingContext_ptr& obj,
                                        CORBA::Exception* expected_exception,
                                        const char* id, CORBA::Boolean print)
{
  if (print)
    cerr << "(" << id << ") TestBindNewContext " << NMUtil::print(name) << endl; 
  
  if (context == NULL) {
    cerr << "Error: given context is null" << endl;
    return false;
  }
  
  try {
    
    obj = context->bind_new_context(*name);
      
  } catch (const CosNaming::NamingContext::NotFound & nf) {
    // TODO: check if nf and expected_exception RTTI types matches
    if(print) {
        cerr << "(" << id << ") TestBindNewContext: expected Exception" << 
                NMUtil::print(nf) << endl;
    }
    return true;
    
  } catch (const CosNaming::NamingContext::AlreadyBound & ab) {
    // TODO: check if ab and expected_exception RTTI types matches
    if(print) {
        cerr << "(" << id << ") TestBindNewContext: expected Exception" << 
                NMUtil::print(ab) << endl;
    }
    return true;
    
  } catch (const CosNaming::NamingContext::CannotProceed & cp) {
    // TODO: check if cp and expected_exception RTTI types matches
    if(print) {
        cerr << "(" << id << ") TestBindNewContext: expected Exception" << 
                NMUtil::print(cp) << endl;
    }
    return true;
  } catch (const CORBA::Exception & e) {
    // TODO: check if cp and expected_exception RTTI types matches
  }

  if(print) {
    cerr << "(" << id << ") TestBindNewContext: OK!!" << endl;
  }
    
  return true;
}
  

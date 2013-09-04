#include "TestBind.h"
#include "NMUtil.h"
#include <typeinfo>

CORBA::Boolean TestBind::test(CosNaming::NamingContext_ptr context, 
                              CosNaming::Name_ptr name,
                              CORBA::Object_ptr obj,
                              CORBA::Exception* expected_exception,
                              const char* id, CORBA::Boolean print)
{
  if (print)
    cerr << "(" << id << ") TestBind " << NMUtil::print(name) << endl; 
  
  if (context == NULL) {
    cerr << "Error: given context is null" << endl;
    return false;
  }
  
  try {
    
    context->bind(*name, obj); 
      
  } catch (const CosNaming::NamingContext::NotFound & nf) {
    // TODO: check if nf and expected_exception RTTI types matches
    if(print) {
      cerr << "(" << id << ") TestBind: expected Exception" << 
        NMUtil::print(nf) << endl;
    }
    return true;
    
  } catch (const CosNaming::NamingContext::AlreadyBound & ab) {
    // TODO: check if ab and expected_exception RTTI types matches
    if(print) {
      cerr << "(" << id << ") TestBind: expected Exception" << 
        NMUtil::print(ab) << endl;
    }
    return true;
    
  } catch (const CosNaming::NamingContext::CannotProceed & cp) {
    // TODO: check if cp and expected_exception RTTI types matches
    if(print) {
        cerr << "(" << id << ") TestBind: expected Exception" << 
                NMUtil::print(cp) << endl;
    }
    return true;
  } catch (const CORBA::Exception & e) {
    // TODO: check if cp and expected_exception RTTI types matches
  }

  if(print) {
    cerr << "(" << id << ") TestBind: OK!!" << endl;
  }
    
  return true;
}
  
CORBA::Boolean TestBind::test_loop(CosNaming::NamingContext_ptr context,
                                   int iterations,
                                   CORBA::Exception* expected_exception,
                                   const char* id, CORBA::Boolean print)
{
  CORBA::Boolean ok = true;
  ostringstream name_id;
  name_id << id << "_hola";

  CosNaming::Name name(1);
  name.length (1);
  name[0].id   = CORBA::string_dup (name_id.str().data());
  name[0].kind = CORBA::string_dup ("");
  
  //CORBA::Long  start = System.currentTimeMillis();                      
  cerr << "Init binding loop: " << endl;
  
  for(int i = 0 ; i< iterations; i++) {
    
    ostringstream name_kind;
    name_kind << i;
    name[0].kind = CORBA::string_dup(name_id.str().data());
    
    if(!test(context, &name, NULL, expected_exception, id, print))
      ok = false;
  }
  
  cerr << "Binding average time: "  << endl; 
  //+((System.currentTimeMillis() - start)/iterations));
  
  return ok;
  


}

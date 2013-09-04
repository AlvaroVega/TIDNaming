#include "TestResolveContext.h"
#include "NMUtil.h"



CORBA::Boolean TestResolveContext::test(CosNaming::NamingContext_ptr context, 
                             CosNaming::Name_ptr name,
                             CORBA::Object_ptr& obj,
                             CORBA::Exception* expected_exception,
                             const char* id, CORBA::Boolean print)
{

  if (print)
    cerr << "(" << id << ") TestResolveContext " << NMUtil::print(name) << endl; 
  

  if (context == NULL) {
    cerr << "Error: given context is null" << endl;
    return false;
  }


  try {
    
    obj = context->resolve(*name);
      
  } catch (const CosNaming::NamingContext::NotFound & nf) {
    // TODO: check if nf and expected_exception RTTI types matches
    if(print) {
        cerr << "(" << id << ") TestResolveContext: expected Exception" << 
                NMUtil::print(nf) << endl;
    }
    return true;
    
  } catch (const CosNaming::NamingContext::AlreadyBound & ab) {
    // TODO: check if ab and expected_exception RTTI types matches
    if(print) {
        cerr << "(" << id << ") TestResolveContext: expected Exception" << 
                NMUtil::print(ab) << endl;
    }
    return true;
    
  } catch (const CosNaming::NamingContext::CannotProceed & cp) {
    // TODO: check if cp and expected_exception RTTI types matches
    if(print) {
        cerr << "(" << id << ") TestResolveContext: expected Exception" << 
                NMUtil::print(cp) << endl;
    }
    return true;
  } catch (const CORBA::Exception & e) {
    // TODO: check if e and expected_exception RTTI types matches
  }

  if(print)
    cerr << "(" << id << ") TestResolveContext: OK!!" << endl;
    
  return true;

}
  
CORBA::Boolean TestResolveContext::test1(CosNaming::NamingContext_ptr context,
                                         CORBA::Exception* expected_exception,
                                         const char* id, CORBA::Boolean print)
{
  CosNaming::Name name(1);
  name.length (1);
  name[0].id   = CORBA::string_dup ("subcontext");
  name[0].kind = CORBA::string_dup ("");

  CORBA::Object_ptr obj;

  return test(context, &name, obj, expected_exception, id, print);
}

CORBA::Boolean TestResolveContext::test2(CosNaming::NamingContext_ptr context,
                                         CORBA::Exception* expected_exception,
                                         const char* id, CORBA::Boolean print)
{
  CosNaming::Name name(1);
  name.length (1);
  name[0].id   = CORBA::string_dup ("subcontext");
  name[0].kind = CORBA::string_dup ("requete");

  CORBA::Object_ptr obj;

  return test(context, &name, obj, expected_exception, id, print);
}


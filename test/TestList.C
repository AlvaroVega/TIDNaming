#include "TestList.h"
#include "NMUtil.h"


CORBA::Boolean TestList::test(CosNaming::NamingContext_ptr context, 
                              int list_size,
                              CORBA::Exception* expected_exception,
                              const char* id, CORBA::Boolean print)
{
  if (print)
    cerr << "(" << id << ") TestList " << endl; 
  
  if (context == NULL) {
    cerr << "Error: given context is null" << endl;
    return false;
  }
  
  try {


    CosNaming::BindingList* bl;
    CosNaming::BindingIterator_var bi = NULL;
    
    context->list(list_size, bl, bi); 

    bool exit = false;
    int j = 0;

    do {
      
      if(print) {
        for(int i = 0; i < bl->length(); i++) {
          cerr << " (" << j++ << ")" <<  " id = " << (*bl)[i].binding_name[0].id << " ";
          cerr << "kind = " << (*bl)[i].binding_name[0].kind << " ";
          
          if((*bl)[i].binding_type == CosNaming::nobject)
            cerr << "object" << endl;
          else
            cerr << "naming context" << endl;;            
        }
      }
      
      if(bi == NULL)
        exit = true;
      else {
        exit = !bi->next_n(list_size, bl);
      }
      
    } while (!exit);

    bi->destroy();
  } catch (const CORBA::Exception & e) {
    // TODO: check if cp and expected_exception RTTI types matches
  }

  if(print) {
    cerr << "(" << id << ") TestList: OK!!" << endl;
  }
    
  return true;
}
  

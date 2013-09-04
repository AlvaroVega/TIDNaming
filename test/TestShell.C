#ifdef _TAO
#include "EchoS.h"
#include "orbsvcs/Naming/Naming_Server.h"
#elif defined _MICO
#include "Echo.h"
#include "coss/CosNaming.h"
#elif defined _TIDORB
#include "CosNaming.h"
#include "POA_Echo.h"
#elif defined _OMNI
#include "Echo.hh"
#include "Naming.hh"
#elif defined _ORBACUS
#include <OB/CORBA.h> 
#include <OB/CosNaming.h> 
#include "Echo_skel.h"
#endif

#include <readline/readline.h>
#include <readline/history.h>

#include <unistd.h>
#include <sys/time.h>

#include <string>
#include <iostream>




using namespace std;

class MyEchoTestImpl: virtual public POA_Echo::Test{
public:
  char* echo (const char* s) 
    throw (CORBA::SystemException)
  {
    return CORBA::string_dup(s);
  }
};

char **
alloc_str_array(int arrays, int strs)
{
  char ** tokens = new char*[arrays];
  for (int i = 0; i < arrays; i++)
  {
    tokens[i] = new char[strs];
    bzero(tokens[i], strs);
  }
  return tokens;
}

void
dealloc_str_array(char ** array, int arrays)
{
  for (int i = 0; i < arrays; i++)
    delete array[i];
  delete array;
}

void
reset_str(char * str)
{
  char * i = str;
  while (*i != '\0')
  {
    *i = '\0';
    i++;
  }
}

int
tokenize(const char * line, char * tokv[], int toks, char separator = ' ')
{
  int j, tc = 0, index = 0;
  for (j = 0; line[j]; j++)
    if (line[j] != separator)
    {
      if (j > 0)
      {
        if (line[j - 1] == separator)
          tc++;
      }
      else
        tc++;
    }
    else
      if (j > 0)
      {
        if (line[j - 1 ] != separator)
        {
          reset_str(tokv[tc - 1]);
          strncpy(tokv[tc - 1], &(line[index]), j - index);
          index = j;
        }
        index = j + 1;
      }
        
  if (index < j)
  {
    reset_str(tokv[tc - 1]);
    strncpy(tokv[tc - 1], &(line[index]), j-index);
  }
  
  return tc;
}

typedef map<string, CosNaming::NamingContext_ptr> NamingContextMap;

NamingContextMap global_naming_contexts;

void
create_context(CosNaming::NamingContext_ptr root, const char * name)
{
  CosNaming::NamingContext_ptr nc =
    root->new_context();
  global_naming_contexts[name] = nc;
}

int
gen_name(const char * line, CosNaming::Name & n)
{
  char ** comp = alloc_str_array(64, 256);
  int cc = tokenize(line, comp, 64, '/');
  n.length(cc);
  for (int i = 0; i < cc; i++)
  {
    char ** fields = alloc_str_array(64, 256);
    int fc = tokenize(comp[i], fields, 64, '.');
    switch (fc)
    {
      case 1:
        n[i] = CosNaming::NameComponent(strdup(fields[0]), "");
        break;
      case 2:
        n[i] = CosNaming::NameComponent(strdup(fields[0]), strdup(fields[1]));
        break;
      default:
        cerr << "Error: syntax error in binding name '" << line << "'" << endl;
        dealloc_str_array(fields, 64);
        dealloc_str_array(comp, 64);
        return -1;
        break;
    }
    dealloc_str_array(fields, 64);
  }
  dealloc_str_array(comp, 64);
  
  return 0;
}

CosNaming::NamingContext_ptr
get_ctx(const char * name)
{
  if (name)
  {
    NamingContextMap::iterator it = global_naming_contexts.find(name);
    if (it == global_naming_contexts.end())
    {
      cerr << "Error: unknown naming context '" << name << "'" << endl;
      return CosNaming::NamingContext::_nil();
    }
    return it->second;
  }
  return CosNaming::NamingContext::_nil();
}

void
bind_context(
      CosNaming::NamingContext_ptr root,
      const char * parent,
      const char * child,
      const char * name,
      bool rebind = false)
{
  CosNaming::NamingContext_ptr r, c;

  if (parent)
  {
    if (CORBA::is_nil(r = get_ctx(parent)))
      return;
  }
  else
    r = root;   

  if (CORBA::is_nil(c = get_ctx(child)))
    return;
  
  CosNaming::Name n;
  if (gen_name(name, n))
    return;
  
  if (rebind)
    r->rebind_context(n, c); 
  else
    r->bind_context(n, c); 
    
}

void
bind(
      CosNaming::NamingContext_ptr root,
      CORBA::Object_ptr obj,
      const char * parent,
      const char * name,
      bool rebind = false)
{
  CosNaming::NamingContext_ptr r, c;

  if (parent)
  {
    if (CORBA::is_nil(r = get_ctx(parent)))
      return;
  }
  else
    r = root;   

  CosNaming::Name n;
  if (gen_name(name, n))
    return;
  
  if (rebind)
    r->rebind(n, obj);
  else
    r->bind(n, obj);
}

void
unbind(
      CosNaming::NamingContext_ptr root,
      const char * parent,
      const char * name)
{
  CosNaming::NamingContext_ptr r;

  if (parent)
  {
    if (CORBA::is_nil(r = get_ctx(parent)))
      return;
  }
  else
    r = root;   

  CosNaming::Name n;
  if (gen_name(name, n))
    return;

  r->unbind(n);
}

void
destroy(
      CosNaming::NamingContext_ptr root,
      const char * ctx)
{
  CosNaming::NamingContext_ptr r;

  if (CORBA::is_nil(r = get_ctx(ctx)))
    return;

  r->destroy();
  NamingContextMap::iterator it = global_naming_contexts.find(ctx);
  global_naming_contexts.erase(it);
}
      
void
print_help()
{
  cerr << endl <<
"Allowed commands:" << endl << endl <<
"   help                            Print this help" << endl <<
"   quit                            Quit shell" << endl <<
"   new_context <ctx_id>            Create a new context with <ctx_id>" << endl <<
"   destroy <ctx_id>                Destroy context with <ctx_id>" << endl <<
"   bind <ctx_id> <bind name>       Bind an object to context with" << endl <<
"                                   <ctx_id> and with name <bind name>" << endl <<
"   bind <bind name>                Bind an object to root context" << endl <<
"                                   with name <bind name>" << endl <<
"   rebind <ctx_id> <bind name>     Rebind an object to context with" << endl <<
"                                   <ctx_id> and with name <bind name>" << endl <<
"   rebind <bind name>              Rebind an object to root context" << endl <<
"                                   with name <bind name>" << endl <<
"   bind_context <par_ctx_id>" << endl <<
"                <child_ctx_id>" << endl <<
"                <bind name>        Bind a context with <child_ctx_i> to" << endl << 
"                                   parent context with <child_ctx_id> " << endl <<
"                                   and name <bind name>" << endl <<
"   bind_context <child_ctx_id>" << endl <<
"                <bind name>        Bind a context with <child_ctx_i> to root " << endl << 
"                                   parent context and name <bind name>" << endl <<
"   rebind_context <par_ctx_id>" << endl <<
"                  <child_ctx_id>" << endl <<
"                  <bind name>      Rebind a context with <child_ctx_i> to" << endl << 
"                                   parent context with <child_ctx_id> " << endl <<
"                                   and name <bind name>" << endl <<
"   rebind_context <child_ctx_id>" << endl <<
"                  <bind name>      Rebind a context with <child_ctx_i> to root " << endl << 
"                                   parent context and name <bind name>" << endl <<
"   refs                            Print the list of naming context references" << endl <<
endl;
}

void
list_refs()
{
  NamingContextMap::iterator it = global_naming_contexts.begin();
  if (it == global_naming_contexts.end())
  {
    cerr << "   No references defined" << endl;
  }
  while (it != global_naming_contexts.end())
  {
    cerr << "   '" << it->first << "'" << endl;
    it++;
  }
  cerr << endl;
}

int main(int argc, char *argv[])
{

  //char pwd[256], uri[1000];
  
  cerr << "TIDNaming TestShell, the TID CORBA "\
          "Naming Service Shell." << endl;
  cerr << "Copyright 2007 Telefonica Investigacion y Desarrollo S.A." << endl;
  cerr << "All rights reserved" << endl << endl;
  cerr << "Type 'help' for instructions" << endl << endl;
  
  try {

    CORBA::ORB_ptr orb = CORBA::ORB_init (argc, argv, "");
    
    /*
     * Obtain a reference to the RootPOA and its Manager
     */

    CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
    PortableServer::POAManager_var mgr = poa->the_POAManager();


    /*
     * Create and activate the servant
     */

    MyEchoTestImpl *servant   = new MyEchoTestImpl;

    PortableServer::ObjectId_var oid  = poa->activate_object (servant);

    CORBA::Object_var ref  = poa->id_to_reference (oid.in());



    /*
     * Obtain NamincContext reference ...
     */

    //    sprintf (uri, "file://%s/ns.ior", getcwd(pwd, 256));
   
    CORBA::Object_var naming_context_object;
    CosNaming::NamingContext_ptr naming_context;
    try
    {
       // For an URI given by args
       char * uri = argv[1];

       naming_context_object = 
         //orb->string_to_object(uri);
         orb->string_to_object("corbaloc:iiop:1.2@127.0.0.1:2809/NameService");
         //orb->resolve_initial_references("NameService");

       naming_context =
          CosNaming::NamingContext::_narrow (naming_context_object.in ());
          
    }
    catch (...)
    {
       cerr << "Error: cannot obtain naming service initial reference" << endl;
       throw;
    }

    //
    // Some simple tests
    //


    char ** tokens = alloc_str_array(64, 256);
    while(true)
    {
      char * line = NULL;
      try
      {
        // Read a line from user
        while (true)
        {
          line = readline(">> ");
          if (line && *line)
          {
            add_history(line);
            break;
          }
        }
    
        int tc = tokenize(line, tokens, 64);
        
        const char * cmd = tokens[0];      
        if (strcmp(cmd, "new_context") == 0)
        {
          if (tc != 2)
            cerr << "Invalid argument count for '" << cmd << "'" << endl;
          else
          {          
            const char * ctx_name = tokens[1];
            create_context(naming_context, ctx_name);
          }
        }
        else if (strcmp(cmd, "destroy") == 0)
        {
          if (tc != 2)
            cerr << "Invalid argument count for '" << cmd << "'" << endl;
          else
          {          
            const char * ctx_name = tokens[1];
            destroy(naming_context, ctx_name);
          }
        }
        else if ((strcmp(cmd, "bind_context") == 0) ||
                 (strcmp(cmd, "rebind_context") == 0))
        {
          const char * parent, * child, *name;
          bool failed = false;
          switch (tc)
          {
            case 3:
              parent = NULL;
              child  = tokens[1];
              name   = tokens[2];
              break;
            case 4:
              parent = tokens[1];
              child  = tokens[2];
              name   = tokens[3];
              break;
            default:
              cerr << "Invalid argument count for '" << cmd << "'" << endl;
              failed = true;
              break;
          }
          if (!failed)
            bind_context(
                naming_context, 
                parent, 
                child, 
                name, 
                strcmp(cmd, "rebind_context") == 0);
        }
        else if ((strcmp(cmd, "bind")   == 0) ||
                 (strcmp(cmd, "rebind") == 0))
        {
          const char * parent, *name;
          bool failed = false;
          switch (tc)
          {
            case 2:
              parent = NULL;
              name   = tokens[1];
              break;
            case 3:
              parent = tokens[1];
              name   = tokens[2];
              break;
            default:
              cerr << "Invalid argument count for '" << cmd << "'" << endl;
              failed = true;
              break;
          }
          if (!failed)
            bind(naming_context, ref, parent, name, strcmp(cmd, "rebind") == 0);
        }
        else if (strcmp(cmd, "unbind")   == 0)
        {
          const char * parent, *name;
          bool failed = false;
          switch (tc)
          {
            case 2:
              parent = NULL;
              name   = tokens[1];
              break;
            case 3:
              parent = tokens[1];
              name   = tokens[2];
              break;
            default:
              cerr << "Invalid argument count for '" << cmd << "'" << endl;
              failed = true;
              break;
          }
          if (!failed)
            unbind(naming_context, parent, name);
        }
        else if (strcmp(cmd, "refs")   == 0)
        {
          list_refs();
        }
        else if (strcmp(cmd, "help")   == 0)
        {
          print_help();
        }
        else if (strcmp(cmd, "quit")   == 0)
        {
          cerr << endl;
          break;
        }
        else
          cerr << "Unknown command '" << line << "'" << endl;       
          
        delete line;
      }
      catch (CosNaming::NamingContext::NotFound &)
      {
        cerr << "Error: name not found" << endl;
        delete line;
      }     
      catch (CosNaming::NamingContext::CannotProceed &)
      {
        cerr << "Error: cannot proceed" << endl;
        delete line;
      }     
      catch (CosNaming::NamingContext::InvalidName &)
      {
        cerr << "Error: invalid name " << endl;
        delete line;
      }     
      catch (CosNaming::NamingContext::AlreadyBound &)
      {
        cerr << "Error: already bounded" << endl;
        delete line;
      }     
      catch (CosNaming::NamingContext::NotEmpty &)
      {
        cerr << "Error: not empty" << endl;
        delete line;
      }     
    }
    
    dealloc_str_array(tokens, 64);
    
    return 0;
    

    orb->destroy();

  } catch (const CORBA::SystemException& ex){
    cerr << "ERROR, excepcion raised: " << ex._name() << " "; // << ex.what() << " ";
    cerr << ex.completed() << endl;
    exit (1);
  }
}

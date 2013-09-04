#include "NMUtil.h"
#include "TIDorb/util.h"

const char* NMUtil::print(CosNaming::Name_ptr name)
{
  TIDorb::util::StringBuffer msg;
  if(name != NULL) {
    CORBA::ULong length = name->length();
    for(int i = 0; i < length; i++) {
      msg << "[" << i << "] id = " << (*name)[i].id << " kind = " << (*name)[i].kind;
    }
  }
  return msg.str().data();
}

const char * NMUtil::print(const CosNaming::NamingContext::NotFound & nf)
{
  TIDorb::util::StringBuffer msg;
  msg << "a NotFound exception raised";
  return msg.str().data();
}


const char * NMUtil::print(const CosNaming::NamingContext::CannotProceed & cp)
{
  TIDorb::util::StringBuffer msg;
  msg << "a CannotProceed exception raised";
  return msg.str().data();
}

const char * NMUtil::print(const CosNaming::NamingContext::AlreadyBound & ab)
{
  TIDorb::util::StringBuffer msg;
  msg << "an AlreadyBound exception raised";
  return msg.str().data();
}

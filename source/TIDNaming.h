// -*- c++ -*-
/*
 *  File name: TIDNaming.h
 *  File type: Header file.
 */
/*
 * Copyright (C) 2008 Telefonica I+D S.A.
 *
 * This source file is a property of Telefonica I+D. You can not
 * use it for any purpose unless you have an explicit authorization
 * by owner.
 */

#ifndef __TIDNAMING_H__
#define __TIDNAMING_H__

#include "TIDorb/config.h"
#include "TIDorb/core.h"
#include "TIDorb/util.h"
#include "TIDThr.h"


#include "CORBA.h"
#include "PortableServer.h"

namespace TIDNaming{

  static const char* st_version  = "1.0.4";

  // Some forward declarations
  class NamingContextImpl;
  class BindingIteratorImpl;
  class BindingIterator_ref;
  class NamingContextExtImpl;
  class ServantManagerNSImpl;
  //  class PersistenceThreadNS;
  //  typedef TIDThr::HandleT<PersistenceThreadNS> PersistenceThreadNS_ref;
}


#include "NamingContextImpl.h"
#include "BindingIteratorImpl.h"
#include "NamingContextExtImpl.h"
#include "ServantManagerNSImpl.h"
//#include "PersistenceThreadNS.h"
//#include "AgentImpl.h"




#endif

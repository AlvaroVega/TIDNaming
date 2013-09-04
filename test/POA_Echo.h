// POA_Echo.h
//
// File generated: Thu Apr 22 13:39:43 CEST 2010
//   by TIDIdlc idl2cpp 1.3.11
//

#ifndef __POA_ECHO_H_
#define __POA_ECHO_H_
#include "PortableServer.h"

#include "Echo.h"
//
// POA file POA_Echo.h (from module)
//
// File generated: Thu Apr 22 13:39:42 CEST 2010
//   by TIDorb idl2cpp 1.3.11 
//   external POA definition File.
//




namespace POA_Echo
{
//
// Test.h (skeleton)
//
// File generated: Thu Apr 22 13:39:42 CEST 2010
//   by TIDIdlc idl2cpp 1.3.11
//



struct __my_Test__ltstr{
		bool operator()(const char* s1, const char* s2) const;
};//end __my_Test__ltstr struct

struct __POA_TestMAP {
		map<const char*,int,__my_Test__ltstr> _methods;
		__POA_TestMAP();
};// end of __POA_TestMAP struct 

class Test: public virtual PortableServer::DynamicImplementation
{

	public:
		Echo::Test_ptr _this();

		void invoke(::CORBA::ServerRequest_ptr _request);
		const CORBA::RepositoryIdSeq_ptr _all_interfaces(PortableServer::POA_ptr poa, const PortableServer::ObjectId& objectId);
		CORBA::RepositoryId _primary_interface(const PortableServer::ObjectId& oid,PortableServer::POA_ptr poa);

		
		virtual char* echo(const char* s) = 0;

	private:

		virtual const CORBA::RepositoryIdSeq_ptr _ids();
		static const CORBA::RepositoryIdSeq_ptr __ids;
		static const CORBA::RepositoryIdSeq_ptr __init_ids();
		static __POA_TestMAP _mapped_methods; 
}; // end of Test





//
// Test_tie.h (tie)
//
// File generated: Thu Apr 22 13:39:42 CEST 2010
//   by TIDIdlc idl2cpp 1.3.11
//



class Test_tie: public Test{

	public:
		Test_tie(Echo::Test_ptr delegate);

		Echo::Test_ptr _delegate();

		virtual const CORBA::RepositoryIdSeq_ptr _all_interfaces(PortableServer::POA_ptr poa, const PortableServer::ObjectId& objectID);

		virtual const CORBA::RepositoryIdSeq_ptr _ids();

	// Operations....
	public:

		char* echo(const char* s);
	private:
		static const CORBA::RepositoryIdSeq_ptr __ids;
		static const CORBA::RepositoryIdSeq_ptr __init_ids();
		Echo::Test_ptr _delegate_tie;
}; // end of Test_tie class.




}


#endif // __POA_ECHO_H_

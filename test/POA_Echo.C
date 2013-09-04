// POA_Echo.C
//
// File generated: Thu Apr 22 13:39:43 CEST 2010
//   by TIDIdlc idl2cpp 1.3.11
//


#include "POA_Echo.h"
//
// Test.C (skeleton)
//
// File generated: Thu Apr 22 13:39:42 CEST 2010
//   by TIDIdlc idl2cpp 1.3.11
//


#include "TIDorb/portable/TypeCodeFactory.h" 

#include "TIDorb/portable/ORB.h"

bool POA_Echo::__my_Test__ltstr::operator()(const char* s1, const char* s2) const
{
	return (strcmp(s1, s2) < 0);
}

POA_Echo::__POA_TestMAP::__POA_TestMAP()
// Constructor
{
	_methods["echo"]=1;
}

Echo::Test_ptr POA_Echo::Test::_this() {
	CORBA::Object_var _ref = PortableServer::DynamicImplementation::_this();
	return Echo::Test::_narrow(_ref);
}

const CORBA::RepositoryIdSeq_ptr POA_Echo::Test::__init_ids(){
	CORBA::RepositoryIdSeq_ptr ids = new  CORBA::RepositoryIdSeq();
	ids->length(1);
	(*ids)[0]=CORBA::string_dup("IDL:Echo/Test:1.0");
	return ids;
}

const CORBA::RepositoryIdSeq_ptr POA_Echo::Test::__ids = POA_Echo::Test::__init_ids();

const CORBA::RepositoryIdSeq_ptr POA_Echo::Test::_ids() {
	return __ids;
}

const CORBA::RepositoryIdSeq_ptr POA_Echo::Test::_all_interfaces(PortableServer::POA_ptr poa, const PortableServer::ObjectId& objectId) 
{
	return _ids();
}

CORBA::RepositoryId POA_Echo::Test::_primary_interface(const PortableServer::ObjectId& oid,PortableServer::POA_ptr poa)
{
	 return CORBA::string_dup("IDL:Echo/Test:1.0");
}

POA_Echo::__POA_TestMAP POA_Echo::Test::_mapped_methods;

void POA_Echo::Test::invoke(::CORBA::ServerRequest_ptr _request)
{
	int _method_id = _mapped_methods._methods[_request->operation()];
	if (_method_id == 0)
	{// Undefined Operation 
		throw ::CORBA::BAD_OPERATION();
	}
	TIDorb::portable::ORB* __orb= dynamic_cast< TIDorb::portable::ORB* > (get_delegate()->orb(this));

		if (_method_id == 1) {
			::CORBA::NVList_var _params;
			__orb->create_list(1, _params);

			::CORBA::NamedValue_var __my_s = _params->add_item("s", ::CORBA::ARG_IN);
			__my_s->value()->delegate().set_type(CORBA::_tc_string);

			_request->arguments(_params);

			 const char* s;
			(*(__my_s->value())) >>= s;

			char* _result = this->echo(s);

			::CORBA::Any _resultAny;
			_resultAny <<=CORBA::Any::from_string(_result, 0, true);
			_request->set_result(_resultAny);


			return;
		}
		else throw ::CORBA::BAD_OPERATION();
}// end of method invoke.

POA_Echo::Test_tie::Test_tie(Echo::Test_ptr delegate)
{
	_delegate_tie = Echo::Test::_duplicate(delegate);
}

Echo::Test_ptr POA_Echo::Test_tie::_delegate()
{
	return _delegate_tie;
}

const CORBA::RepositoryIdSeq_ptr POA_Echo::Test_tie::__init_ids(){
	CORBA::RepositoryIdSeq_ptr ids = new  CORBA::RepositoryIdSeq();
	ids->length(1);
	(*ids)[0]=CORBA::string_dup("IDL:Echo/Test:1.0");
	return ids;
}

const CORBA::RepositoryIdSeq_ptr POA_Echo::Test_tie::__ids =POA_Echo::Test_tie::__init_ids();

const CORBA::RepositoryIdSeq_ptr POA_Echo::Test_tie::_ids()
{
	return __ids;
}

const CORBA::RepositoryIdSeq_ptr POA_Echo::Test_tie::_all_interfaces(PortableServer::POA_ptr poa, const PortableServer::ObjectId& objectId)
{
	return __ids;
}

char* POA_Echo::Test_tie::echo(const char* s)
{
	return _delegate_tie->echo(s);
}




// Echo.C
//
// File generated: Thu Apr 22 13:39:42 CEST 2010
//   by TIDIdlc idl2cpp 1.3.11
//

#include "Echo.h"

#include "POA_Echo.h"
//
// Test.C (interface)
//
// File generated: Thu Apr 22 13:39:42 CEST 2010
//   by TIDIdlc idl2cpp 1.3.11
//


#include "TIDorb/portable/TypeCodeFactory.h" 


//Static Members
Echo::Test_ptr Echo::Test::_narrow(const ::CORBA::Object_ptr obj) 
{
	return Echo::_TestHelper::narrow(obj, false);
}

Echo::Test_ptr Echo::Test::_unchecked_narrow(const ::CORBA::Object_ptr obj) 
{
	return Echo::_TestHelper::narrow(obj, true);
}

Echo::Test_ptr Echo::Test::_duplicate(Echo::Test_ptr ref){
	CORBA::Object::_duplicate(ref);
	return ref;
}// end of Duplicate.

Echo::Test_ptr Echo::Test::_nil(){
	 return Echo::Test::_narrow(CORBA::Object::_nil());
}

void Echo::_TestHelper::insert(::CORBA::Any& any, Echo::Test_ptr _value) {
	TIDorb::portable::Any& delegate = any.delegate();
	delegate.insert_Object((CORBA::Object_ptr)_value,Echo::_tc_Test);
}

void Echo::_TestHelper::insert(::CORBA::Any& any, Echo::Test_ptr* _value, bool must_free) {
	TIDorb::portable::Any& delegate = any.delegate();
	delegate.insert_Object((CORBA::Object_ptr)(*_value), Echo::_tc_Test);
	CORBA::release(*_value);
}

CORBA::Boolean Echo::_TestHelper::extract(const ::CORBA::Any& any, Echo::Test_ptr& _value) {
	::TIDorb::portable::Any& delegate = any.delegate();
	CORBA::Object_var obj;
	bool ret = delegate.extract_Object(obj);
	if (ret){
		if(CORBA::is_nil(obj)) {
			(Echo::Test_ptr&)_value = NULL;
		} else {
			(Echo::Test_ptr&)_value = Echo::_TestHelper::narrow(obj,false);
			ret = ! CORBA::is_nil(_value);
		}
	}
	return ret;
}

CORBA::TypeCode_ptr Echo::_TestHelper::type() {
	if (Echo::_tc_Test) {
		return Echo::_tc_Test;
	}

	return TIDorb::portable::TypeCodeFactory::create_interface_tc(
		(const ::CORBA::RepositoryId) id(),(const ::CORBA::Identifier) "Test");
}

const ::CORBA::TypeCode_ptr Echo::_tc_Test=Echo::_TestHelper::type();

void Echo::_TestHelper::read(::TIDorb::portable::InputStream& is, Echo::Test_ptr& _value) {
	::CORBA::Object_ptr obj;
	is.read_Object(obj);
	_value = Echo::_TestHelper::narrow(obj, true); 
}

Echo::Test_ptr Echo::_TestHelper::narrow(const ::CORBA::Object_ptr obj, bool is_a) {
	if (CORBA::is_nil(obj))
		return NULL;
	Echo::Test_ptr _concrete_ref = dynamic_cast<Echo::Test_ptr> (obj);
 	if (!CORBA::is_nil(_concrete_ref))
		return Echo::Test::_duplicate(_concrete_ref);
	if (is_a || obj->_is_a(id())) {
		Echo::_TestStub* result = new Echo::_TestStub();
		if(result==NULL) 
			throw ::CORBA::NO_MEMORY();
		::TIDorb::portable::Stub* __aux= dynamic_cast< ::TIDorb::portable::Stub*>(obj);
 		if (__aux!=NULL) {
			result->_set_delegate(__aux->_get_delegate());
			return (Echo::Test_ptr)result;
		}
	}
	return Echo::Test::_nil();
}

void Echo::_TestHelper::write(::TIDorb::portable::OutputStream& os, const Echo::Test_ptr _value) {
	os.write_Object((::CORBA::Object_ptr)_value);
}

::CORBA::TypeCode_ptr Echo::_TestHolder::_type() const {
	return CORBA::TypeCode::_duplicate(Echo::_TestHelper::type());
}



//
// _TestStub.C (stub)
//
// File generated: Thu Apr 22 13:39:42 CEST 2010
//   by TIDIdlc idl2cpp 1.3.11
//



const CORBA::RepositoryIdSeq_ptr Echo::_TestStub::__init_ids(){
	CORBA::RepositoryIdSeq_ptr ids = new  CORBA::RepositoryIdSeq();
	ids->length(1);
	(*ids)[0]=CORBA::string_dup("IDL:Echo/Test:1.0");
	return ids;
}

const CORBA::RepositoryIdSeq_ptr Echo::_TestStub::__ids =Echo::_TestStub::__init_ids();

const CORBA::RepositoryIdSeq_ptr Echo::_TestStub::_ids()
{
	return __ids;
}

char* Echo::_TestStub::echo(const char* s)
{
    	CORBA::Request_var _request = this->_request("echo");
	_request->set_return_type(CORBA::_tc_string);
	::CORBA::Any& __my_s = _request->add_in_arg();
	__my_s <<= CORBA::Any::from_string(s, 0);
	_request->invoke();
	CORBA::Exception* _exception = _request->env()->exception();
	if (_exception != NULL)
	{
		CORBA::SystemException_ptr __systemException = CORBA::SystemException::_downcast(_exception);
		if (__systemException != NULL)
		{
			__systemException->_raise();
		}
		throw ::CORBA::INTERNAL();
	}
	char* _l_result;
	const char* const__l_result;
	if (_request->return_value()>>=  const__l_result) {
		_l_result =::CORBA::string_dup(const__l_result);
		return _l_result;
	} else
		throw ::CORBA::INTERNAL();
}




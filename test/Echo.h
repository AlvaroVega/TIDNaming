// Echo.h
//
// File generated: Thu Apr 22 13:39:43 CEST 2010
//   by TIDIdlc idl2cpp 1.3.11
//

#ifndef __ECHO_H_
#define __ECHO_H_

#include "TIDorb/portable.h"

#include "TIDorb/types.h"

#include "CORBA.h"
//
// Echo.h (module)
//
// File generated: Thu Apr 22 13:39:42 CEST 2010
//   by TIDIdlc idl2cpp 1.3.11
//






namespace Echo
{
	// Begin of forward Declarations  definition
		class Test;
		typedef Test* Test_ptr;
		typedef ::TIDorb::templates::InterfaceT_var<Test> Test_var;
		typedef ::TIDorb::templates::InterfaceT_out<Test> Test_out;
	// End of forward declarations


	// Begin of children  header files inclusion
//
// Test.h (interface)
//
// File generated: Thu Apr 22 13:39:42 CEST 2010
//   by TIDIdlc idl2cpp 1.3.11
//



extern const ::CORBA::TypeCode_ptr _tc_Test;

class Test :  public virtual CORBA::Object
{

	public:
		typedef Test_ptr _ptr_type;
		typedef Test_var _var_type;

	// Constructors & operators 
	protected:
		Test() {};
		virtual ~Test() {};

	private:
		void operator= (Test_ptr obj) {};


	public: // Static members
		static Echo::Test_ptr _narrow(const ::CORBA::Object_ptr obj) ;
		static Echo::Test_ptr _unchecked_narrow(const ::CORBA::Object_ptr obj) ;
		static Echo::Test_ptr _duplicate(Echo::Test_ptr val);
		static Echo::Test_ptr _nil();

	public: //Operations, Constants & Attributes Declaration 
		
		virtual char* echo(const char* s) = 0;


}; // end of Testheader definition

class _TestHelper {

	public:
		static ::CORBA::TypeCode_ptr type();

		static const char* id() { return "IDL:Echo/Test:1.0"; }

		static void insert(::CORBA::Any& any, Echo::Test_ptr _value);

		static void insert(::CORBA::Any& any, Echo::Test_ptr* _value, bool must_free=true);

		static CORBA::Boolean extract(const ::CORBA::Any& any, Echo::Test_ptr& _value);

		static void read(::TIDorb::portable::InputStream& is, Echo::Test_ptr& _value);

		static void write(::TIDorb::portable::OutputStream& os, const Echo::Test_ptr _value);

		static Echo::Test_ptr narrow(const ::CORBA::Object_ptr obj, bool is_a);

};// End of helper definition

class _TestHolder: public virtual ::TIDorb::portable::Streamable {

	public:
		Echo::Test_ptr value; 

		_TestHolder() {value = Echo::Test::_nil();}
		_TestHolder(const Echo::Test_ptr initial){ value=Echo::Test::_duplicate((Echo::Test_ptr)initial);
		}
		::CORBA::TypeCode_ptr _type() const;
		void _write(::TIDorb::portable::OutputStream& outs) const
		{
			Echo::_TestHelper::write(outs,value); 
		}
		void _read(::TIDorb::portable::InputStream& ins) 
		{
			Echo::_TestHelper::read(ins,value); 
		}
}; // end of holder class REF




		// Interface Stub Inclusion (Client Side).
//
// _TestStub.h (stub)
//
// File generated: Thu Apr 22 13:39:42 CEST 2010
//   by TIDIdlc idl2cpp 1.3.11
//



class _TestStub:
	public virtual ::TIDorb::portable::Stub,
	public virtual Echo::Test
{

	public:
		virtual const CORBA::RepositoryIdSeq_ptr _ids();

	private:
		static const CORBA::RepositoryIdSeq_ptr __ids;
		static const CORBA::RepositoryIdSeq_ptr __init_ids();

	public:
 
		char* echo(const char* s);


}; //End of _StubClass




	// End of children  header files inclusion

}// end of namespace Echo

//
// Echo_ext.h (from module)
//
// File generated: Thu Apr 22 13:39:42 CEST 2010
//   by TIDorb idl2cpp 1.3.11
//   external Any operators definition File.
//



//
// Test_ext.h (from interface)
//
// File generated: Thu Apr 22 13:39:42 CEST 2010
//   by TIDorb idl2cpp 1.3.11
//   external Any operators definition File.
//



		inline void operator <<= (::CORBA::Any& any, Echo::Test_ptr _value)
		{
			Echo::_TestHelper::insert(any,_value);
		}
		inline void operator <<= (::CORBA::Any& any, Echo::Test_ptr* _value)
		{
			Echo::_TestHelper::insert(any,_value);
		}
		inline CORBA::Boolean operator >>= (const ::CORBA::Any& any, Echo::Test_ptr& _value)
		{
			return Echo::_TestHelper::extract(any, _value);
		}






#endif // __ECHO_H_

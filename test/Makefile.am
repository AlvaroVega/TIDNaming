SUBDIRS = .

AM_CPPFLAGS = \
	-I$(top_srcdir)/source/idl \
	-I$(top_srcdir)/source \
	@TIDORBC_LIBS@ -D_TIDORB

echo_idl_dependencies = Echo.h Echo.C POA_Echo.h POA_Echo.C 

CLEANFILES = $(echo_idl_dependencies)

#POA_Echo.h: Echo.h

POA_Echo.h: Echo.idl
	${IDLC} -D_PRE_3_0_COMPILER_ -I$(top_srcdir)/source/idl/ Echo.idl


testdir = $(pkgdatadir)/test
check_PROGRAMS = Test TestSimple TestSimple_ssl TestShell

nodist_TestShell_SOURCES = $(echo_idl_dependencies)
TestShell_SOURCES = \
	POA_Echo.h \
	TestShell.C

TestShell_LDADD = -lreadline $(std_ldadd)

nodist_TestSimple_SOURCES = $(echo_idl_dependencies)
TestSimple_SOURCES = \
	POA_Echo.h \
	TestSimple.C 

nodist_TestSimple_ssl_SOURCES = $(echo_idl_dependencies)
TestSimple_ssl_SOURCES = \
	POA_Echo.h \
	TestSimple_ssl.C 

noinst_HEADERS = Echo.idl

std_ldadd = \
	$(top_srcdir)/source/idl/libTIDNamingIdl.la \
	$(top_srcdir)/source/libTIDNaming.la \
	@TIDORBC_LDFLAGS@ \
	@TIDORBC_LIBS@

nodist_Test_SOURCES = $(echo_idl_dependencies)
Test_SOURCES = Test.C \
	NMUtil.h \
	NMUtil.C \
	TestBind.h \
	TestBind.C \
	TestUnbind.h \
	TestUnbind.C \
	TestBindContext.h \
	TestBindContext.C \
	TestBindNewContext.h \
	TestBindNewContext.C \
	TestNewContext.h \
	TestNewContext.C \
	TestDestroy.h \
	TestDestroy.C \
	TestList.h \
	TestList.C \
	TestResolveContext.h \
	TestResolveContext.C

Test_LDADD = $(std_ldadd)

TestSimple_LDADD = $(std_ldadd)
TestSimple_ssl_LDADD = $(std_ldadd)

TESTS = Test TestSimple TestShell

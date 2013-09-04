#
#  File name: Makefile.am
#  File type: makefile file.
#

#
# Copyright (C) 2006 Telefonica I+D S.A.
#
# This source file is a property of Telefonica I+D. You can not
# use it for any purpose unless you have an explicit authorization
# by owner.
#

SUBDIRS = idl . 

AM_FLAGS = -DENABLE_DEBUG

AM_CPPFLAGS = \
	-I$(srcdir)/idl \
	@TIDORBC_LIBS@

lib_LTLIBRARIES = libTIDNaming.la

libTIDNaming_la_SOURCES =		\
	NamingContextImpl.h		\
	NamingContextImpl.C		\
	BindingIteratorImpl.h		\
	BindingIteratorImpl.C		\
	NamingContextExtImpl.h		\
	NamingContextExtImpl.C		\
	ServantManagerNSImpl.C  	\
	ServantManagerNSImpl.h		\
	NamingContextReader.h		\
	NamingContextWriter.h		\
	NamingContextFileReader.h	\
	NamingContextFileReader.C 	\
	NamingContextFileWriter.h       \
	NamingContextFileWriter.C	\
	NamingContextNullReader.h	\
	NamingContextNullReader.C 	\
	NamingContextNullWriter.h       \
	NamingContextNullWriter.C	\
	NamingContextIOFactory.h	\
	NamingContextIOFactory.C	\
	NamingContextFileIOFactory.h    \
	NamingContextFileIOFactory.C    \
	NamingContextNullIOFactory.h    \
	NamingContextNullIOFactory.C 
# 	PersistenceThreadNS.C	  	\
# 	PersistenceThreadNS.h		\
# 	AgentImpl.h			\
# 	AgentImpl.C			

libTIDNaming_la_LDFLAGS = \
        -release $(VERSION)

ns_serverdir = $(bindir)
ns_server_PROGRAMS = ns_server 
ns_server_SOURCES = ns_server.C
ns_server_LDADD = $(srcdir)/libTIDNaming.la \
	$(srcdir)/idl/libTIDNamingIdl.la \
	@TIDORBC_LDFLAGS@ @TIDORBC_LIBS@

dist_bin_SCRIPTS = start_naming_ssl.sh

# ncdir = $(bindir)
# nc_PROGRAMS = nc
# nc_SOURCES = nc.C 
# nc_LDADD = $(srcdir)/libTIDNaming.la \
# 	$(srcdir)/idl/libTIDNamingIdl.la \
# 	@TIDORBC_LDFLAGS@ @TIDORBC_LIBS@

# ns_borradir = $(bindir)
# ns_borra_PROGRAMS = ns_borra
# ns_borra_SOURCES = ns_borra.C 
# ns_borra_LDADD = $(srcdir)/libTIDNaming.la \
# 	$(srcdir)/idl/libTIDNamingIdl.la \
# 	@TIDORBC_LDFLAGS@ @TIDORBC_LIBS@

# ns_fixdir = $(bindir)
# ns_fix_PROGRAMS = ns_fix
# ns_fix_SOURCES = ns_fix.C 
# ns_fix_LDADD = $(srcdir)/libTIDNaming.la \
# 	$(srcdir)/idl/libTIDNamingIdl.la \
# 	@TIDORBC_LDFLAGS@ @TIDORBC_LIBS@

# ns_iiopdir = $(bindir)
# ns_iiop_PROGRAMS = ns_iiop
# ns_iiop_SOURCES = ns_iiop.C 
# ns_iiop_LDADD = $(srcdir)/libTIDNaming.la \
# 	$(srcdir)/idl/libTIDNamingIdl.la \
# 	@TIDORBC_LDFLAGS@ @TIDORBC_LIBS@

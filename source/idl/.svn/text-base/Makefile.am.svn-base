#
#  File name: idl/Makefile.am
#  File type: makefile file.
#

#
# Copyright (C) 2006 Telefonica I+D S.A.
#
# This source file is a property of Telefonica I+D. You can not
# use it for any purpose unless you have an explicit authorization
# by owner.
#

AM_CPPFLAGS = \
	-I$(top_srcdir)/source/	\
	@TIDORBC_LIBS@

CLEANFILES = *.C *.h

lib_LTLIBRARIES = libTIDNamingIdl.la 

libTIDNamingIdl_la_LDFLAGS = \
        -release $(VERSION)

idl_sources =               \
	CosNaming.idl       

idl_internal_sources =      \
	NamingInternals.idl 

cpp_headers =           \
	CosNaming.h 

cpp_internal_headers =  \
	NamingInternals.h  

cpp_sources =           \
	CosNaming.C        

cpp_internal_sources =  \
	NamingInternals.C  

poa_cpp_headers =           \
	POA_CosNaming.h      

poa_cpp_internal_headers =  \
	POA_NamingInternals.h 

poa_cpp_sources =             \
	POA_CosNaming.C

poa_cpp_internal_sources =    \
	POA_NamingInternals.C 

nodist_libTIDNamingIdl_la_SOURCES = \
	$(cpp_sources)          \
	$(cpp_internal_sources) \
	$(poa_cpp_sources)          \
	$(poa_cpp_internal_sources) \
	$(cpp_internal_headers) \
	$(poa_cpp_internal_headers)

$(cpp_sources): %.C: %.idl
	${IDLC} -D_PRE_3_0_COMPILER_ $<

$(cpp_internal_sources): %.C: %.idl
	${IDLC} -D_PRE_3_0_COMPILER_ $<

instdir=$(includedir)/TIDNaming

nodist_inst_HEADERS = \
        $(cpp_headers) \
        $(poa_cpp_headers)

inst_HEADERS = \
	$(idl_sources)


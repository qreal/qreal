##
##  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
##  Copyright (C) 2006-2008 - INRIA - Bruno JOFRET
##
##  This file must be used under the terms of the CeCILL.
##  This source file is licensed as described in the file COPYING, which
##  you should have received as part of this distribution.  The terms
##  are also available at
##  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
##
##

libImplicitlist_la_CFLAGS =	-I  $(top_builddir)/src/c/type \
				-I .

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES = libImplicitlist.la

HEAD = implicitList.h

libImplicitlist_la_SOURCES = 	$(HEAD) \
				simplicitLists.c \
				dimplicitLists.c \
				cimplicitLists.c \
				zimplicitLists.c

check_PROGRAMS = 	testFloatImplicitList \
			testDoubleImplicitList

check_LDADD =		$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			libImplicitlist.la

check_INCLUDES =	-I  $(top_builddir)/src/c/type \
			-I .

testFloatImplicitList_SOURCES = testFloatImplicitList.c
testFloatImplicitList_LDADD = $(check_LDADD)
testFloatImplicitList_CFLAGS = $(check_INCLUDES)

testDoubleImplicitList_SOURCES = testDoubleImplicitList.c
testDoubleImplicitList_LDADD = $(check_LDADD)
testDoubleImplicitList_CFLAGS = $(check_INCLUDES)

TESTS =  	testFloatImplicitList \
		testDoubleImplicitList

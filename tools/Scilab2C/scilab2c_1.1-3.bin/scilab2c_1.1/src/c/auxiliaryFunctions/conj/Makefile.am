##
##  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
##  Copyright (C) 2008-2008 - INRIA - Allan SIMON
##
##  This file must be used under the terms of the CeCILL.
##  This source file is licensed as described in the file COPYING, which
##  you should have received as part of this distribution.  The terms
##  are also available at
##  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
##
##

libConj_la_CFLAGS =	-I $(top_builddir)/src/c/type \
			-I ../includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES = libConj.la

HEAD = ../includes/conj.h

libConj_la_SOURCES = 	$(HEAD) \
			cconja.c \
			zconja.c \
			zconjs.c \
	                cconjs.c

check_PROGRAMS = 	testConj

check_LDADD =   	libConj.la \
			$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la



check_INCLUDES =	-I $(top_builddir)/src/c/type \
			-I ../includes

testConj_SOURCES = testConj.c
testConj_LDADD = $(check_LDADD)
testConj_CFLAGS = $(check_INCLUDES)

TESTS =		testConj

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

libAddition_la_CFLAGS =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/operations/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES = libAddition.la

HEAD = ../includes/addition.h

libAddition_la_SOURCES = 	$(HEAD) \
				sadds.c \
				dadds.c \
				cadds.c \
				zadds.c \
				sadda.c \
				dadda.c \
				cadda.c \
				zadda.c

check_PROGRAMS = 	testAddition

check_LDADD =		$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			libAddition.la

check_INCLUDES =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/operations/includes

testAddition_SOURCES = testAddition.c
testAddition_LDADD = $(check_LDADD)
testAddition_CFLAGS = $(check_INCLUDES)

TESTS =		testAddition
##
##  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
##  Copyright (C) 2008 - INRIA - Arnaud TORSET
##
##  This file must be used under the terms of the CeCILL.
##  This source file is licensed as described in the file COPYING, which
##  you should have received as part of this distribution.  The terms
##  are also available at
##  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
##
##

libMatrixFill_la_CFLAGS =	-I $(top_builddir)/src/c/type \
				-I $(top_builddir)/src/c/matrixOperations/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES = libMatrixFill.la

HEAD = ../includes/fill.h

libMatrixFill_la_SOURCES = 		$(HEAD) \
					sfilla.c \
					dfilla.c \
					cfilla.c \
					zfilla.c


############
##  CHECK
############


check_PROGRAMS = 	testDoubleFill   testFloatFill

check_LDADD =		$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			libMatrixFill.la

check_INCLUDES =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/matrixOperations/includes

testDoubleFill_SOURCES = testDoubleFill.c
testDoubleFill_LDADD = $(check_LDADD)
testDoubleFill_CFLAGS = $(check_INCLUDES)

testFloatFill_SOURCES = testFloatFill.c
testFloatFill_LDADD = $(check_LDADD)
testFloatFill_CFLAGS = $(check_INCLUDES)

TESTS =		testDoubleFill   testFloatFill





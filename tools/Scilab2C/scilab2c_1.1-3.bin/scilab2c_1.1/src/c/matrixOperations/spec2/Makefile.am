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

libSpec2_la_CFLAGS =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/matrixOperations/includes \
			-I $(top_builddir)/src/c/operations/includes \
			-I $(top_builddir)/src/c/elementaryFunctions/includes\
			-I $(top_builddir)/src/c/auxiliaryFunctions/includes


instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES = libSpec2.la

HEAD = ../includes/spec.h

libSpec2_la_SOURCES = 	$(HEAD) \
				sspec2a.c \
				dspec2a.c \
				cspec2a.c \
				zspec2a.c


############
##  CHECK
############


check_PROGRAMS = 	testDoubleSpec2    testFloatSpec2

check_LDADD =		$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/elementaryFunctions/sqrt/libSqrt.la \
			$(top_builddir)/src/fortran/lapack/libscilapack.la \
			$(top_builddir)/src/fortran/blas/libsciblas.la \
			$(top_builddir)/src/c/auxiliaryFunctions/abs/libAbs.la \
			$(top_builddir)/src/c/auxiliaryFunctions/conj/libConj.la \
			$(top_builddir)/src/c/auxiliaryFunctions/sign/libSign.la \
			$(top_builddir)/src/c/auxiliaryFunctions/pythag/libPythag.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			$(top_builddir)/src/c/matrixOperations/inversion/libMatrixInversion.la\
			$(top_builddir)/src/c/matrixOperations/zeros/libMatrixZeros.la\
			libSpec2.la

check_INCLUDES =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/matrixOperations/includes \
			-I $(top_builddir)/src/c/operations/includes \
			-I $(top_builddir)/src/c/elementaryFunctions/includes\
			-I $(top_builddir)/src/c/auxiliaryFunctions/includes

testDoubleSpec2_SOURCES = testDoubleSpec2.c
testDoubleSpec2_LDADD = $(check_LDADD)
testDoubleSpec2_CFLAGS = $(check_INCLUDES)

testFloatSpec2_SOURCES = testFloatSpec2.c
testFloatSpec2_LDADD = $(check_LDADD)
testFloatSpec2_CFLAGS = $(check_INCLUDES)

TESTS =		testDoubleSpec2   testFloatSpec2


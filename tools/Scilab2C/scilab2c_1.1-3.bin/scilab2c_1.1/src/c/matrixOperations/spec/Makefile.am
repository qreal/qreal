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

libSpec_la_CFLAGS =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/matrixOperations/includes \
			-I $(top_builddir)/src/c/operations/includes \
			-I $(top_builddir)/src/c/elementaryFunctions/includes\
			-I $(top_builddir)/src/c/auxiliaryFunctions/includes


instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES = libSpec.la

HEAD = ../includes/spec.h

libSpec_la_SOURCES = 	$(HEAD) \
			sspeca.c \
			dspeca.c \
			cspeca.c \
			zspeca.c


############
##  CHECK
############


check_PROGRAMS = 	testDoubleSpec    testFloatSpec

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
			libSpec.la

check_INCLUDES =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/matrixOperations/includes \
			-I $(top_builddir)/src/c/operations/includes \
			-I $(top_builddir)/src/c/elementaryFunctions/includes\
			-I $(top_builddir)/src/c/auxiliaryFunctions/includes

testDoubleSpec_SOURCES = testDoubleSpec.c
testDoubleSpec_LDADD = $(check_LDADD)
testDoubleSpec_CFLAGS = $(check_INCLUDES)

testFloatSpec_SOURCES = testFloatSpec.c
testFloatSpec_LDADD = $(check_LDADD)
testFloatSpec_CFLAGS = $(check_INCLUDES)

TESTS =		testDoubleSpec   testFloatSpec


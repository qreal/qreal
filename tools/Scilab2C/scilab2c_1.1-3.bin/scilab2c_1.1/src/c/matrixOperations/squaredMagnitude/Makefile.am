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

libMatrixSquaredMagnitude_la_CFLAGS =	-I $(top_builddir)/src/c/type \
					-I $(top_builddir)/src/c/matrixOperations/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES = libMatrixSquaredMagnitude.la

HEAD = ../includes/matrixSquaredMagnitude.h

libMatrixSquaredMagnitude_la_SOURCES = 	$(HEAD) \
					ssquMagns.c \
					ssquMagna.c \
					dsquMagns.c \
					dsquMagna.c \
					csquMagns.c \
					csquMagna.c \
					zsquMagns.c \
					zsquMagna.c

#############
##  CHECK  ##
#############

check_PROGRAMS = 	testDoubleMatrixSquaredMagnitude   testFloatMatrixSquaredMagnitude

check_LDADD =		$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/fortran/lapack/libscilapack.la \
			$(top_builddir)/src/c/elementaryFunctions/sqrt/libSqrt.la \
			$(top_builddir)/src/c/auxiliaryFunctions/abs/libAbs.la \
			$(top_builddir)/src/c/auxiliaryFunctions/pythag/libPythag.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			$(top_builddir)/src/c/matrixOperations/magnitude/libMatrixMagnitude.la \
			libMatrixSquaredMagnitude.la

check_INCLUDES =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/matrixOperations/includes \
			-I $(top_builddir)/src/c/elementaryFunctions/includes

testDoubleMatrixSquaredMagnitude_SOURCES = 	testDoubleSquaredMagnitude.c
testDoubleMatrixSquaredMagnitude_LDADD   = 	$(check_LDADD)
testDoubleMatrixSquaredMagnitude_CFLAGS  = 	$(check_INCLUDES)

testFloatMatrixSquaredMagnitude_SOURCES = 	testFloatSquaredMagnitude.c
testFloatMatrixSquaredMagnitude_LDADD   = 	$(check_LDADD)
testFloatMatrixSquaredMagnitude_CFLAGS  = 	$(check_INCLUDES)

TESTS =		testDoubleMatrixSquaredMagnitude   testFloatMatrixSquaredMagnitude

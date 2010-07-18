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

libMatrixMagnitude_la_CFLAGS =	-I $(top_builddir)/src/c/type \
				-I $(top_builddir)/src/c/matrixOperations/includes \
				-I $(top_builddir)/src/c/auxiliaryFunctions/includes \
				-I $(top_builddir)/src/c/operations/includes \
				-I $(top_builddir)/src/c/elementaryFunctions/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES = libMatrixMagnitude.la

HEAD = ../includes/matrixMagnitude.h

libMatrixMagnitude_la_SOURCES = 	$(HEAD) \
					smagns.c \
					smagna.c \
					dmagns.c \
					dmagna.c \
					cmagns.c \
					cmagna.c \
					zmagns.c \
					zmagna.c

#############
##  CHECK  ##
#############

check_PROGRAMS = 	testDoubleMatrixMagnitude   testFloatMatrixMagnitude

check_LDADD =		$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/fortran/lapack/libscilapack.la \
			$(top_builddir)/src/c/elementaryFunctions/sqrt/libSqrt.la \
			$(top_builddir)/src/c/auxiliaryFunctions/abs/libAbs.la \
			$(top_builddir)/src/c/auxiliaryFunctions/pythag/libPythag.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			libMatrixMagnitude.la

check_INCLUDES =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/matrixOperations/includes \
			-I $(top_builddir)/src/c/operations/includes \
			-I $(top_builddir)/src/c/auxiliaryFunctions/includes \
			-I $(top_builddir)/src/c/elementaryFunctions/includes

testDoubleMatrixMagnitude_SOURCES = 	testDoubleMagnitude.c
testDoubleMatrixMagnitude_LDADD = $(check_LDADD)
testDoubleMatrixMagnitude_CFLAGS = $(check_INCLUDES)

testFloatMatrixMagnitude_SOURCES = 	testFloatMagnitude.c
testFloatMatrixMagnitude_LDADD = $(check_LDADD)
testFloatMatrixMagnitude_CFLAGS = $(check_INCLUDES)

TESTS =		testDoubleMatrixMagnitude   testFloatMatrixMagnitude

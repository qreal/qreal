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

libDist_la_CFLAGS =		-I $(top_builddir)/src/c/type \
				-I $(top_builddir)/src/c/matrixOperations/includes \
				-I $(top_builddir)/src/c/operations/includes \
				-I $(top_builddir)/src/c/elementaryFunctions/includes\
				-I $(top_builddir)/src/c/auxiliaryFunctions/includes


instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES = libDist.la

HEAD = ../includes/dist.h

libDist_la_SOURCES = 			$(HEAD) \
					sdists.c \
					sdista.c \
					ddists.c \
					ddista.c \
					cdists.c \
					cdista.c \
					zdists.c \
					zdista.c


############
##  CHECK
############


check_PROGRAMS = 	testDoubleDist   testFloatDist

check_LDADD =		$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/fortran/lapack/libscilapack.la \
			$(top_builddir)/src/c/elementaryFunctions/pow/libPow.la \
			$(top_builddir)/src/c/elementaryFunctions/sqrt/libSqrt.la \
			$(top_builddir)/src/c/elementaryFunctions/exp/libExp.la \
			$(top_builddir)/src/c/elementaryFunctions/log/libLog.la \
			$(top_builddir)/src/c/elementaryFunctions/log1p/libLog1p.la \
			$(top_builddir)/src/c/elementaryFunctions/lnp1m1/libLnp1m1.la \
			$(top_builddir)/src/c/elementaryFunctions/sin/libSin.la \
			$(top_builddir)/src/c/elementaryFunctions/cos/libCos.la \
			$(top_builddir)/src/c/elementaryFunctions/sinh/libSinh.la \
			$(top_builddir)/src/c/elementaryFunctions/cosh/libCosh.la \
			$(top_builddir)/src/c/auxiliaryFunctions/pythag/libPythag.la \
			$(top_builddir)/src/c/auxiliaryFunctions/abs/libAbs.la \
			$(top_builddir)/src/c/auxiliaryFunctions/conj/libConj.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/c/operations/subtraction/libSubtraction.la \
			$(top_builddir)/src/c/matrixOperations/multiplication/libMatrixMultiplication.la \
			$(top_builddir)/src/c/matrixOperations/transpose/libMatrixTranspose.la \
			$(top_builddir)/src/c/matrixOperations/spec/libSpec.la \
			$(top_builddir)/src/c/matrixOperations/zeros/libMatrixZeros.la \
			libDist.la

check_INCLUDES =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/matrixOperations/includes \
			-I $(top_builddir)/src/c/operations/includes \
			-I $(top_builddir)/src/c/elementaryFunctions/includes\
			-I $(top_builddir)/src/c/auxiliaryFunctions/includes

testDoubleDist_SOURCES = testDoubleDist.c
testDoubleDist_LDADD = $(check_LDADD)
testDoubleDist_CFLAGS = $(check_INCLUDES)

testFloatDist_SOURCES = testFloatDist.c
testFloatDist_LDADD = $(check_LDADD)
testFloatDist_CFLAGS = $(check_INCLUDES)

TESTS =		testDoubleDist   testFloatDist





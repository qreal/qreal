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




libLogm_la_CFLAGS =	-I ./includes \
			-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/elementaryFunctions/includes \
			-I $(top_builddir)/src/c/operations/includes \
			-I $(top_builddir)/src/c/auxiliaryFunctions/includes \
			-I $(top_builddir)/src/c/matrixOperations/includes


instdir = $(top_builddir)/lib


pkglib_LTLIBRARIES = libLogm.la

HEAD = ../includes/logm.h


libLogm_la_SOURCES = 		$(HEAD) \
				 zlogma.c \
				 clogma.c \
				 slogma.c \
				 dlogma.c

#############
##  CHECK  ##
#############

check_PROGRAMS = 	testDoubleLogm	testFloatLogm

check_LDADD =			$(top_builddir)/src/c/type/libDoubleComplex.la \
				$(top_builddir)/src/c/type/libFloatComplex.la \
				$(top_builddir)/src/fortran/lapack/libscilapack.la \
				$(top_builddir)/src/fortran/blas/libsciblas.la \
				$(top_builddir)/src/c/elementaryFunctions/log/libLog.la \
				$(top_builddir)/src/c/elementaryFunctions/sqrt/libSqrt.la \
				$(top_builddir)/src/c/elementaryFunctions/log1p/libLog1p.la \
				$(top_builddir)/src/c/elementaryFunctions/lnp1m1/libLnp1m1.la \
				$(top_builddir)/src/c/auxiliaryFunctions/abs/libAbs.la \
				$(top_builddir)/src/c/auxiliaryFunctions/conj/libConj.la \
				$(top_builddir)/src/c/auxiliaryFunctions/pythag/libPythag.la \
				$(top_builddir)/src/c/auxiliaryFunctions/sign/libSign.la \
				$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
				$(top_builddir)/src/c/operations/addition/libAddition.la \
				$(top_builddir)/src/c/matrixOperations/multiplication/libMatrixMultiplication.la \
				$(top_builddir)/src/c/matrixOperations/transpose/libMatrixTranspose.la \
				$(top_builddir)/src/c/matrixOperations/inversion/libMatrixInversion.la \
				$(top_builddir)/src/c/matrixOperations/spec2/libSpec2.la \
				$(top_builddir)/src/c/matrixOperations/zeros/libMatrixZeros.la \
				$(top_builddir)/src/c/matrixOperations/logm/libLogm.la


check_INCLUDES =	-I ./includes \
			-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/elementaryFunctions/includes \
			-I $(top_builddir)/src/c/operations/includes \
			-I $(top_builddir)/src/c/auxiliaryFunctions/includes \
			-I $(top_builddir)/src/c/matrixOperations/includes

testDoubleLogm_SOURCES = testDoubleLogm.c
testDoubleLogm_LDADD = $(check_LDADD)
testDoubleLogm_CFLAGS = $(check_INCLUDES)

testFloatLogm_SOURCES = testFloatLogm.c
testFloatLogm_LDADD = $(check_LDADD)
testFloatLogm_CFLAGS = $(check_INCLUDES)

TESTS =	testDoubleLogm	testFloatLogm

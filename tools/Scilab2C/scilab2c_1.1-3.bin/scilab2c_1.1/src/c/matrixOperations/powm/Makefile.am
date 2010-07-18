##
##  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
##  Copyright (C) 2008-2008 - INRIA - Arnaud TORSET
##
##  This file must be used under the terms of the CeCILL.
##  This source file is licensed as described in the file COPYING, which
##  you should have received as part of this distribution.  The terms
##  are also available at
##  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
##
##

libMatrixPow_la_CFLAGS =	-I $(top_builddir)/src/c/type \
				-I $(top_builddir)/includes \
				-I $(top_builddir)/src/c/operations/includes\
				-I $(top_builddir)/src/c/auxiliaryFunctions/includes\
				-I $(top_builddir)/src/c/elementaryFunctions/includes\
				-I $(top_builddir)/src/c/matrixOperations/includes


instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES = libMatrixPow.la

HEAD = ../includes/matrixPow.h

libMatrixPow_la_SOURCES = 	$(HEAD) \
				spowma.c\
				dpowma.c\
				cpowma.c\
				zpowma.c

check_PROGRAMS = 	testDoubleMatrixPow	testFloatMatrixPow

check_LDADD =		$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/fortran/lapack/libscilapack.la \
			$(top_builddir)/src/c/auxiliaryFunctions/abs/libAbs.la \
			$(top_builddir)/src/c/auxiliaryFunctions/sign/libSign.la \
			$(top_builddir)/src/c/auxiliaryFunctions/pythag/libPythag.la \
			$(top_builddir)/src/c/auxiliaryFunctions/frexp/libFrexp.la \
			$(top_builddir)/src/c/auxiliaryFunctions/conj/libConj.la \
			$(top_builddir)/src/c/elementaryFunctions/sqrt/libSqrt.la \
			$(top_builddir)/src/c/elementaryFunctions/exp/libExp.la \
			$(top_builddir)/src/c/elementaryFunctions/cos/libCos.la \
			$(top_builddir)/src/c/elementaryFunctions/pow/libPow.la \
			$(top_builddir)/src/c/elementaryFunctions/log/libLog.la \
			$(top_builddir)/src/c/elementaryFunctions/log1p/libLog1p.la \
			$(top_builddir)/src/c/elementaryFunctions/lnp1m1/libLnp1m1.la\
			$(top_builddir)/src/c/elementaryFunctions/cosh/libCosh.la \
			$(top_builddir)/src/c/elementaryFunctions/sin/libSin.la \
			$(top_builddir)/src/c/elementaryFunctions/sinh/libSinh.la \
			$(top_builddir)/src/c/matrixOperations/spec2/libSpec2.la \
			$(top_builddir)/src/c/matrixOperations/zeros/libMatrixZeros.la \
			$(top_builddir)/src/c/matrixOperations/inversion/libMatrixInversion.la \
			$(top_builddir)/src/c/matrixOperations/transpose/libMatrixTranspose.la \
			$(top_builddir)/src/c/matrixOperations/cat/libMatrixConcatenation.la \
			$(top_builddir)/src/c/matrixOperations/expm/libMatrixExponential.la \
			$(top_builddir)/src/c/matrixOperations/logm/libLogm.la \
			$(top_builddir)/src/c/matrixOperations/infiniteNorm/libMatrixInfiniteNorm.la \
			$(top_builddir)/src/c/matrixOperations/eye/libMatrixEye.la \
			$(top_builddir)/src/c/matrixOperations/multiplication/libMatrixMultiplication.la \
			$(top_builddir)/src/c/matrixOperations/division/libMatrixDivision.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			$(top_builddir)/src/c/operations/subtraction/libSubtraction.la \
			$(top_builddir)/src/c/operations/division/libDivision.la \
			$(top_builddir)/src/fortran/blas/libsciblas.la \
			libMatrixPow.la

check_INCLUDES =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/includes \
			-I $(top_builddir)/src/c/auxiliaryFunctions/includes\
			-I $(top_builddir)/src/c/elementaryFunctions/includes\
			-I $(top_builddir)/src/c/operations/includes\
			-I $(top_builddir)/src/c/matrixOperations/includes

testDoubleMatrixPow_SOURCES = testDoublePowm.c
testDoubleMatrixPow_LDADD = $(check_LDADD)
testDoubleMatrixPow_CFLAGS = $(check_INCLUDES)

testFloatMatrixPow_SOURCES = testFloatPowm.c
testFloatMatrixPow_LDADD = $(check_LDADD)
testFloatMatrixPow_CFLAGS = $(check_INCLUDES)

TESTS =	testDoubleMatrixPow	testFloatMatrixPow

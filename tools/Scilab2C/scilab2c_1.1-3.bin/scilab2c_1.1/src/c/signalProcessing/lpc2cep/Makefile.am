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




libLpc2cep_la_CFLAGS =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/elementaryFunctions/includes \
			-I $(top_builddir)/src/c/auxiliaryFunctions/includes \
			-I $(top_builddir)/src/c/matrixOperations/includes \
			-I $(top_builddir)/src/c/operations/includes \
			-I $(top_builddir)/src/c/signalProcessing/includes


instdir = $(top_builddir)/lib


pkglib_LTLIBRARIES = libLpc2cep.la

HEAD = ../includes/lpc2cep.h


libLpc2cep_la_SOURCES = 	$(HEAD) \
				slpc2cepa.c \
				dlpc2cepa.c \
				clpc2cepa.c \
				zlpc2cepa.c


###############
#### Check ####
###############
check_PROGRAMS = 	testDoubleLpc2cep	testFloatLpc2cep


check_LDADD =		$(top_builddir)/src/c/type/libDoubleComplex.la \
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
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/c/operations/subtraction/libSubtraction.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			$(top_builddir)/src/c/matrixOperations/multiplication/libMatrixMultiplication.la \
			$(top_builddir)/src/c/matrixOperations/transpose/libMatrixTranspose.la \
			$(top_builddir)/src/c/matrixOperations/inversion/libMatrixInversion.la \
			$(top_builddir)/src/c/matrixOperations/logm/libLogm.la\
			$(top_builddir)/src/c/matrixOperations/spec2/libSpec2.la\
			$(top_builddir)/src/c/matrixOperations/zeros/libMatrixZeros.la\
			$(top_builddir)/src/c/signalProcessing/fft/libFft.la \
			$(top_builddir)/src/c/signalProcessing/ifft/libIfft.la \
			$(top_builddir)/src/c/signalProcessing/lpc2cep/libLpc2cep.la \
			@LIBMATH@


check_INCLUDES =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/elementaryFunctions/includes \
			-I $(top_builddir)/src/c/auxiliaryFunctions/includes \
			-I $(top_builddir)/src/c/matrixOperations/includes \
			-I $(top_builddir)/src/c/operations/includes \
			-I $(top_builddir)/src/c/signalProcessing/includes


testDoubleLpc2cep_SOURCES = testDoubleLpc2cep.c
testDoubleLpc2cep_LDADD = $(check_LDADD)
testDoubleLpc2cep_CFLAGS = $(check_INCLUDES)

testFloatLpc2cep_SOURCES = testFloatLpc2cep.c
testFloatLpc2cep_LDADD = $(check_LDADD)
testFloatLpc2cep_CFLAGS = $(check_INCLUDES)


TESTS =		testDoubleLpc2cep 	testFloatLpc2cep

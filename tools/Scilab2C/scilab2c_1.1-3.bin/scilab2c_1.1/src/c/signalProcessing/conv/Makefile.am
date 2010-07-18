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




libConv_la_CFLAGS =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/operations/includes \
			-I $(top_builddir)/src/c/signalProcessing/includes


instdir = $(top_builddir)/lib


pkglib_LTLIBRARIES = libConv.la

HEAD = ../includes/conv.h



libConv_la_SOURCES = 	$(HEAD) \
			sconva.c \
			dconva.c \
			cconva.c \
			zconva.c


###############
#### Check ####
###############

check_LDADD =		$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			$(top_builddir)/src/c/signalProcessing/ifft/libIfft.la \
			$(top_builddir)/src/c/signalProcessing/fft/libFft.la \
			$(top_builddir)/src/fortran/lapack/libscilapack.la \
			$(top_builddir)/src/fortran/blas/libsciblas.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/c/operations/subtraction/libSubtraction.la \
			$(top_builddir)/src/c/matrixOperations/zeros/libMatrixZeros.la \
			libConv.la


check_INCLUDES =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/operations/includes \
			-I $(top_builddir)/src/c/signalProcessing/fft \
			-I $(top_builddir)/src/c/signalProcessing/ifft \
			-I $(top_builddir)/src/c/signalProcessing/includes

check_PROGRAMS = 	testFloatConv       testDoubleConv

TESTS =		testFloatConv       testDoubleConv


testDoubleConv_SOURCES = testDoubleConv.c
testDoubleConv_LDADD = $(check_LDADD)
testDoubleConv_CFLAGS = $(check_INCLUDES)

testFloatConv_SOURCES = testFloatConv.c
testFloatConv_LDADD = $(check_LDADD)
testFloatConv_CFLAGS = $(check_INCLUDES)




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




libLev_la_CFLAGS =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/matrixOperations/includes \
			-I $(top_builddir)/src/c/operations/includes \
			-I $(top_builddir)/src/c/auxiliaryFunctions/includes \
			-I $(top_builddir)/src/c/elementaryFunctions/includes \
			-I $(top_builddir)/src/c/signalProcessing/includes


instdir = $(top_builddir)/lib


pkglib_LTLIBRARIES = libLev.la

HEAD = ../includes/lev.h


libLev_la_SOURCES = 	$(HEAD) \
			dleva.c\
			sleva.c\
			zleva.c\
			cleva.c\
			dleva2.c


###############
#### Check ####
###############
check_PROGRAMS = 	testDoubleLev	testFloatLev


check_LDADD =		$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/fortran/blas/libsciblas.la \
			$(top_builddir)/src/fortran/lapack/libscilapack.la \
			$(top_builddir)/src/c/auxiliaryFunctions/conj/libConj.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			$(top_builddir)/src/c/operations/subtraction/libSubtraction.la \
			$(top_builddir)/src/c/operations/division/libDivision.la \
			$(top_builddir)/src/c/signalProcessing/lev/libLev.la \
			@LIBMATH@


check_INCLUDES =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/signalProcessing/includes \
			-I $(top_builddir)/src/c/matrixOperations/includes \
			-I $(top_builddir)/src/c/operations/includes \
			-I $(top_builddir)/src/c/auxiliaryFunctions/includes



testDoubleLev_SOURCES = testDoubleLev.c
testDoubleLev_LDADD = $(check_LDADD)
testDoubleLev_CFLAGS = $(check_INCLUDES)

testFloatLev_SOURCES = testFloatLev.c
testFloatLev_LDADD = $(check_LDADD)
testFloatLev_CFLAGS = $(check_INCLUDES)


TESTS =		testDoubleLev	testFloatLev

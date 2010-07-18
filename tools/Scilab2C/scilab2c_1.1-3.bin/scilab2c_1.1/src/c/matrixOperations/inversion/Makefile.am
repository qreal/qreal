##
##  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
##  Copyright (C) 2008-2008 - INRIA - Allan SIMON
##
##  This file must be used under the terms of the CeCILL.
##  This source file is licensed as described in the file COPYING, which
##  you should have received as part of this distribution.  The terms
##  are also available at
##  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
##
##

libMatrixInversion_la_CFLAGS =	-I $(top_builddir)/src/c/type \
				-I $(top_builddir)/src/c/matrixOperations/includes \
				-I $(top_builddir)/src/c/auxiliaryFunctions/includes \
				-I $(top_builddir)/includes \
				-I $(top_builddir)/src/c/elementaryFunctions/includes


instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES = libMatrixInversion.la

HEAD = ../includes/matrixInversion.h

libMatrixInversion_la_SOURCES =	$(HEAD) \
			        sinverma.c \
				dinverma.c \
				cinverma.c \
		                zinverma.c

check_PROGRAMS = 	testMatrixInversion

check_LDADD =		$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/elementaryFunctions/sqrt/libSqrt.la \
			$(top_builddir)/src/fortran/lapack/libscilapack.la \
			$(top_builddir)/src/fortran/blas/libsciblas.la \
			$(top_builddir)/src/c/auxiliaryFunctions/abs/libAbs.la \
			$(top_builddir)/src/c/auxiliaryFunctions/sign/libSign.la \
			$(top_builddir)/src/c/auxiliaryFunctions/pythag/libPythag.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			libMatrixInversion.la

check_INCLUDES =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/matrixOperations/includes \
                    	-I $(top_builddir)/src/c/auxiliaryFunctions/includes \
                    	-I $(top_builddir)/includes \
                    	-I $(top_builddir)/src/c/elementaryFunctions/includes

testMatrixInversion_SOURCES = testMatrixInversion.c
testMatrixInversion_LDADD = $(check_LDADD)
testMatrixInversion_CFLAGS = $(check_INCLUDES)

TESTS =		testMatrixInversion

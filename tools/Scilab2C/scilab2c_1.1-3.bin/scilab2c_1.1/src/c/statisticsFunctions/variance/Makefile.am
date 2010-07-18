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

libVariance_la_CFLAGS =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/elementaryFunctions/includes \
			-I $(top_builddir)/src/c/matrixOperations/includes \
			-I $(top_builddir)/src/c/operations/includes \
			-I ../includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libVariance.la

libVariance_la_SOURCES =	$(HEAD) $(SRC)

SRC	=	svariancea.c \
		scolumnvariancea.c \
		dvariancea.c \
		srowvariancea.c \
		drowvariancea.c \
		dcolumnvariancea.c \
		cvariancea.c \
		crowvariancea.c \
		ccolumnvariancea.c \
		zvariancea.c \
		zrowvariancea.c \
		zcolumnvariancea.c

HEAD	=	../includes/variance.h

####
# Checking Part
####

check_INCLUDES	=	-I $(top_builddir)/src/c/statisticsFunctions/includes \
			-I $(top_builddir)/src/c/elementaryFunctions/includes \
			-I $(top_builddir)/src/c/matrixOperations/includes \
			-I $(top_builddir)/src/c/operations/includes \
			-I $(top_builddir)/src/c/type

check_LDADD	=	$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/c/operations/subtraction/libSubtraction.la \
			$(top_builddir)/src/fortran/lapack/libscilapack.la \
			$(top_builddir)/src/c/statisticsFunctions/variance/libVariance.la \
			$(top_builddir)/src/c/statisticsFunctions/sum/libSum.la \
			$(top_builddir)/src/c/statisticsFunctions/mean/libMean.la \
			$(top_builddir)/src/c/elementaryFunctions/pow/libPow.la \
			$(top_builddir)/src/c/elementaryFunctions/log/libLog.la \
			$(top_builddir)/src/c/elementaryFunctions/exp/libExp.la \
			$(top_builddir)/src/c/elementaryFunctions/cos/libCos.la \
			$(top_builddir)/src/c/elementaryFunctions/sin/libSin.la \
			$(top_builddir)/src/c/elementaryFunctions/cosh/libCosh.la \
			$(top_builddir)/src/c/elementaryFunctions/sinh/libSinh.la \
			$(top_builddir)/src/c/elementaryFunctions/sqrt/libSqrt.la \
			$(top_builddir)/src/c/elementaryFunctions/log1p/libLog1p.la \
			$(top_builddir)/src/c/elementaryFunctions/lnp1m1/libLnp1m1.la \
			$(top_builddir)/src/c/auxiliaryFunctions/pythag/libPythag.la \
			$(top_builddir)/src/c/auxiliaryFunctions/abs/libAbs.la \
			$(top_builddir)/src/c/matrixOperations/transpose/libMatrixTranspose.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			$(top_builddir)/src/c/operations/division/libDivision.la \
			$(top_builddir)/src/c/auxiliaryFunctions/conj/libConj.la \
			@LIBMATH@

check_PROGRAMS	= 	testFloatVarianceNono testDoubleVarianceNono

TESTS		=	testFloatVarianceNono testDoubleVarianceNono

#
# -*- Variance Tests -*-
#
testFloatVarianceNono_SOURCES = testFloatVarianceNono.c
testFloatVarianceNono_CFLAGS =	$(check_INCLUDES)
testFloatVarianceNono_LDADD =	$(check_LDADD)

testDoubleVarianceNono_SOURCES =testDoubleVarianceNono.c
testDoubleVarianceNono_CFLAGS =	$(check_INCLUDES)
testDoubleVarianceNono_LDADD =	$(check_LDADD)

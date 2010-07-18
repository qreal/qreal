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

libFrexp_la_CFLAGS =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/includes \
			-I $(top_builddir)/src/c/operations/includes \
                    	-I $(top_builddir)/src/c/elementaryFunctions/includes \
                    	-I $(top_builddir)/src/c/auxiliaryFunctions/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES = libFrexp.la

HEAD = ../includes/frexp.h

libFrexp_la_SOURCES = 	$(HEAD) \
			sfrexps.c \
			dfrexps.c

check_PROGRAMS = 	testFrexp

check_LDADD =		$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/c/auxiliaryFunctions/abs/libAbs.la \
			$(top_builddir)/src/c/auxiliaryFunctions/sign/libSign.la \
			$(top_builddir)/src/c/auxiliaryFunctions/pythag/libPythag.la \
			$(top_builddir)/src/c/auxiliaryFunctions/frexp/libFrexp.la \
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
			$(top_builddir)/src/c/elementaryFunctions/pow/libPow.la \
			$(top_builddir)/src/fortran/lapack/libscilapack.la \
			$(top_builddir)/src/fortran/blas/libsciblas.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			libFrexp.la

check_INCLUDES =	-I $(top_builddir)/src/c/type \
	                -I $(top_builddir)/includes \
			-I $(top_builddir)/src/c/operations/includes \
        	        -I $(top_builddir)/src/c/elementaryFunctions/includes \
                	-I $(top_builddir)/src/c/auxiliaryFunctions/includes



testFrexp_SOURCES = testFrexp.c
testFrexp_LDADD = $(check_LDADD)
testFrexp_CFLAGS = $(check_INCLUDES)

TESTS =		testFrexp

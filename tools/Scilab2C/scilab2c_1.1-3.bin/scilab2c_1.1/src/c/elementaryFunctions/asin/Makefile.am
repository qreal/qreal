##
##  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
##  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
##
##  This file must be used under the terms of the CeCILL.
##  This source file is licensed as described in the file COPYING, which
##  you should have received as part of this distribution.  The terms
##  are also available at
##  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
##
##

libAsin_la_CFLAGS =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/elementaryFunctions/includes \
			-I $(top_builddir)/src/c/auxiliaryFunctions/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libAsin.la

libAsin_la_SOURCES =	$(HEAD) $(SRC)

SRC	=	sasins.c \
		dasins.c \
		casins.c \
		zasins.c \
		sasina.c \
		dasina.c \
		casina.c \
		zasina.c

HEAD	=	../includes/asin.h


####
# Checking Part
####

check_INCLUDES	=	-I $(top_builddir)/src/c/elementaryFunctions/includes \
			-I $(top_builddir)/src/c/auxiliaryFunctions/includes \
			-I $(top_builddir)/src/c/type

check_LDADD	=	$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/fortran/lapack/libscilapack.la \
			$(top_builddir)/src/fortran/blas/libsciblas.la \
			$(top_builddir)/src/c/elementaryFunctions/asin/libAsin.la \
			$(top_builddir)/src/c/elementaryFunctions/sqrt/libSqrt.la \
			$(top_builddir)/src/c/elementaryFunctions/log/libLog.la \
			$(top_builddir)/src/c/elementaryFunctions/log1p/libLog1p.la \
			$(top_builddir)/src/c/elementaryFunctions/atan/libAtan.la \
			$(top_builddir)/src/c/elementaryFunctions/lnp1m1/libLnp1m1.la \
			$(top_builddir)/src/c/auxiliaryFunctions/abs/libAbs.la \
			$(top_builddir)/src/c/auxiliaryFunctions/pythag/libPythag.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			@LIBMATH@

check_PROGRAMS	= 	testFloatAsin testDoubleAsin

TESTS		=	testFloatAsin testDoubleAsin

#
# -*- ArcSine Tests -*-
#
testFloatAsin_SOURCES =	testAsin.h testFloatAsin.c
testFloatAsin_CFLAGS =	$(check_INCLUDES)
testFloatAsin_LDADD =	$(check_LDADD)

testDoubleAsin_SOURCES = testAsin.h testDoubleAsin.c
testDoubleAsin_CFLAGS =	$(check_INCLUDES)
testDoubleAsin_LDADD =	$(check_LDADD)

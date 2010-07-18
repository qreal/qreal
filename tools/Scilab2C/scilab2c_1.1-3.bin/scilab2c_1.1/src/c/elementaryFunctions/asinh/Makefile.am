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

libAsinh_la_CFLAGS =	-I ../../type \
			-I ../includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libAsinh.la

libAsinh_la_SOURCES =	$(HEAD) $(SRC)

SRC	=	sasinhs.c \
		dasinhs.c \
		casinhs.c \
		zasinhs.c \
		sasinha.c \
		dasinha.c \
		casinha.c \
		zasinha.c

HEAD	=	../includes/asinh.h

####
# Checking Part
####

check_INCLUDES	=	-I $(top_builddir)/src/c/elementaryFunctions/includes \
			-I $(top_builddir)/src/c/type

check_LDADD	=	$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/fortran/lapack/libscilapack.la \
			$(top_builddir)/src/c/elementaryFunctions/asinh/libAsinh.la \
			$(top_builddir)/src/c/elementaryFunctions/asin/libAsin.la \
			$(top_builddir)/src/c/elementaryFunctions/log1p/libLog1p.la \
			$(top_builddir)/src/c/elementaryFunctions/log/libLog.la \
			$(top_builddir)/src/c/elementaryFunctions/atan/libAtan.la \
			$(top_builddir)/src/c/elementaryFunctions/sqrt/libSqrt.la \
			$(top_builddir)/src/c/elementaryFunctions/lnp1m1/libLnp1m1.la \
			$(top_builddir)/src/c/auxiliaryFunctions/pythag/libPythag.la \
			$(top_builddir)/src/c/auxiliaryFunctions/abs/libAbs.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			@LIBMATH@

check_PROGRAMS	= 	testFloatAsinh testDoubleAsinh

TESTS		=	testFloatAsinh testDoubleAsinh

#
# -*- Hyperbolic ArcSine Tests -*-
#
testFloatAsinh_SOURCES = 	testAsinh.h testFloatAsinh.c
testFloatAsinh_CFLAGS =		$(check_INCLUDES)
testFloatAsinh_LDADD =		$(check_LDADD)

testDoubleAsinh_SOURCES = 	testAsinh.h testDoubleAsinh.c
testDoubleAsinh_CFLAGS =	$(check_INCLUDES)
testDoubleAsinh_LDADD =		$(check_LDADD)

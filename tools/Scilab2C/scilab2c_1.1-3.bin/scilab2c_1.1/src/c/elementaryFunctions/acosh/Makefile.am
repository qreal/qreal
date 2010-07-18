##
##  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
##  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
##
##  This file must be used under the terms of the CeCILL.
##  This source file is licensed as described in the file COPYING, which
##  you should have received as part of this distribution.  The terms
##  are also available at
##  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
##
##

libAcosh_la_CFLAGS =	-I ../../type \
			-I $(top_builddir)/src/c/auxiliaryFunctions/includes \
			-I ../includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libAcosh.la

libAcosh_la_SOURCES =	$(HEAD) $(SRC)

SRC	=	sacoshs.c \
		dacoshs.c \
		cacoshs.c \
		zacoshs.c \
		sacosha.c \
		dacosha.c \
		cacosha.c \
		zacosha.c

HEAD	=	../includes/acosh.h

####
# Checking Part
####

check_INCLUDES	=	-I $(top_builddir)/src/c/elementaryFunctions/includes \
			-I $(top_builddir)/src/c/auxiliaryFunctions/includes \
			-I $(top_builddir)/src/c/type

check_LDADD	  =	$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/fortran/lapack/libscilapack.la \
			$(top_builddir)/src/c/elementaryFunctions/acos/libAcos.la \
			$(top_builddir)/src/c/elementaryFunctions/acosh/libAcosh.la \
			$(top_builddir)/src/c/elementaryFunctions/atan/libAtan.la \
			$(top_builddir)/src/c/elementaryFunctions/log/libLog.la \
			$(top_builddir)/src/c/elementaryFunctions/log1p/libLog1p.la \
			$(top_builddir)/src/c/elementaryFunctions/sqrt/libSqrt.la \
			$(top_builddir)/src/c/elementaryFunctions/lnp1m1/libLnp1m1.la \
			$(top_builddir)/src/c/auxiliaryFunctions/abs/libAbs.la \
			$(top_builddir)/src/c/auxiliaryFunctions/pythag/libPythag.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			$(top_builddir)/src/c/auxiliaryFunctions/isnan/libIsnan.la \
			@LIBMATH@

check_PROGRAMS	= 	testFloatAcosh testDoubleAcosh

TESTS		=	testFloatAcosh testDoubleAcosh

#
# -*- Hyperbolic Acosine Tests -*-
#
testFloatAcosh_SOURCES = 	testAcosh.h testFloatAcosh.c
testFloatAcosh_CFLAGS  =	$(check_INCLUDES)
testFloatAcosh_LDADD =		$(check_LDADD)

testDoubleAcosh_SOURCES = 	testAcosh.h testDoubleAcosh.c
testDoubleAcosh_CFLAGS  =	$(check_INCLUDES)
testDoubleAcosh_LDADD =		$(check_LDADD)

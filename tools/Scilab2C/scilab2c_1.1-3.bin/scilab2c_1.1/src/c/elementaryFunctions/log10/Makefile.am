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

libLog10_la_CFLAGS =	-I ../../type \
			-I ../includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libLog10.la

libLog10_la_SOURCES =	$(HEAD) $(SRC)

SRC	=	slog10s.c \
		dlog10s.c \
		clog10s.c \
		zlog10s.c \
		slog10a.c \
		dlog10a.c \
		clog10a.c \
		zlog10a.c

HEAD	=	../includes/log10.h

####
# Checking Part
####

check_INCLUDES	=	-I $(top_builddir)/src/c/elementaryFunctions/includes \
			-I $(top_builddir)/src/c/type

check_LDADD	=	$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/fortran/lapack/libscilapack.la \
			$(top_builddir)/src/c/elementaryFunctions/log/libLog.la \
			$(top_builddir)/src/c/elementaryFunctions/log10/libLog10.la \
			$(top_builddir)/src/c/elementaryFunctions/log1p/libLog1p.la \
			$(top_builddir)/src/c/elementaryFunctions/lnp1m1/libLnp1m1.la \
			$(top_builddir)/src/c/elementaryFunctions/sqrt/libSqrt.la \
			$(top_builddir)/src/c/auxiliaryFunctions/abs/libAbs.la \
			$(top_builddir)/src/c/auxiliaryFunctions/pythag/libPythag.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			@LIBMATH@

check_PROGRAMS	= 	testFloatLog10 testDoubleLog10

TESTS		=	testFloatLog10 testDoubleLog10

#
# -*- 10-Base Logarithm Tests -*-
#
testFloatLog10_SOURCES = 	testLog10.h testFloatLog10.c
testFloatLog10_CFLAGS =		$(check_INCLUDES)
testFloatLog10_LDADD =		$(check_LDADD)

testDoubleLog10_SOURCES = 	testLog10.h testDoubleLog10.c
testDoubleLog10_CFLAGS =	$(check_INCLUDES)
testDoubleLog10_LDADD =		$(check_LDADD)

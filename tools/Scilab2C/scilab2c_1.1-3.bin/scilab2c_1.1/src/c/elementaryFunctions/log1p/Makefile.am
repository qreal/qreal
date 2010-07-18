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

libLog1p_la_CFLAGS =	-I ../../type \
			-I ../includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libLog1p.la

libLog1p_la_SOURCES =	$(HEAD) $(SRC)

SRC	=	slog1ps.c \
		dlog1ps.c \
		clog1ps.c \
		zlog1ps.c \
		slog1pa.c \
		dlog1pa.c \
		clog1pa.c \
		zlog1pa.c

HEAD	=	../includes/log1p.h

####
# Checking Part
####

check_INCLUDES	=	-I $(top_builddir)/src/c/elementaryFunctions/includes \
			-I $(top_builddir)/src/c/type

check_LDADD	=	$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/fortran/lapack/libscilapack.la \
			$(top_builddir)/src/c/elementaryFunctions/log1p/libLog1p.la \
			$(top_builddir)/src/c/elementaryFunctions/lnp1m1/libLnp1m1.la \
			$(top_builddir)/src/c/auxiliaryFunctions/pythag/libPythag.la \
			$(top_builddir)/src/c/elementaryFunctions/log/libLog.la \
			$(top_builddir)/src/c/elementaryFunctions/sqrt/libSqrt.la \
			$(top_builddir)/src/c/auxiliaryFunctions/abs/libAbs.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			@LIBMATH@

check_PROGRAMS	= 	testFloatLog1p testDoubleLog1p

TESTS		=	testFloatLog1p testDoubleLog1p

#
# -*- Log1p Tests -*-
#
testFloatLog1p_SOURCES = 	testLog1p.h testFloatLog1p.c
testFloatLog1p_CFLAGS =		$(check_INCLUDES)
testFloatLog1p_LDADD	=	$(check_LDADD)

testDoubleLog1p_SOURCES = 	testLog1p.h testDoubleLog1p.c
testDoubleLog1p_CFLAGS =	$(check_INCLUDES)
testDoubleLog1p_LDADD	=	$(check_LDADD)

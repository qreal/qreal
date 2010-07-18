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

libCosh_la_CFLAGS =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/auxiliaryFunctions/includes \
			-I $(top_builddir)/src/c/elementaryFunctions/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libCosh.la

libCosh_la_SOURCES =	$(HEAD) $(SRC)

SRC	=	scoshs.c \
		dcoshs.c \
		ccoshs.c \
		zcoshs.c \
		scosha.c \
		dcosha.c \
		ccosha.c \
		zcosha.c

HEAD	=	../includes/cosh.h

####
# Checking Part
####

check_INCLUDES	=	-I $(top_builddir)/src/c/elementaryFunctions/includes \
			-I $(top_builddir)/src/c/type

check_LDADD	=	$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/fortran/lapack/libscilapack.la \
			$(top_builddir)/src/c/elementaryFunctions/cosh/libCosh.la \
			$(top_builddir)/src/c/elementaryFunctions/cos/libCos.la \
			$(top_builddir)/src/c/elementaryFunctions/sinh/libSinh.la \
			$(top_builddir)/src/c/elementaryFunctions/sin/libSin.la \
			$(top_builddir)/src/c/elementaryFunctions/exp/libExp.la \
			$(top_builddir)/src/c/elementaryFunctions/sqrt/libSqrt.la \
			$(top_builddir)/src/c/auxiliaryFunctions/abs/libAbs.la \
			$(top_builddir)/src/c/auxiliaryFunctions/pythag/libPythag.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			@LIBMATH@

check_PROGRAMS	= 	testFloatCosh testDoubleCosh

TESTS		=	testFloatCosh testDoubleCosh

#
# -*- Hyperbolic Cosine Tests -*-
#
testFloatCosh_SOURCES = 	testCosh.h testFloatCosh.c
testFloatCosh_CFLAGS	=	$(check_INCLUDES)
testFloatCosh_LDADD	=	$(check_LDADD)

testDoubleCosh_SOURCES = 	testCosh.h testDoubleCosh.c
testDoubleCosh_CFLAGS	=	$(check_INCLUDES)
testDoubleCosh_LDADD	=	$(check_LDADD)

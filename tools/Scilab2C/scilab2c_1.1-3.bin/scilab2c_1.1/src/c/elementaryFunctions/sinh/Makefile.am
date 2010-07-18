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

libSinh_la_CFLAGS =	-I ../../type \
			-I ../includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libSinh.la

libSinh_la_SOURCES =	$(HEAD) $(SRC)

SRC	=	ssinhs.c \
		dsinhs.c \
		csinhs.c \
		zsinhs.c \
		ssinha.c \
		dsinha.c \
		csinha.c \
		zsinha.c

HEAD	=	../includes/sinh.h

####
# Checking Part
####

check_INCLUDES	=	-I $(top_builddir)/src/c/elementaryFunctions/includes \
			-I $(top_builddir)/src/c/type

check_LDADD	=	$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/fortran/lapack/libscilapack.la \
			$(top_builddir)/src/c/elementaryFunctions/sinh/libSinh.la \
			$(top_builddir)/src/c/elementaryFunctions/sin/libSin.la \
			$(top_builddir)/src/c/elementaryFunctions/cosh/libCosh.la \
			$(top_builddir)/src/c/elementaryFunctions/cos/libCos.la \
			$(top_builddir)/src/c/elementaryFunctions/exp/libExp.la \
			$(top_builddir)/src/c/elementaryFunctions/sqrt/libSqrt.la \
			$(top_builddir)/src/c/auxiliaryFunctions/abs/libAbs.la \
			$(top_builddir)/src/c/auxiliaryFunctions/pythag/libPythag.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			@LIBMATH@

check_PROGRAMS	= 	testFloatSinh testDoubleSinh

TESTS		=	testFloatSinh testDoubleSinh

#
# -*- Hyperbolic Sine Tests -*-
#
testFloatSinh_SOURCES = 	testSinh.h testFloatSinh.c
testFloatSinh_CFLAGS	=	$(check_INCLUDES)
testFloatSinh_LDADD	=	$(check_LDADD)

testDoubleSinh_SOURCES = 	testSinh.h testDoubleSinh.c
testDoubleSinh_CFLAGS	=	$(check_INCLUDES)
testDoubleSinh_LDADD	=	$(check_LDADD)

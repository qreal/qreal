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

libAtanh_la_CFLAGS =	-I ../../type \
			-I ../includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libAtanh.la

libAtanh_la_SOURCES =	$(HEAD) $(SRC)

SRC	=	satanhs.c \
		datanhs.c \
		catanhs.c \
		zatanhs.c \
		satanha.c \
		datanha.c \
		catanha.c \
		zatanha.c

HEAD	=	../includes/atanh.h

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
			$(top_builddir)/src/c/elementaryFunctions/atan/libAtan.la \
			$(top_builddir)/src/c/elementaryFunctions/sqrt/libSqrt.la \
			$(top_builddir)/src/c/elementaryFunctions/atanh/libAtanh.la \
			$(top_builddir)/src/c/elementaryFunctions/lnp1m1/libLnp1m1.la \
			$(top_builddir)/src/c/auxiliaryFunctions/abs/libAbs.la \
			$(top_builddir)/src/c/auxiliaryFunctions/pythag/libPythag.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			$(top_builddir)/src/c/auxiliaryFunctions/isnan/libIsnan.la \
			@LIBMATH@

check_PROGRAMS	= 	testFloatAtanh testDoubleAtanh

TESTS		=	testFloatAtanh testDoubleAtanh

#
# -*- Hyperbolic ArcTangeant Tests -*-
#
testFloatAtanh_SOURCES = 	testAtanh.h testFloatAtanh.c
testFloatAtanh_CFLAGS =		$(check_INCLUDES)
testFloatAtanh_LDADD =		$(check_LDADD)

testDoubleAtanh_SOURCES = 	testAtanh.h testDoubleAtanh.c
testDoubleAtanh_CFLAGS =	$(check_INCLUDES)
testDoubleAtanh_LDADD =		$(check_LDADD)

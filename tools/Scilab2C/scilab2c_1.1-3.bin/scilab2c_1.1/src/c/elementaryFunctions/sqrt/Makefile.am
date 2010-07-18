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

libSqrt_la_CFLAGS =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/elementaryFunctions/includes \
			-I $(top_builddir)/src/c/auxiliaryFunctions/includes \
			-I $(top_builddir)/src/c/operations/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libSqrt.la

libSqrt_la_SOURCES =	$(HEAD) $(SRC)

SRC	=	ssqrts.c \
		dsqrts.c \
		csqrts.c \
		zsqrts.c \
		ssqrta.c \
		dsqrta.c \
		csqrta.c \
		zsqrta.c

HEAD	=	../includes/sqrt.h

####
# Checking Part
####

check_INCLUDES	=	-I $(top_builddir)/src/c/elementaryFunctions/includes \
			-I $(top_builddir)/src/c/type

check_LDADD	=	$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/fortran/lapack/libscilapack.la \
			$(top_builddir)/src/c/elementaryFunctions/sqrt/libSqrt.la \
			$(top_builddir)/src/c/auxiliaryFunctions/abs/libAbs.la \
			$(top_builddir)/src/c/auxiliaryFunctions/pythag/libPythag.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			@LIBMATH@

check_PROGRAMS	= 	testFloatSqrt testDoubleSqrt

TESTS		=	testFloatSqrt testDoubleSqrt

#
# -*- Square Root Tests -*-
#
testFloatSqrt_SOURCES = 	testSqrt.h testFloatSqrt.c
testFloatSqrt_CFLAGS =		$(check_INCLUDES)
testFloatSqrt_LDADD =		$(check_LDADD)

testDoubleSqrt_SOURCES = 	testSqrt.h testDoubleSqrt.c
testDoubleSqrt_CFLAGS =		$(check_INCLUDES)
testDoubleSqrt_LDADD =		$(check_LDADD)

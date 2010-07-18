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



libCos_la_CFLAGS =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/elementaryFunctions/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libCos.la

libCos_la_SOURCES =  $(HEAD) $(SRC)

HEAD =	../includes/cos.h

SRC	=	scoss.c \
		dcoss.c \
		ccoss.c \
		zcoss.c \
		scosa.c \
		dcosa.c \
		ccosa.c \
		zcosa.c

####
# Checking Part
####

check_INCLUDES	=	-I $(top_builddir)/src/c/elementaryFunctions/includes \
			-I $(top_builddir)/src/c/type

check_LDADD	=	$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/fortran/lapack/libscilapack.la \
			$(top_builddir)/src/c/elementaryFunctions/cos/libCos.la \
			$(top_builddir)/src/c/elementaryFunctions/sin/libSin.la \
			$(top_builddir)/src/c/elementaryFunctions/cosh/libCosh.la \
			$(top_builddir)/src/c/elementaryFunctions/sinh/libSinh.la \
			$(top_builddir)/src/c/elementaryFunctions/exp/libExp.la \
			$(top_builddir)/src/c/elementaryFunctions/sqrt/libSqrt.la \
			$(top_builddir)/src/c/auxiliaryFunctions/abs/libAbs.la \
			$(top_builddir)/src/c/auxiliaryFunctions/pythag/libPythag.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			@LIBMATH@

check_PROGRAMS	= 	testFloatCos testDoubleCos

TESTS		=	testFloatCos testDoubleCos

#
# -*- Cosine Tests -*-
#
testFloatCos_SOURCES = 	../../includes/constant.h testCos.h testFloatCos.c
testFloatCos_CFLAGS =	$(check_INCLUDES)
testFloatCos_LDADD =	$(check_LDADD)

testDoubleCos_SOURCES =	../../includes/constant.h testCos.h testDoubleCos.c
testDoubleCos_CFLAGS =	$(check_INCLUDES)
testDoubleCos_LDADD =	$(check_LDADD)

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

libAcos_la_CFLAGS =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/elementaryFunctions/includes \
			-I $(top_builddir)/src/c/auxiliaryFunctions/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libAcos.la

libAcos_la_SOURCES =	$(HEAD) $(SRC)

SRC	=	sacoss.c \
		dacoss.c \
		cacoss.c \
		zacoss.c \
		sacosa.c \
		dacosa.c \
		cacosa.c \
		zacosa.c

HEAD	=	../includes/acos.h \
		../../auxiliaryFunctions/includes/min.h \
		../../auxiliaryFunctions/includes/max.h

####
# Checking Part
####

check_INCLUDES	=	-I $(top_builddir)/src/c/elementaryFunctions/includes \
			-I $(top_builddir)/includes \
			-I $(top_builddir)/src/c/type

check_LDADD	=	$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/fortran/lapack/libscilapack.la \
			$(top_builddir)/src/c/elementaryFunctions/acos/libAcos.la \
			$(top_builddir)/src/c/elementaryFunctions/atan/libAtan.la \
			$(top_builddir)/src/c/elementaryFunctions/log/libLog.la \
			$(top_builddir)/src/c/elementaryFunctions/log1p/libLog1p.la \
			$(top_builddir)/src/c/elementaryFunctions/sqrt/libSqrt.la \
			$(top_builddir)/src/c/elementaryFunctions/lnp1m1/libLnp1m1.la \
			$(top_builddir)/src/c/auxiliaryFunctions/abs/libAbs.la \
			$(top_builddir)/src/c/auxiliaryFunctions/pythag/libPythag.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			@LIBMATH@

check_PROGRAMS	= 	testRealsAcos testCplxAcos

TESTS		=	testRealsAcos testCplxAcos

#
# -*- Acosine Tests -*-
#
testRealsAcos_SOURCES = 	testAcos.h testRealsAcos.c
testRealsAcos_CFLAGS	=	$(check_INCLUDES)
testRealsAcos_LDADD	=	$(check_LDADD)

testCplxAcos_SOURCES = 	testAcos.h testCplxAcos.c
testCplxAcos_CFLAGS	=	$(check_INCLUDES)
testCplxAcos_LDADD	=	$(check_LDADD)


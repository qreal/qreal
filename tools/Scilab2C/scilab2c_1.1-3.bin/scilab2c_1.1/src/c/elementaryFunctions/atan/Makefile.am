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

libAtan_la_CFLAGS =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/elementaryFunctions/includes \
			-I $(top_builddir)/src/c/auxiliaryFunctions/includes


instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libAtan.la

libAtan_la_SOURCES =	$(HEAD) $(SRC)

SRC	=	satans.c \
		datans.c \
		catans.c \
		zatans.c \
		satana.c \
		datana.c \
		catana.c \
		zatana.c

HEAD	=	../includes/atan.h

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
			$(top_builddir)/src/c/elementaryFunctions/atan/libAtan.la \
			$(top_builddir)/src/c/elementaryFunctions/lnp1m1/libLnp1m1.la \
			$(top_builddir)/src/c/auxiliaryFunctions/abs/libAbs.la \
			$(top_builddir)/src/c/auxiliaryFunctions/pythag/libPythag.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			@LIBMATH@

check_PROGRAMS	= 	testFloatAtan testDoubleAtan

TESTS		=	testFloatAtan testDoubleAtan

#
# -*- Arc tangeant Tests -*-
#
testFloatAtan_SOURCES = 	testAtan.h testFloatAtan.c
testFloatAtan_CFLAGS	=	$(check_INCLUDES)
testFloatAtan_LDADD	=	$(check_LDADD)

testDoubleAtan_SOURCES = 	testAtan.h testDoubleAtan.c
testDoubleAtan_CFLAGS	=	$(check_INCLUDES)
testDoubleAtan_LDADD	=	$(check_LDADD)

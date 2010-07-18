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

libAbs_la_CFLAGS =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/auxiliaryFunctions/includes \
			-I $(top_builddir)/src/c/elementaryFunctions/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libAbs.la

libAbs_la_SOURCES =	$(HEAD) $(SRC)

SRC	=	sabss.c \
		dabss.c \
		cabss.c \
		zabss.c \
		sabsa.c \
		dabsa.c \
		cabsa.c \
		zabsa.c

HEAD	=	../includes/abs.h

####
# Checking Part
####

check_INCLUDES	=	-I $(top_builddir)/src/c/auxiliaryFunctions/includes \
			-I $(top_builddir)/src/c/elementaryFunctions/includes \
			-I $(top_builddir)/src/c/type

check_LDADD	=	$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/fortran/lapack/libscilapack.la \
			$(top_builddir)/src/c/auxiliaryFunctions/abs/libAbs.la \
			$(top_builddir)/src/c/elementaryFunctions/sqrt/libSqrt.la \
			$(top_builddir)/src/c/auxiliaryFunctions/pythag/libPythag.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			@LIBMATH@

check_PROGRAMS	= 	testFloatAbs testDoubleAbs

TESTS		=	testFloatAbs testDoubleAbs

#
# -*- Abs Tests -*-
#
testFloatAbs_SOURCES = 	testAbs.h testFloatAbs.c
testFloatAbs_CFLAGS =	$(check_INCLUDES)
testFloatAbs_LDADD =	$(check_LDADD)

testDoubleAbs_SOURCES =	testAbs.h testDoubleAbs.c
testDoubleAbs_CFLAGS =	$(check_INCLUDES)
testDoubleAbs_LDADD =	$(check_LDADD)

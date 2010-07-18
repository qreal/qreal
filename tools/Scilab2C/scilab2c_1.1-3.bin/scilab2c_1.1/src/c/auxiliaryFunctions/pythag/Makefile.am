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

libPythag_la_CFLAGS =	-I  $(top_builddir)/src/c/type \
			-I  $(top_builddir)/src/c/auxiliaryFunctions/includes \
			-I  $(top_builddir)/src/c/elementaryFunctions/includes \
			-I  $(top_builddir)/src/c/operations/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libPythag.la

libPythag_la_SOURCES =	$(HEAD) $(SRC)

SRC	=	spythags.c \
		dpythags.c \
		cpythags.c \
		zpythags.c

HEAD	=	../includes/pythag.h

####
# Checking Part
####

check_INCLUDES	=	-I $(top_builddir)/src/c/auxiliaryFunctions/includes \
			-I $(top_builddir)/src/c/elementaryFunctions/includes \
			-I  $(top_builddir)/src/c/operations/includes \
			-I $(top_builddir)/src/c/type

check_PROGRAMS	= 	testPythag

TESTS		=	testPythag

#
# -*- Pythag Tests -*-
#
testPythag_SOURCES = 	testPythag.h testPythag.c
testPythag_CFLAGS  =	$(check_INCLUDES)
testPythag_LDADD   =	$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/fortran/lapack/libscilapack.la \
			$(top_builddir)/src/c/auxiliaryFunctions/pythag/libPythag.la \
			$(top_builddir)/src/c/elementaryFunctions/sqrt/libSqrt.la \
			$(top_builddir)/src/c/auxiliaryFunctions/abs/libAbs.la \
			$(top_builddir)/src/c/auxiliaryFunctions/pythag/libPythag.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			@LIBMATH@

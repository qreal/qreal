##
##  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
##  Copyright (C) 2008-2008 - INRIA - Allan SIMON
##
##  This file must be used under the terms of the CeCILL.
##  This source file is licensed as described in the file COPYING, which
##  you should have received as part of this distribution.  The terms
##  are also available at
##  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
##
##

libMatrixEye_la_CFLAGS =	-I $(top_builddir)/src/c/type \
				-I $(top_builddir)/src/c/matrixOperations/includes \
				-I $(top_builddir)/includes


instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES = libMatrixEye.la

HEAD = ../includes/matrixEye.h

libMatrixEye_la_SOURCES = 	$(HEAD) \
				seyea.c \
				deyea.c \
				ceyea.c \
				zeyea.c



check_PROGRAMS = 	testMatrixEye

check_LDADD =		$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			libMatrixEye.la

check_INCLUDES =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/matrixOperations/includes \
			-I $(top_builddir)/src/c/includes

testMatrixEye_SOURCES = testMatrixEye.c
testMatrixEye_LDADD = $(check_LDADD)
testMatrixEye_CFLAGS = $(check_INCLUDES)

TESTS =		testMatrixEye

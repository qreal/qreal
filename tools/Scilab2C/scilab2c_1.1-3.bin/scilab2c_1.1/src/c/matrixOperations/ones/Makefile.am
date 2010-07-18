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

libMatrixOnes_la_CFLAGS =	-I $(top_builddir)/src/c/type \
				-I $(top_builddir)/src/c/matrixOperations/includes


instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES = libMatrixOnes.la

HEAD = ../includes/matrixOnes.h

libMatrixOnes_la_SOURCES = 	$(HEAD) \
				sonesa.c \
				donesa.c \
				conesa.c \
				zonesa.c

check_PROGRAMS = 	testMatrixOnes

check_LDADD =		$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			libMatrixOnes.la

check_INCLUDES =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/matrixOperations/includes

testMatrixOnes_SOURCES = testMatrixOnes.c
testMatrixOnes_LDADD = $(check_LDADD)
testMatrixOnes_CFLAGS = $(check_INCLUDES)

TESTS =		testMatrixOnes

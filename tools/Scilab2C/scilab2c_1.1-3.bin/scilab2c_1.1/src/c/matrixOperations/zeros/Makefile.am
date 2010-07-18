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

libMatrixZeros_la_CFLAGS =	-I $(top_builddir)/src/c/type \
				-I $(top_builddir)/src/c/matrixOperations/includes


instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES = libMatrixZeros.la

HEAD = ../includes/matrixZeros.h

libMatrixZeros_la_SOURCES = 	$(HEAD) \
				szerosa.c \
				dzerosa.c \
				czerosa.c \
				zzerosa.c

check_PROGRAMS = 	testMatrixZeros

check_LDADD =		$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			libMatrixZeros.la

check_INCLUDES =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/matrixOperations/includes

testMatrixZeros_SOURCES = testMatrixZeros.c
testMatrixZeros_LDADD = $(check_LDADD)
testMatrixZeros_CFLAGS = $(check_INCLUDES)

TESTS =		testMatrixZeros

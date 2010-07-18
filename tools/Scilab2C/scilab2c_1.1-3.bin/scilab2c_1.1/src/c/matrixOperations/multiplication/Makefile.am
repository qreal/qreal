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

libMatrixMultiplication_la_CFLAGS =	-I $(top_builddir)/src/c/type \
					-I $(top_builddir)/src/c/matrixOperations/includes \
					-I $(top_builddir)/src/c/operations/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES = libMatrixMultiplication.la

HEAD = ../includes/matrixMultiplication.h

libMatrixMultiplication_la_SOURCES = 	$(HEAD) \
					smulma.c \
					dmulma.c \
					cmulma.c \
					zmulma.c

check_PROGRAMS = 	testFloatMatrixMultiplication testDoubleMatrixMultiplication

check_LDADD =		$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/fortran/blas/libsciblas.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			libMatrixMultiplication.la

check_INCLUDES =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/matrixOperations/includes \
			-I $(top_builddir)/src/c/operations/includes

testFloatMatrixMultiplication_SOURCES = 	testFloatMatrixMultiplication.c 
testFloatMatrixMultiplication_LDADD = $(check_LDADD)
testFloatMatrixMultiplication_CFLAGS = $(check_INCLUDES)

testDoubleMatrixMultiplication_SOURCES = 	testDoubleMatrixMultiplication.c 
testDoubleMatrixMultiplication_LDADD = $(check_LDADD)
testDoubleMatrixMultiplication_CFLAGS = $(check_INCLUDES)
TESTS =		testFloatMatrixMultiplication testDoubleMatrixMultiplication

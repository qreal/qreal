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

libMultiplication_la_CFLAGS =	-I $(top_builddir)/src/c/type \
				-I $(top_builddir)/src/c/matrixOperations/includes \
				-I $(top_builddir)/src/c/operations/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES = libMultiplication.la

HEAD = ../includes/multiplication.h

libMultiplication_la_SOURCES = 	$(HEAD) \
				smuls.c \
				dmuls.c \
				cmuls.c \
				zmuls.c \
				smula.c \
				dmula.c \
				cmula.c \
				zmula.c \
				smulv.c \
				dmulv.c \
				cmulv.c \
				zmulv.c


check_PROGRAMS = 	testMultiplication

check_LDADD =		$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/matrixOperations/zeros/libMatrixZeros.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			libMultiplication.la

check_INCLUDES =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/matrixOperations/includes \
			-I $(top_builddir)/src/c/operations/includes

testMultiplication_SOURCES = testMultiplication.c
testMultiplication_LDADD = $(check_LDADD)
testMultiplication_CFLAGS = $(check_INCLUDES)

TESTS =		testMultiplication

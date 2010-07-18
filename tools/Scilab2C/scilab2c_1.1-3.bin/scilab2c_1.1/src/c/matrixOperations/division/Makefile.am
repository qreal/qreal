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

libMatrixDivision_la_CFLAGS =	-I $(top_builddir)/src/c/type \
				-I $(top_builddir)/src/c/matrixOperations/includes \
				-I $(top_builddir)/src/c/auxiliaryFunctions/includes \
				-I $(top_builddir)/src/c/operations/includes \
				-I $(top_builddir)/src/c/elementaryFunctions/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES = libMatrixDivision.la

HEAD = ../includes/matrixDivision.h

libMatrixDivision_la_SOURCES = 	$(HEAD) \
				srdivma.c \
				sldivma.c \
				drdivma.c \
				dldivma.c \
				zrdivma.c \
				zldivma.c \
				crdivma.c \
				cldivma.c

check_PROGRAMS = 	testMatrixRDivision \
			testMatrixLDivision

check_LDADD =		$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/fortran/lapack/libscilapack.la \
			$(top_builddir)/src/c/matrixOperations/transpose/libMatrixTranspose.la \
			$(top_builddir)/src/c/matrixOperations/inversion/libMatrixInversion.la \
			$(top_builddir)/src/c/matrixOperations/multiplication/libMatrixMultiplication.la \
			$(top_builddir)/src/c/matrixOperations/cat/libMatrixConcatenation.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			$(top_builddir)/src/c/auxiliaryFunctions/conj/libConj.la \
			libMatrixDivision.la

check_INCLUDES =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/matrixOperations/includes \
			-I $(top_builddir)/src/c/auxiliaryFunctions/includes \
			-I $(top_builddir)/src/c/operations/includes \
			-I $(top_builddir)/src/c/elementaryFunctions/includes

ztestMatrixLDivision_SOURCES =    testMatrixLDivision.c
testMatrixLDivision_LDADD = $(check_LDADD)
testMatrixLDivision_CFLAGS = $(check_INCLUDES)

testMatrixRDivision_SOURCES =    testMatrixRDivision.c
testMatrixRDivision_LDADD = $(check_LDADD)
testMatrixRDivision_CFLAGS = $(check_INCLUDES)

TESTS =		testMatrixRDivision \
		testMatrixLDivision

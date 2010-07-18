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

libMatrixTranspose_la_CFLAGS =	-I $(top_builddir)/src/c/type \
				-I $(top_builddir)/src/c/matrixOperations/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES = libMatrixTranspose.la

HEAD = ../includes/matrixTranspose.h

libMatrixTranspose_la_SOURCES = 	$(HEAD) \
					stransposea.c \
					dtransposea.c \
					ctransposea.c \
					ztransposea.c

check_PROGRAMS = 	testMatrixTranspose

check_LDADD =		$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/fortran/blas/libsciblas.la \
			libMatrixTranspose.la

check_INCLUDES =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/matrixOperations/includes

testMatrixTranspose_SOURCES = 	testMatrixTranspose.c
testMatrixTranspose_LDADD = $(check_LDADD)
testMatrixTranspose_CFLAGS = $(check_INCLUDES)

TESTS =		testMatrixTranspose

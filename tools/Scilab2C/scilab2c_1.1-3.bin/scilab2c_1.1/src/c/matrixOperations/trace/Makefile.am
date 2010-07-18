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

libMatrixTrace_la_CFLAGS =	-I $(top_builddir)/src/c/type \
				-I $(top_builddir)/src/c/matrixOperations/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES = libMatrixTrace.la

HEAD = ../includes/matrixTrace.h

libMatrixTrace_la_SOURCES = 	$(HEAD) \
				stracea.c \
				dtracea.c \
				ctracea.c \
				ztracea.c

check_PROGRAMS = 	testMatrixTrace

check_LDADD =		$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			libMatrixTrace.la

check_INCLUDES =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/matrixOperations/includes

testMatrixTrace_SOURCES = testMatrixTrace.c
testMatrixTrace_LDADD = $(check_LDADD)
testMatrixTrace_CFLAGS = $(check_INCLUDES)

TESTS =		testMatrixTrace

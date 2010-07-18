##
##  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
##  Copyright (C) 2008-2008 - INRIA - Arnaud TORSET
##
##  This file must be used under the terms of the CeCILL.
##  This source file is licensed as described in the file COPYING, which
##  you should have received as part of this distribution.  The terms
##  are also available at
##  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
##
##

libJmat_la_CFLAGS=	-I $(top_builddir)/src/c/matrixOperations/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES = libJmat.la

HEAD = ../includes/jmat.h

libJmat_la_SOURCES = 	$(HEAD) \
			sjmata.c \
			djmata.c

check_PROGRAMS = 	testJmat

check_LDADD =		libJmat.la

check_INCLUDES =	-I $(top_builddir)/src/c/matrixOperations/includes

testJmat_SOURCES = testJmat.c
testJmat_LDADD = $(check_LDADD)
testJmat_CFLAGS = $(check_INCLUDES)

TESTS =		testJmat

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

libHilbert_la_CFLAGS =	-I $(top_builddir)/src/c/type \
			-I ../includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES = libHilbert.la

HEAD = ../includes/hilbert.h

libHilbert_la_SOURCES = 	$(HEAD) \
				shilba.c \
				dhilba.c

check_PROGRAMS = 	testHilbert

check_LDADD =   libHilbert.la

check_INCLUDES =	-I $(top_builddir)/src/c/type \
			-I ../includes

testHilbert_SOURCES = testHilbert.c
testHilbert_LDADD = $(check_LDADD)
testHilbert_CFLAGS = $(check_INCLUDES)

TESTS =		testHilbert

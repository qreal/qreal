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

libSubtraction_la_CFLAGS =	-I $(top_builddir)/src/c/type \
				-I $(top_builddir)/src/c/operations/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES = libSubtraction.la

HEAD = ../includes/subtraction.h

libSubtraction_la_SOURCES = 	$(HEAD) \
				sdiffs.c \
				ddiffs.c \
				cdiffs.c \
				zdiffs.c \
				sdiffa.c \
				ddiffa.c \
				cdiffa.c \
				zdiffa.c

check_PROGRAMS = 	testSubtraction

check_LDADD =		$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			libSubtraction.la

check_INCLUDES =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/operations/includes

testSubtraction_SOURCES = testSubtraction.c
testSubtraction_LDADD = $(check_LDADD)
testSubtraction_CFLAGS = $(check_INCLUDES)

TESTS =		testSubtraction
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

libDivision_la_CFLAGS =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/auxiliaryFunctions/includes \
			-I $(top_builddir)/src/c/operations/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES = libDivision.la

HEAD = ../includes/division.h

libDivision_la_SOURCES = 	$(HEAD) \
				srdivs.c \
				drdivs.c \
				crdivs.c \
				zrdivs.c \
				sldivs.c \
				dldivs.c \
				cldivs.c \
				zldivs.c \
				srdiva.c \
				drdiva.c \
				crdiva.c \
				zrdiva.c \
				sldiva.c \
				dldiva.c \
				cldiva.c \
				zldiva.c

check_PROGRAMS = 	testLDivision \
			testRDivision

check_LDADD =		$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			$(top_builddir)/src/c/auxiliaryFunctions/conj/libConj.la \
			libDivision.la

check_INCLUDES =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/operations/includes


testLDivision_SOURCES = testLDivision.c
testLDivision_LDADD = $(check_LDADD)
testLDivision_CFLAGS = $(check_INCLUDES)


testRDivision_SOURCES = testRDivision.c
testRDivision_LDADD = $(check_LDADD)
testRDivision_CFLAGS = $(check_INCLUDES)

TESTS =		testLDivision \
		testRDivision

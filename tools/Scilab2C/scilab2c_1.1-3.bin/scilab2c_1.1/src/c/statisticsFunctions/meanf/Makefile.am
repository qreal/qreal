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

libMeanf_la_CFLAGS =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/operations/includes \
			-I ../includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libMeanf.la

libMeanf_la_SOURCES =	$(HEAD) $(SRC)

SRC	=	smeanfa.c \
		srowmeanfa.c \
		scolumnmeanfa.c \
		dmeanfa.c \
		drowmeanfa.c \
		dcolumnmeanfa.c \
		cmeanfa.c \
		crowmeanfa.c \
		ccolumnmeanfa.c \
		zmeanfa.c \
		zrowmeanfa.c \
		zcolumnmeanfa.c

HEAD	=	../includes/meanf.h

####
# Checking Part
####

check_INCLUDES	=	-I $(top_builddir)/src/c/statisticsFunctions/includes \
			-I $(top_builddir)/src/c/operations/includes \
			-I $(top_builddir)/src/c/type

check_LDADD	=	$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/c/statisticsFunctions/meanf/libMeanf.la \
			$(top_builddir)/src/c/statisticsFunctions/sum/libSum.la \
			$(top_builddir)/src/c/operations/division/libDivision.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			$(top_builddir)/src/c/auxiliaryFunctions/conj/libConj.la \
			@LIBMATH@

check_PROGRAMS	= 	testFloatMeanf testDoubleMeanf

TESTS		=	testFloatMeanf testDoubleMeanf

#
# -*- Meanf Tests -*-
#
testFloatMeanf_SOURCES = testFloatMeanf.c
testFloatMeanf_CFLAGS =	$(check_INCLUDES)
testFloatMeanf_LDADD =	$(check_LDADD)

testDoubleMeanf_SOURCES =testDoubleMeanf.c
testDoubleMeanf_CFLAGS =	$(check_INCLUDES)
testDoubleMeanf_LDADD =	$(check_LDADD)

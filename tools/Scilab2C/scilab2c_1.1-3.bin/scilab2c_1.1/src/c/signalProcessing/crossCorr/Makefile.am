##
##  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
##  Copyright (C) 2008 - INRIA - Arnaud TORSET
##
##  This file must be used under the terms of the CeCILL.
##  This source file is licensed as described in the file COPYING, which
##  you should have received as part of this distribution.  The terms
##  are also available at
##  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
##
##




libCrossCorr_la_CFLAGS =	-I $(top_builddir)/src/c/type \
				-I $(top_builddir)/src/c/auxiliaryFunctions/includes \
				-I $(top_builddir)/src/c/signalProcessing/includes


instdir = $(top_builddir)/lib


pkglib_LTLIBRARIES = libCrossCorr.la

HEAD = ../includes/crossCorr.h

libCrossCorr_la_SOURCES = 	$(HEAD) \
				scrossCorra.c \
				dcrossCorra.c \
				ccrossCorra.c \
				zcrossCorra.c


###############
#### Check ####
###############

check_LDADD =		$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/auxiliaryFunctions/conj/libConj.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/c/signalProcessing/conv2d/libConv2d.la \
			libCrossCorr.la



check_INCLUDES =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/auxiliaryFunctions/includes \
			-I $(top_builddir)/src/c/signalProcessing/includes

check_PROGRAMS = 	testFloatCrossCorr  testDoubleCrossCorr

TESTS =		testFloatCrossCorr       testDoubleCrossCorr

testDoubleCrossCorr_SOURCES = testDoubleCrossCorr.c
testDoubleCrossCorr_LDADD = $(check_LDADD)
testDoubleCrossCorr_CFLAGS = $(check_INCLUDES)

testFloatCrossCorr_SOURCES = testFloatCrossCorr.c
testFloatCrossCorr_LDADD = $(check_LDADD)
testFloatCrossCorr_CFLAGS = $(check_INCLUDES)




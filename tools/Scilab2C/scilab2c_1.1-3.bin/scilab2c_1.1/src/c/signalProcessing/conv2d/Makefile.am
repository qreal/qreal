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




libConv2d_la_CFLAGS =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/operations/includes \
			-I $(top_builddir)/src/c/signalProcessing/includes


instdir = $(top_builddir)/lib


pkglib_LTLIBRARIES = libConv2d.la

HEAD = ../includes/conv2d.h

libConv2d_la_SOURCES = 	$(HEAD) \
			sconv2da.c \
			dconv2da.c \
			cconv2da.c \
			zconv2da.c


###############
#### Check ####
###############

check_LDADD =		$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			libConv2d.la



check_INCLUDES =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/operations/includes \
			-I $(top_builddir)/src/c/signalProcessing/includes

check_PROGRAMS = 	testFloatConv2d       testDoubleConv2d

TESTS =		testFloatConv2d       testDoubleConv2d

testDoubleConv2d_SOURCES = testDoubleConv2d.c
testDoubleConv2d_LDADD = $(check_LDADD)
testDoubleConv2d_CFLAGS = $(check_INCLUDES)

testFloatConv2d_SOURCES = testFloatConv2d.c
testFloatConv2d_LDADD = $(check_LDADD)
testFloatConv2d_CFLAGS = $(check_INCLUDES)




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

libSum_la_CFLAGS =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/operations/includes \
			-I ../includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libSum.la

libSum_la_SOURCES =	$(HEAD) $(SRC)

SRC	=	ssuma.c \
		srowsuma.c \
		scolumnsuma.c \
		dsuma.c \
		drowsuma.c \
		dcolumnsuma.c \
		csuma.c \
		crowsuma.c \
		ccolumnsuma.c \
		zsuma.c \
		zrowsuma.c \
		zcolumnsuma.c

HEAD	=	../includes/sum.h

####
# Checking Part
####

check_INCLUDES	=	-I $(top_builddir)/src/c/statisticsFunctions/includes \
			-I $(top_builddir)/src/c/operations/includes \
			-I $(top_builddir)/src/c/type

check_LDADD	=	$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/c/statisticsFunctions/sum/libSum.la \
			@LIBMATH@

check_PROGRAMS	= 	testFloatSum testDoubleSum

TESTS		=	testFloatSum testDoubleSum

#
# -*- Sum Tests -*-
#
testFloatSum_SOURCES = testFloatSum.c
testFloatSum_CFLAGS =	$(check_INCLUDES)
testFloatSum_LDADD =	$(check_LDADD)

testDoubleSum_SOURCES =testDoubleSum.c
testDoubleSum_CFLAGS =	$(check_INCLUDES)
testDoubleSum_LDADD =	$(check_LDADD)
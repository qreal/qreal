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

libMin_la_CFLAGS =	-I $(top_builddir)/src/c/statisticsFunctions/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libMin.la

libMin_la_SOURCES =	$(HEAD) $(SRC)

SRC	=	smina.c \
		srowmina.c \
		scolumnmina.c \
		dmina.c \
		drowmina.c \
		dcolumnmina.c

HEAD	=	../includes/statMin.h

####
# Checking Part
####

check_INCLUDES	=	-I $(top_builddir)/src/c/statisticsFunctions/includes

check_LDADD	=	$(top_builddir)/src/c/statisticsFunctions/min/libMin.la

check_PROGRAMS	= 	testFloatMin testDoubleMin

TESTS		=	testFloatMin testDoubleMin

#
# -*- Min Tests -*-
#
testFloatMin_SOURCES = testFloatMin.c
testFloatMin_CFLAGS =	$(check_INCLUDES)
testFloatMin_LDADD =	$(check_LDADD)

testDoubleMin_SOURCES =testDoubleMin.c
testDoubleMin_CFLAGS =	$(check_INCLUDES)
testDoubleMin_LDADD =	$(check_LDADD)

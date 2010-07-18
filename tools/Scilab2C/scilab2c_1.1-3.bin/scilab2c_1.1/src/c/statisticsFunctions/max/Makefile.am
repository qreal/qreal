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

libMax_la_CFLAGS =	-I $(top_builddir)/src/c/statisticsFunctions/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libMax.la

libMax_la_SOURCES =	$(HEAD) $(SRC)

SRC	=	smaxa.c \
		srowmaxa.c \
		scolumnmaxa.c \
		dmaxa.c \
		drowmaxa.c \
		dcolumnmaxa.c

HEAD	=	../includes/statMax.h

####
# Checking Part
####

check_INCLUDES	=	-I $(top_builddir)/src/c/statisticsFunctions/includes

check_LDADD	=	$(top_builddir)/src/c/statisticsFunctions/max/libMax.la

check_PROGRAMS	= 	testFloatMax testDoubleMax

TESTS		=	testFloatMax testDoubleMax

#
# -*- Max Tests -*-
#
testFloatMax_SOURCES = testFloatMax.c
testFloatMax_CFLAGS =	$(check_INCLUDES)
testFloatMax_LDADD =	$(check_LDADD)

testDoubleMax_SOURCES =testDoubleMax.c
testDoubleMax_CFLAGS =	$(check_INCLUDES)
testDoubleMax_LDADD =	$(check_LDADD)

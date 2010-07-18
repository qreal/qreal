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



libCeil_la_CFLAGS =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/elementaryFunctions/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libCeil.la

libCeil_la_SOURCES =  $(HEAD) $(SRC)

HEAD =	../includes/ceil.h

SRC	=	sceils.c \
		dceils.c \
		cceils.c \
		zceils.c \
		sceila.c \
		dceila.c \
		cceila.c \
		zceila.c

####
# Checking Part
####

check_INCLUDES	=	-I $(top_builddir)/src/c/elementaryFunctions/includes \
			-I $(top_builddir)/src/c/type

check_LDADD	=	$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/elementaryFunctions/ceil/libCeil.la \
			@LIBMATH@

check_PROGRAMS	= 	testFloatCeil testDoubleCeil

TESTS		=	testFloatCeil testDoubleCeil

#
# -*- Ceil Tests -*-
#
testFloatCeil_SOURCES	 =	testFloatCeil.c
testFloatCeil_CFLAGS	 =	$(check_INCLUDES)
testFloatCeil_LDADD	 =	$(check_LDADD)

testDoubleCeil_SOURCES	 = 	testDoubleCeil.c
testDoubleCeil_CFLAGS	 =	$(check_INCLUDES)
testDoubleCeil_LDADD	 =	$(check_LDADD)

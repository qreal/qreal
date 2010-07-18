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



libInt_la_CFLAGS =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/elementaryFunctions/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libInt.la

libInt_la_SOURCES =  $(HEAD) $(SRC)

HEAD =	../includes/int.h

SRC	=	sints.c \
		dints.c \
		cints.c \
		zints.c \
		sinta.c \
		dinta.c \
		cinta.c \
		zinta.c

####
# Checking Part
####

check_INCLUDES	=	-I $(top_builddir)/src/c/elementaryFunctions/includes \
			-I $(top_builddir)/src/c/type

check_LDADD	=	$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/elementaryFunctions/int/libInt.la \
			$(top_builddir)/src/c/elementaryFunctions/fix/libFix.la \
			@LIBMATH@

check_PROGRAMS	= 	testFloatInt testDoubleInt

TESTS		=	testFloatInt testDoubleInt

#
# -*- Int Tests -*-
#
testFloatInt_SOURCES	 =	testFloatInt.c
testFloatInt_CFLAGS	 =	$(check_INCLUDES)
testFloatInt_LDADD	 =	$(check_LDADD)

testDoubleInt_SOURCES	 = 	testDoubleInt.c
testDoubleInt_CFLAGS	 =	$(check_INCLUDES)
testDoubleInt_LDADD	 =	$(check_LDADD)

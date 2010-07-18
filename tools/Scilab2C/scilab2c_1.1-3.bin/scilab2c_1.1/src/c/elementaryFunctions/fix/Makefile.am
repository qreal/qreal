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



libFix_la_CFLAGS =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/elementaryFunctions/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libFix.la

libFix_la_SOURCES =  $(HEAD) $(SRC)

HEAD =	../includes/fix.h

SRC	=	sfixs.c \
		dfixs.c \
		cfixs.c \
		zfixs.c \
		sfixa.c \
		dfixa.c \
		cfixa.c \
		zfixa.c

####
# Checking Part
####

check_INCLUDES	=	-I $(top_builddir)/src/c/elementaryFunctions/includes \
			-I $(top_builddir)/src/c/type

check_LDADD	=	$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/elementaryFunctions/fix/libFix.la \
			@LIBMATH@

check_PROGRAMS	= 	testFloatFix testDoubleFix

TESTS		=	testFloatFix testDoubleFix

#
# -*- Fix Tests -*-
#
testFloatFix_SOURCES	 =	testFloatFix.c
testFloatFix_CFLAGS	 =	$(check_INCLUDES)
testFloatFix_LDADD	 =	$(check_LDADD)

testDoubleFix_SOURCES	 = 	testDoubleFix.c
testDoubleFix_CFLAGS	 =	$(check_INCLUDES)
testDoubleFix_LDADD	 =	$(check_LDADD)

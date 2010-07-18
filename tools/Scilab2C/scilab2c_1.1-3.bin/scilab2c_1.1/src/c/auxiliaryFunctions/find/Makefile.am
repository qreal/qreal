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

AM_CFLAGS =	-I ../../type \
		-I ../includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libFind.la

libFind_la_SOURCES =	$(HEAD) $(SRC)

SRC	=	sfinda.c \
		dfinda.c \
		cfinda.c \
		zfinda.c

HEAD	=	../includes/find.h \
		../../includes/notFound.h

####
# Checking Part
####

check_INCLUDES	=	-I $(top_builddir)/src/c/auxiliaryFunctions/includes \
			-I $(top_builddir)/src/c/type

check_PROGRAMS	= 	testFind

TESTS		=	testFind

#
# -*- Find Tests -*-
#
testFind_SOURCES = 	testFind.h testFind.c
testFind_CFLAGS	=	$(check_INCLUDES)
testFind_LDADD	=	$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/auxiliaryFunctions/find/libFind.la \
			@LIBMATH@

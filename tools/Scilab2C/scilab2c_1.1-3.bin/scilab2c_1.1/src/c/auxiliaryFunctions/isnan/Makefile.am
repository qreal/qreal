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

libIsnan_la_CFLAGS =	-I ../../type \
			-I ../includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libIsnan.la

libIsnan_la_SOURCES =	$(HEAD) $(SRC)

SRC	=	sisnans.c \
		disnans.c \
		cisnans.c \
		zisnans.c

HEAD	=	../includes/isnan.h

####
# Checking Part
####

check_INCLUDES	=	-I $(top_builddir)/src/c/auxiliaryFunctions/includes \
			-I $(top_builddir)/src/c/type

check_PROGRAMS	= 	testIsnan

TESTS		=	testIsnan

#
# -*- Isnan Tests -*-
#
testIsnan_SOURCES = 	testIsNan.h testIsNan.c
testIsnan_CFLAGS  =	$(check_INCLUDES)
testIsnan_LDADD	  =	$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/auxiliaryFunctions/isnan/libIsnan.la \
			@LIBMATH@

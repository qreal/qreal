##
##  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
##  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
##
##  This file must be used under the terms of the CeCILL.
##  This source file is licensed as described in the file COPYING, which
##  you should have received as part of this distribution.  The terms
##  are also available at
##  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
##
##

libSize_la_CFLAGS =	-I ../../type \
			-I ../includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libSize.la

libSize_la_SOURCES =	$(HEAD)

HEAD	=	../includes/size.h

####
# Checking Part
####

check_INCLUDES	=	-I $(top_builddir)/src/c/auxiliaryFunctions/includes \
			-I $(top_builddir)/src/c/type

check_PROGRAMS	= 	testSize

TESTS		=	testSize

#
# -*- Size Tests -*-
#
testSize_SOURCES = 	testSize.h testSize.c
testSize_CFLAGS	=	$(check_INCLUDES)
testSize_LDADD	=	$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/auxiliaryFunctions/size/libSize.la \
			@LIBMATH@

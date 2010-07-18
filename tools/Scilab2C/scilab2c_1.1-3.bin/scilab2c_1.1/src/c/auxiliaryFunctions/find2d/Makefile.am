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
		-I $(top_builddir)/src/c/auxiliaryFunctions/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libFind2d.la

libFind2d_la_SOURCES =	$(HEAD) $(SRC)

SRC	=	sfind2da.c \
		dfind2da.c \
		cfind2da.c \
		zfind2da.c

HEAD	=	../includes/find2d.h

####
# Checking Part
####

check_INCLUDES	=	-I $(top_builddir)/src/c/auxiliaryFunctions/includes \
			-I $(top_builddir)/src/c/type

check_PROGRAMS	= 	testFind2d

TESTS		=	testFind2d

#
# -*- Find2d Tests -*-
#
testFind2d_SOURCES = 	testFind2d.h testFind2d.c
testFind2d_CFLAGS	=	$(check_INCLUDES)
testFind2d_LDADD	=	$(top_builddir)/src/c/type/libDoubleComplex.la \
				$(top_builddir)/src/c/type/libFloatComplex.la \
				$(top_builddir)/src/c/auxiliaryFunctions/find2d/libFind2d.la \
				@LIBMATH@

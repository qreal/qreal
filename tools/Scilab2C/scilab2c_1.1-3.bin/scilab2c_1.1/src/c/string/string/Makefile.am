##
##  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
##  Copyright (C) 2008-2008 - INRIA - Allan SIMON
##
##  This file must be used under the terms of the CeCILL.
##  This source file is licensed as described in the file COPYING, which
##  you should have received as part of this distribution.  The terms
##  are also available at
##  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
##
##

libString_la_CFLAGS =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/string/includes

pkglib_LTLIBRARIES =	libString.la

libString_la_SOURCES =	$(HEAD) $(SRC)

SRC	=	sstrings.c \
		dstrings.c \
		cstrings.c \
		zstrings.c \
		sstringa.c \
		dstringa.c \
		cstringa.c \
		zstringa.c

HEAD	=	../includes/string.h

####
# Checking Part
####

check_INCLUDES	=	-I $(top_builddir)/src/c/string/includes \
			-I $(top_builddir)/src/c/type

check_LDADD	=	$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			libString.la

check_PROGRAMS	= 	testString

TESTS		=	testString

#
# -*- Dispine Tests -*-
#
testString_SOURCES  = 	testString.c
testString_CFLAGS	=	$(check_INCLUDES)
testString_LDADD	=	$(check_LDADD)


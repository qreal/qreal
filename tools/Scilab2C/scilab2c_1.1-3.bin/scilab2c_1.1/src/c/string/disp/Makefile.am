##
##  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
##  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
##
##  This file must be used under the terms of the CeCILL.
##  This source file is licensed as described in the file COPYING, which
##  you should have received as part of this distribution.  The terms
##  are also available at
##  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
##
##

libDisp_la_CFLAGS =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/string/includes

pkglib_LTLIBRARIES =	libDisp.la

libDisp_la_SOURCES =	$(HEAD) $(SRC)

SRC	=	sdisps.c \
		ddisps.c \
		cdisps.c \
		zdisps.c \
		sdispa.c \
		ddispa.c \
		cdispa.c \
		zdispa.c

HEAD	=	../includes/disp.h

####
# Checking Part
####

check_INCLUDES	=	-I $(top_builddir)/src/c/string/includes \
			-I $(top_builddir)/src/c/type

check_LDADD	=	$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/string/disp/libDisp.la

check_PROGRAMS	= 	testDisp

TESTS		=	testDisp

#
# -*- Dispine Tests -*-
#
testDisp_SOURCES = 	testDisp.c
testDisp_CFLAGS	=	$(check_INCLUDES)
testDisp_LDADD	=	$(check_LDADD)


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



libRound_la_CFLAGS =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/elementaryFunctions/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libRound.la

libRound_la_SOURCES =  $(HEAD) $(SRC)

HEAD =	../includes/round.h

SRC	=	srounds.c \
		drounds.c \
		crounds.c \
		zrounds.c \
		srounda.c \
		drounda.c \
		crounda.c \
		zrounda.c

####
# Checking Part
####

check_INCLUDES	=	-I $(top_builddir)/src/c/elementaryFunctions/includes \
			-I $(top_builddir)/src/c/type

check_LDADD	=	$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/elementaryFunctions/round/libRound.la

check_PROGRAMS	= 	testFloatRound testDoubleRound

TESTS		=	testFloatRound testDoubleRound

#
# -*- Round Tests -*-
#
testFloatRound_SOURCES	 =	testFloatRound.c
testFloatRound_CFLAGS	 =	$(check_INCLUDES)
testFloatRound_LDADD	 =	$(check_LDADD)

testDoubleRound_SOURCES	 = 	testDoubleRound.c
testDoubleRound_CFLAGS	 =	$(check_INCLUDES)
testDoubleRound_LDADD	 =	$(check_LDADD)

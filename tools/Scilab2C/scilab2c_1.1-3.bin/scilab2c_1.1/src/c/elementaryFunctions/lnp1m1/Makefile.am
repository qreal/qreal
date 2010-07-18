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

libLnp1m1_la_CFLAGS =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/auxiliaryFunctions/includes \
			-I $(top_builddir)/src/c/elementaryFunctions/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libLnp1m1.la

libLnp1m1_la_SOURCES =	$(HEAD) $(SRC)

SRC	=	slnp1m1s.c \
		dlnp1m1s.c

HEAD	=	$(top_builddir)/src/c/elementaryFunctions/includes/lnp1m1.h

####
# Checking Part
####

check_INCLUDES	=	-I $(top_builddir)/src/c/elementaryFunctions/includes \
			-I $(top_builddir)/src/c/type

check_LDADD	=	$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/fortran/lapack/libscilapack.la \
			$(top_builddir)/src/c/elementaryFunctions/lnp1m1/libLnp1m1.la \
			$(top_builddir)/src/c/elementaryFunctions/sqrt/libSqrt.la \
			$(top_builddir)/src/c/auxiliaryFunctions/pythag/libPythag.la \
			$(top_builddir)/src/c/auxiliaryFunctions/abs/libAbs.la \
			$(top_builddir)/src/c/operations/multiplication/libMultiplication.la \
			@LIBMATH@

check_PROGRAMS	= 	testFloatLnp1m1 testDoubleLnp1m1

TESTS		=	testFloatLnp1m1 testDoubleLnp1m1

testFloatLnp1m1_SOURCES = 	testLnp1m1.h testFloatLnp1m1.c
testFloatLnp1m1_CFLAGS =	$(check_INCLUDES)
testFloatLnp1m1_LDADD =		$(check_LDADD)

testDoubleLnp1m1_SOURCES = 	testLnp1m1.h testDoubleLnp1m1.c
testDoubleLnp1m1_CFLAGS	=	$(check_INCLUDES)
testDoubleLnp1m1_LDADD	=	$(check_LDADD)

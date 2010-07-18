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

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES = libDoubleComplex.la libFloatComplex.la

libDoubleComplex_la_SOURCES = $(DCHEAD) $(DCSRC)

libFloatComplex_la_SOURCES = $(FCHEAD) $(FCSRC)

check_PROGRAMS = testDoubleComplex testFloatComplex

testDoubleComplex_SOURCES = testDoubleComplex.c
testDoubleComplex_LDADD = libDoubleComplex.la

testFloatComplex_SOURCES = testFloatComplex.c
testFloatComplex_LDADD = libFloatComplex.la

TESTS = testDoubleComplex testFloatComplex

### Double Complex
DCSRC	=	doubleComplex.c
DCHEAD	=	doubleComplex.h

### Float Complex
FCSRC	=	floatComplex.c
FCHEAD	=	floatComplex.h

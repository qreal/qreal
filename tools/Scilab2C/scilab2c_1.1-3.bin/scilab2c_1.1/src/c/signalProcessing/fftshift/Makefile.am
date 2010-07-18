##
##  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
##  Copyright (C) 2008 - INRIA - Arnaud TORSET
##
##  This file must be used under the terms of the CeCILL.
##  This source file is licensed as described in the file COPYING, which
##  you should have received as part of this distribution.  The terms
##  are also available at
##  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
##
##




libFftShift_la_CFLAGS =		-I $(top_builddir)/src/c/type \
				-I $(top_builddir)/src/c/signalProcessing/includes


instdir = $(top_builddir)/lib


pkglib_LTLIBRARIES = libFftShift.la

HEAD = ../includes/fftshift.h

libFftShift_la_SOURCES = 		$(HEAD) \
					sfftshifta.c \
					dfftshifta.c \
					cfftshifta.c \
					zfftshifta.c \
					srowfftshifta.c \
					drowfftshifta.c \
					crowfftshifta.c \
					zrowfftshifta.c \
					scolumnfftshifta.c \
					dcolumnfftshifta.c \
					ccolumnfftshifta.c \
					zcolumnfftshifta.c


###############
#### Check ####
###############

check_LDADD =		$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			libFftShift.la



check_INCLUDES =	-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/signalProcessing/includes

check_PROGRAMS = 	testFloatFftShift  testDoubleFftShift

TESTS =		testFloatFftShift       testDoubleFftShift

testDoubleFftShift_SOURCES = testDoubleFftShift.c
testDoubleFftShift_LDADD = $(check_LDADD)
testDoubleFftShift_CFLAGS = $(check_INCLUDES)

testFloatFftShift_SOURCES = testFloatFftShift.c
testFloatFftShift_LDADD = $(check_LDADD)
testFloatFftShift_CFLAGS = $(check_INCLUDES)




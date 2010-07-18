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



libFft_la_CFLAGS =	-I . \
			-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/signalProcessing/includes \
			-I $(top_builddir)/src/c/operations/includes \
			-I $(top_builddir)/src/c/matrixOperations/includes \
			-I $(top_builddir)/src/c/auxiliaryFunctions/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libFft.la

libFft_la_SOURCES =  $(HEAD) $(SRC)

HEAD =	../includes/fft.h \
	fft_internal.h

SRC	=		zfftma.c \
			cfftma.c \
            		dfft2.c \
			dfftbi.c \
			dfftmx.c \
			fft842.c \
			r2tx.c \
			r4tx.c \
			r8tx.c \
			dfftma.c \
			sfftma.c

####
# Checking Part
####

check_INCLUDES	=	-I . \
			-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/signalProcessing/includes \
			-I $(top_builddir)/src/c/matrixOperations/includes \
			-I $(top_builddir)/src/c/operations/includes \
			-I $(top_builddir)/src/c/auxiliaryFunctions/includes


check_LDADD	=	$(top_builddir)/src/c/type/libDoubleComplex.la \
			$(top_builddir)/src/c/type/libFloatComplex.la \
			$(top_builddir)/src/fortran/lapack/libscilapack.la \
			$(top_builddir)/src/fortran/blas/libsciblas.la \
			$(top_builddir)/src/c/signalProcessing/fft/libFft.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/c/operations/subtraction/libSubtraction.la \
			$(top_builddir)/src/c/matrixOperations/zeros/libMatrixZeros.la \
			@LIBMATH@

check_PROGRAMS	= 	testFloatFft testDoubleFft  testMatFft

TESTS		=	testFloatFft testDoubleFft  testMatFft

#
# -*- Fftine Tests -*-
#
testFloatFft_SOURCES = 	testFloatFft.c
testFloatFft_CFLAGS =	$(check_INCLUDES)
testFloatFft_LDADD =	$(check_LDADD)

testDoubleFft_SOURCES =	testDoubleFft.c
testDoubleFft_CFLAGS =	$(check_INCLUDES)
testDoubleFft_LDADD =	$(check_LDADD)


testMatFft_SOURCES = 	testMatFft.c
testMatFft_CFLAGS =	$(check_INCLUDES)
testMatFft_LDADD =	$(check_LDADD)

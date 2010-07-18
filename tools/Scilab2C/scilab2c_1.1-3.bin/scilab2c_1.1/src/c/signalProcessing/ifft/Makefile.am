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



libIfft_la_CFLAGS =	-I . \
			-I $(top_builddir)/src/c/type \
			-I $(top_builddir)/src/c/signalProcessing/includes \
			-I $(top_builddir)/src/c/matrixOperations/includes \
			-I $(top_builddir)/src/c/operations/includes \
			-I $(top_builddir)/src/c/auxiliaryFunctions/includes

instdir = $(top_builddir)/lib

pkglib_LTLIBRARIES =	libIfft.la

libIfft_la_SOURCES =  $(HEAD) $(SRC)

HEAD =	../includes/ifft.h \
	fft_internal.h

SRC	=		zifftma.c \
			cifftma.c \
			difft2.c \
			difftbi.c \
			difftmx.c \
			ifft842.c \
			ir2tx.c \
			ir4tx.c \
			ir8tx.c\
			difftma.c \
			sifftma.c


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
			$(top_builddir)/src/c/signalProcessing/ifft/libIfft.la \
			$(top_builddir)/src/c/matrixOperations/zeros/libMatrixZeros.la \
			$(top_builddir)/src/c/operations/addition/libAddition.la \
			$(top_builddir)/src/c/operations/subtraction/libSubtraction.la \
			@LIBMATH@

check_PROGRAMS	= 	testFloatIfft testDoubleIfft   testMatIfft

TESTS		=	testFloatIfft testDoubleIfft   testMatIfft

#
# -*- Fftine Tests -*-
#
testFloatIfft_SOURCES = 	testFloatIfft.c
testFloatIfft_CFLAGS =	$(check_INCLUDES)
testFloatIfft_LDADD =	$(check_LDADD)

testDoubleIfft_SOURCES =	testDoubleIfft.c
testDoubleIfft_CFLAGS =	$(check_INCLUDES)
testDoubleIfft_LDADD =	$(check_LDADD)


testMatIfft_SOURCES =	testMatIfft.c
testMatIfft_CFLAGS =	$(check_INCLUDES)
testMatIfft_LDADD =	$(check_LDADD)

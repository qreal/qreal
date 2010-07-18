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

AC_INIT([sci2cLibrary],[0.3],[])
AC_CONFIG_AUX_DIR(config)
AC_PREREQ(2.61)

RM='rm -f'
SCIDIR=$srcdir
SCIDIRFULL=`cd $SCIDIR && pwd`

AC_CONFIG_HEADERS([includes/machine.h])


AM_PROG_INSTALL_STRIP
AC_PROG_LN_S
#AC_PACKAGE_VERSION([4.0])

AC_CANONICAL_HOST

AM_MAINTAINER_MODE
AM_INIT_AUTOMAKE([-Wall foreign]) # Not using -Werror because we override {C,F}FLAGS in order to disable optimisation

### LibTool
AC_PROG_LIBTOOL
### Fortran Compiler
AC_PROG_F77
### C Compiler
AC_PROG_CC
### C++ Compiler
AC_PROG_CXX

# for "subdir-objects"
AM_PROG_CC_C_O
AC_PROG_F77_C_O

### Need to check for math lib and symbols
AC_SEARCH_LIBS(atan, m, LIBMATH="-lm")
AC_SUBST(LIBMATH)


#################################
## all the --with-* argument help
#################################
AC_ARG_WITH(stdc99,
   AS_HELP_STRING([--with-stdc99],[Enable standard C99 (Used in complex implementation)]))
if test "$with_stdc99" = yes; then
   AC_CHECK_HEADERS([complex.h])
   CFLAGS="$CFLAGS -std=c99"
fi

AC_ARG_WITH(blas,
   AS_HELP_STRING([--with-blas],[Enable call to blas library (Used in matrix operations)]))
if test "$with_blas" = no; then
   CFLAGS="$CFLAGS -DWITHOUT_BLAS"
fi

AC_ARG_ENABLE(debug,
   AS_HELP_STRING([--enable-debug],[Use verbose messages through compilation process]))
if test "$enable_debug" = yes; then
   CFLAGS="$CFLAGS -Wall -Wextra -pedantic -Wconversion -Wformat -Wshadow \
   -Wpointer-arith -Wcast-align -Wmissing-prototypes \
   -Wmissing-declarations"
fi

AC_ARG_ENABLE(stopOnWarning,
   AS_HELP_STRING([--enable-stop-on-warning],[Stop the compilation on the first warning found in the C/C++ code]))
if test "$enable_stop_on_warning" = yes; then
   CFLAGS="$CFLAGS -Werror"
fi

###########�FORTRAN FLAGS ######################

######################
######## With F77 / G77 / GFortran ...
######################

if test -n "$F77"; then
	  FFLAGS="$FFLAGS -g -Wall "
fi

###########�C FLAGS ######################

######################
######## With GCC ...
######################
     CFLAGS="$CFLAGS"


##############################################################
## test for functions in standard C library and C math library
##############################################################

# Provided by math.h
#AC_CHECK_FUNCS([pow] [sqrt] [finite] [floor])

#AC_CHECK_FUNC([exp10],,[
#				AC_DEFINE([log_10_],[2.3025850929940456840179914546844],[Provide a macro to do exp10])
#				AC_DEFINE([exp10(x)],[exp( (log_10_) * (x) )],[Provide a macro to do exp10])
#				])

########################
## test for header files
########################

AC_HEADER_STDBOOL

#################
## FFTW
#################

AC_ARG_WITH(fftw,
    [  --with-fftw	           compile with the FFTW 3 library ])
if test "$with_fftw" = yes; then
  # AC_FFTW()
  echo
fi

#################
## atlas
#################

# check user arguments
USER_ATLAS_LIB_PATH=""

AC_ARG_WITH(atlas-library,
     [  --with-atlas-library=DIR  Atlas library files are in DIR and we use Atlas ],
     [  USER_ATLAS_LIB_PATH=$withval
     CC_OPTIONS="$CC_OPTIONS -DWITH_ATLAS"],
     )
if test "$USER_ATLAS_LIB_PATH" = ""; then
	WHICH_BLAS="STD_BLAS"
 	IN_ATLAS=LOCAL
else
 	echo "Using Atlas library"
 	WHICH_BLAS="ATLAS_LAPACKBLAS"
 	ATLAS_DIR=$USER_ATLAS_LIB_PATH
 	IN_ATLAS=ATLAS
 	AC_DEFINE([WITH_ATLAS],[],[With the Atlas Lib])
fi

#######################
###### Creation of the header file (machine.h)
#######################

#operations/multiplication/Makefile
#operations/division/Makefile

AC_CONFIG_FILES([
Makefile
src/fortran/blas/Makefile
src/fortran/lapack/Makefile
src/c/type/Makefile
src/c/operations/Makefile
src/c/operations/addition/Makefile
src/c/operations/subtraction/Makefile
src/c/operations/division/Makefile
src/c/operations/multiplication/Makefile
src/c/matrixOperations/Makefile
src/c/matrixOperations/multiplication/Makefile
src/c/matrixOperations/division/Makefile
src/c/matrixOperations/cat/Makefile
src/c/matrixOperations/transpose/Makefile
src/c/matrixOperations/trace/Makefile
src/c/matrixOperations/hilbert/Makefile
src/c/matrixOperations/expm/Makefile
src/c/matrixOperations/eye/Makefile
src/c/matrixOperations/ones/Makefile
src/c/matrixOperations/infiniteNorm/Makefile
src/c/matrixOperations/inversion/Makefile
src/c/matrixOperations/jmat/Makefile
src/c/matrixOperations/chol/Makefile
src/c/matrixOperations/determ/Makefile
src/c/matrixOperations/dist/Makefile
src/c/matrixOperations/fill/Makefile
src/c/matrixOperations/magnitude/Makefile
src/c/matrixOperations/squaredMagnitude/Makefile
src/c/matrixOperations/logm/Makefile
src/c/matrixOperations/powm/Makefile
src/c/matrixOperations/zeros/Makefile
src/c/matrixOperations/spec/Makefile
src/c/matrixOperations/spec2/Makefile
src/c/implicitList/Makefile
src/c/elementaryFunctions/Makefile
src/c/elementaryFunctions/cos/Makefile
src/c/elementaryFunctions/cosh/Makefile
src/c/elementaryFunctions/acos/Makefile
src/c/elementaryFunctions/acosh/Makefile
src/c/elementaryFunctions/sin/Makefile
src/c/elementaryFunctions/sinh/Makefile
src/c/elementaryFunctions/asin/Makefile
src/c/elementaryFunctions/asinh/Makefile
src/c/elementaryFunctions/tan/Makefile
src/c/elementaryFunctions/tanh/Makefile
src/c/elementaryFunctions/atan/Makefile
src/c/elementaryFunctions/atan2/Makefile
src/c/elementaryFunctions/atanh/Makefile
src/c/elementaryFunctions/log/Makefile
src/c/elementaryFunctions/log1p/Makefile
src/c/elementaryFunctions/log10/Makefile
src/c/elementaryFunctions/exp/Makefile
src/c/elementaryFunctions/exp10/Makefile
src/c/elementaryFunctions/sqrt/Makefile
src/c/elementaryFunctions/lnp1m1/Makefile
src/c/elementaryFunctions/pow/Makefile
src/c/elementaryFunctions/ceil/Makefile
src/c/elementaryFunctions/fix/Makefile
src/c/elementaryFunctions/floor/Makefile
src/c/elementaryFunctions/int/Makefile
src/c/elementaryFunctions/round/Makefile
src/c/auxiliaryFunctions/Makefile
src/c/auxiliaryFunctions/abs/Makefile
src/c/auxiliaryFunctions/find/Makefile
src/c/auxiliaryFunctions/find2d/Makefile
src/c/auxiliaryFunctions/frexp/Makefile
src/c/auxiliaryFunctions/isempty/Makefile
src/c/auxiliaryFunctions/isnan/Makefile
src/c/auxiliaryFunctions/rand/Makefile
src/c/auxiliaryFunctions/sign/Makefile
src/c/auxiliaryFunctions/size/Makefile
src/c/auxiliaryFunctions/length/Makefile
src/c/auxiliaryFunctions/type/Makefile
src/c/auxiliaryFunctions/pythag/Makefile
src/c/auxiliaryFunctions/conj/Makefile
src/c/statisticsFunctions/Makefile
src/c/statisticsFunctions/mean/Makefile
src/c/statisticsFunctions/meanf/Makefile
src/c/statisticsFunctions/stdevf/Makefile
src/c/statisticsFunctions/prod/Makefile
src/c/statisticsFunctions/sum/Makefile
src/c/statisticsFunctions/variance/Makefile
src/c/statisticsFunctions/variancef/Makefile
src/c/statisticsFunctions/max/Makefile
src/c/statisticsFunctions/min/Makefile
src/c/string/Makefile
src/c/string/disp/Makefile
src/c/string/string/Makefile
src/c/signalProcessing/Makefile
src/c/signalProcessing/fft/Makefile
src/c/signalProcessing/ifft/Makefile
src/c/signalProcessing/levin/Makefile
src/c/signalProcessing/conv/Makefile
src/c/signalProcessing/conv2d/Makefile
src/c/signalProcessing/hilbert/Makefile
src/c/signalProcessing/crossCorr/Makefile
src/c/signalProcessing/lpc2cep/Makefile
src/c/signalProcessing/lev/Makefile
src/c/signalProcessing/fftshift/Makefile
])


AC_OUTPUT

echo ""

echo "Sci2C is configured as follows. Please verify that this configuration"
echo "matches your expectations."
echo ""
echo "Host system type : $host"
echo ""
echo "                  Option                        Value"
echo "-------------------------------------------------------------------------"
echo "Options:"
echo "Atlas library (--with-atlas-library) ............ : $USER_ATLAS_LIB_PATH"
echo "Use FFTW (--with-fftw) .......................... : $with_fftw"

echo ""

echo ""
echo "Options used to compile and link:"
echo "  PREFIX           = $PREFIX_DIR"
echo "  EXEC-PREFIX      = $EXEC_PREFIX_DIR"
echo "  CC               = $CC"
echo "  CFLAGS           = $CFLAGS"
#echo "  CPP              = $CPP"
#echo "  CPPFLAGS         = $CPPFLAGS"
echo "  PCFLAGS          = $PCFLAGS"
echo "  DEFS             = $DEFS"
echo "  LD               = $LD"
echo "  LDFLAGS          = $LDFLAGS"
echo "  LIBS             = $LIBS"
echo "  LIBMATH          = $LIBMATH"
echo "  CXX              = $CXX"
echo "  CXXFLAGS         = $CXXFLAGS"
echo "  F77              = $F77"
echo "  FFLAGS           = $FFLAGS"
echo "  F77_LDFLAGS      = $F77_LDFLAGS"
echo ""

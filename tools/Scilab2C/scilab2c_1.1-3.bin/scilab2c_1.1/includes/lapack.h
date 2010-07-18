/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __LAPACK_H__
#define __LAPACK_H__

#ifndef _MACRO_C2F_
#define _MACRO_C2F_
#define C2F(name) name##_ 
#endif 

#include "doubleComplex.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define getRelativeMachinePrecision()		dlamch_("e", 1L)
#define getOverflowThreshold()			dlamch_("o", 1L)
#define getUnderflowThreshold()			dlamch_("u", 1L)

#define getOneNorm(lines,cols,in,work) 		dlange_("1", lines, cols, in, lines, work)
#define resolveSystemLinear(cols1,row2,cpytranIn1,pIpiv, transposeOfIn2,info) \
		dgetrs_  ("N" ,cols1, row2, cpytranIn1 , cols1, pIpiv,transposeOfIn2, cols1, info) ;




/**
 *  -- LAPACK auxiliary routine (version 3.0) --
 *     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
 *     Courant Institute, Argonne National Lab, and Rice University
 *     October 31, 1992
 *
 *  Purpose
 *  =======
 *
 *  DLAMCH determines double precision machine parameters.
 *
 *  Arguments
 *  =========
 *
 *  CMACH   (input) CHARACTER*1
 *          Specifies the value to be returned by DLAMCH:
 *          = 'E' or 'e',   DLAMCH := eps
 *          = 'S' or 's ,   DLAMCH := sfmin
 *          = 'B' or 'b',   DLAMCH := base
 *          = 'P' or 'p',   DLAMCH := eps*base
 *          = 'N' or 'n',   DLAMCH := t
 *          = 'R' or 'r',   DLAMCH := rnd
 *          = 'M' or 'm',   DLAMCH := emin
 *          = 'U' or 'u',   DLAMCH := rmin
 *          = 'L' or 'l',   DLAMCH := emax
 *          = 'O' or 'o',   DLAMCH := rmax
 *
 *          where
 *
 *          eps   = relative machine precision
 *          sfmin = safe minimum, such that 1/sfmin does not overflow
 *          base  = base of the machine
 *          prec  = eps*base
 *          t     = number of (base) digits in the mantissa
 *          rnd   = 1.0 when rounding occurs in addition, 0.0 otherwise
 *          emin  = minimum exponent before (gradual) underflow
 *          rmin  = underflow threshold - base**(emin-1)
 *          emax  = largest exponent before overflow
 *          rmax  = overflow threshold  - (base**emax)*(1-eps)
 *
 * =====================================================================
 **/
extern		double dlamch_  (char *CMACH, unsigned long int i);

extern		double dlange_  (char* NORM, int* M, int* N, double* A, int* LDA , double* WORK);

extern 		double dgetrf_	(int*  M, int* N , double* A , int* LDA , int* IPIV , int* INFO);

extern		double dgecon_	(char* NORM, int* N, double* A, int* LDA, double* ANORM,
							 double* RCOND, double* WORK , int* IWORK, int* INFO ) ;

extern		double dgetrs_  (char* TRANS, int* N, int* NRHS, double* A, int* LDA,
							 int* IPIV, double* B, int* LDB, int* INFO ) ;

extern 		double dgelsy_ (int* M, int* N, int* NRHS, double* A, int* LDA,
							double* B, int* LDB, int* JPVT, double* RCOND, int* RANK,
							double* WORK, int* LWORK, int* INFO) ;

extern		double dlacpy_ (char* NORM, int* M, int* N, double* A, int* LDA,
							double* B,  int* LDB );

extern      double dgetri_ (int* N , double* A , int* LDA , int* IPIV , double* WORK,
                            int* LWORK , int* INFO ) ;


/****** doubleComplex fortran function ************/
extern 		double zgelsy_ (int*,int*,int*,doubleComplex*,int*,doubleComplex*,int*,int*,double*,int*,doubleComplex*,int*,double*,int*) ;


extern	    double zlamch_  ();

extern		double zlange_  (char*,int*,int*,doubleComplex*,int*,doubleComplex*);

extern 		double zgetrf_ (int *, int *, doubleComplex *, int *, int *, int *);

extern		double zgecon_	( char*,int*,doubleComplex*,int*,double*,double*,doubleComplex*,double*,int*) ;

extern		double zgetrs_  ( char *,int*,int*,doubleComplex*,int*,int*,doubleComplex*,int*,int*) ;

extern		double zlacpy_ (char*,int*,int*,doubleComplex*,int*,doubleComplex*,int*);

extern      double zgetri_ (int*,doubleComplex*,int*,int*,doubleComplex*,int*,int*) ;
/*extern      int zgelsy_ ();*/

/*certainly have some blas functions in */
extern int C2F(split)();
extern int C2F(exch)();

extern int C2F(balbak)();
extern double C2F(ddot)();
extern int C2F(pade)();
extern int C2F(dcopy)();
extern int C2F(dscal)();

extern int C2F(dgeco)();
extern int C2F(dgesl)();
extern int C2F(coef)();
extern int C2F(cerr)();
extern int C2F(dclmat)();
extern int C2F(dexpm1)();
extern int C2F(wexpm1)();
extern int C2F(drot)();

extern int C2F(intexpm) ();

extern int C2F(zcopy)(int*,doubleComplex *,int*,doubleComplex*,int*);

extern  int C2F(dgemm)(char *,char*,int*,int*,int*,double*,double*,int*,double*,int*,double*,double*,int*);
extern  int     C2F(idamax)() ;/* could be transcribe easily in c */
extern  int     C2F(daxpy) () ;/* could be transcribe easily in c */
extern  int     C2F(dscal) () ;/* could be transcribe easily in c */
extern  int     C2F(dasum) () ;/* could be transcribe easily in c */

/* used in chol */
extern int C2F(dpotrf)(char*,int*,double*,int*,int*);
extern int C2F(zpotrf)(char*,int*,doubleComplex*,int*,int*);

/* used in logm */
extern int C2F(zgeev)(char*,char*,int*,doubleComplex*,int*,doubleComplex*,
					  doubleComplex*,int*,doubleComplex*,int*,doubleComplex*,int *,doubleComplex*,int*);	

extern int C2F(zheev)(char*,char*,int*,doubleComplex*,int*,double*,doubleComplex*,int*,double*,int*);

/* used in spec */
extern int C2F(dgeev)(char*,char*,int*,double*,int*,double*,double*,double*,int*,double*,int*,double*,int*,int*);
extern int C2F(dsyev)(char*,char*,int*,double*,int*,double*,double*,int*,int*);

#ifdef  __cplusplus
} /* extern "C" */
#endif
#endif /* !__LAPACK_H__ */

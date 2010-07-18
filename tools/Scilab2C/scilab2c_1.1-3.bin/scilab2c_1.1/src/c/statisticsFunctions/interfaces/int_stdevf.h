/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2008 - INRIA - Allan SIMON
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* THIS IS AN AUTOMATICALLY GENERATED FILE : DO NOT EDIT BY HAND. */

#ifndef __INT_STDEVF_H__
#define __INT_STDEVF_H__

#define s0s0stdevfs0(in1,in2)					sstdevfs(in1,in2)

#define d0d0stdevfd0(in1,in2)					dstdevfs(in1,in2)

#define c0s0stdevfc0(in1,in2)					cstdevfs(in1,FloatComplex(in2 , 0.0f))

#define z0d0stdevfz0(in1,in2)					zstdevfs(in1,DoubleComplex(in2, 0.0))

#define s2s2stdevfs0(in1, size1, in2, size2)				sstdevfa(in1, size1[0],size1[1], in2)

#define d2d2stdevfd0(in1, size1, in2, size2)				dstdevfa(in1, size1[0],size1[1], in2)

#define c2s2stdevfc0(in1, size1, in2, size2)				cstdevfcs(in1, size1[0], size1[1], in2)

#define z2d2stdevfz0(in1, size1, in2, size2)				zstdevfzd(in1, size1[0], size1[1], in2)


/*by rows or columns */

#define s0s0s0stdevfs0(in1,in2,in3)				(in3==1) ? srowstdevfs(in1,in2) : scolumnstdevfs(in1,in2)	

#define d0d0d0stdevfd0(in1,in2,in3)				(in3==1) ? drowstdevfs(in1,in2) : dcolumnstdevfs(in1,in2)	

#define c0s0s0stdevfc0(in1,in2,in3)				(in3==1) ? crowstdevfs(in1,FloatComplex(in2 , 0.0f))  : ccolumnstdevfs(in1,FloatComplex(in2 , 0.0f))	

#define z0d0d0stdevfz0(in1,in2,in3)				(in3==1) ? zrowstdevfs(in1,DoubleComplex(in2, 0.0))   : zcolumnstdevfs(in1,DoubleComplex(in2, 0.0))
	

#define s2s2s0stdevfs2(in1, size1, in2, size2,in3,out)		(in3==1) ? srowstdevfa(in1,size1[0],size1[1],in2,out) : scolumnstdevfa(in1,size1[0],size1[1],in2,out)	

#define d2d2d0stdevfd2(in1, size1, in2, size2,in3,out)		(in3==1) ? drowstdevfa(in1,size1[0],size1[1],in2,out) : dcolumnstdevfa(in1,size1[0],size1[1],in2,out)





#define c2s2s0stdevfc2(in1, size1, in2, size2,in3,out)		if (in3 == 1) {float* temp;\
								 temp=malloc((uint)(size1[0]*size1[1])*sizeof(float));\
								 sfilla(temp,size1[0],size1[1],0);\
								 crowstdevfa(in1,size1[0],size1[1],FloatComplexMatrix(in2,temp,size1[0]*size1[1]),out);\
								 free(temp);\
								}\
								else\
								{float* temp;\
								 temp=malloc((uint)(size1[0]*size1[1])*sizeof(float));\
								 sfilla(temp,size1[0],size1[1],0);\
								 ccolumnstdevfa(in1,size1[0],size1[1],FloatComplexMatrix(in2,temp,size1[0]*size1[1]),out);\
								 free(temp);\
								}	

#define z2d2d0stdevfz2(in1, size1, in2, size2,in3,out)		if (in3 == 1) {double* temp;\
								temp=malloc((uint)(size1[0]*size1[1])*sizeof(double));\
								dfilla(temp,size1[0],size1[1],0);\
								zrowstdevfa(in1,size1[0],size1[1],DoubleComplexMatrix(in2,temp,size1[0]*size1[1]),out);\
								free(temp);\
								}\
								else\
								{double* temp;\
								temp=malloc((uint)(size1[0]*size1[1])*sizeof(double));\
								dfilla(temp,size1[0],size1[1],0);\
								zcolumnstdevfa(in1,size1[0],size1[1],DoubleComplexMatrix(in2,temp,size1[0]*size1[1]),out);\
								free(temp);\
								}	

		
/**/


#define s0c0stdevfs0(in1,in2)					creals (cstdevfs(FloatComplex(in1 , 0.0f),in2))
#define s0c0stdevfc0(in1,in2)					cstdevfs(FloatComplex(in1 , 0.0f),in2)

#define d0z0stdevfd0(in1,in2)					zreals(zstdevfs(DoubleComplex(in1, 0.0 ),in2)
#define d0z0stdevfz0(in1,in2)					zstdevfs(DoubleComplex(in1, 0.0 ),in2)


#define c0c0stdevfc0(in1,in2)					cstdevfs(in1,in2)

#define z0z0stdevfz0(in1,in2)					zstdevfs(in1,in2)


#define s2c2stdevfs0(in1, size1, in2, size2)				creals(cstdevfsc(in1, size1[0],size1[1], in2))
#define s2c2stdevfc0(in1, size1, in2, size2)				cstdevfsc(in1, size1[0],size1[1], in2)

#define d2z2stdevfd0(in1, size1, in2, size2)				zreals(zstdevfdz(in1, size1[0],size1[1], in2))
#define d2z2stdevfz0(in1, size1, in2, size2)				zstdevfdz(in1, size1[0],size1[1], in2)

#define c2c2stdevfc0(in1, size1, in2, size2)				cstdevfa(in1, size1[0], size1[1], in2)

#define z2z2stdevfz0(in1, size1, in2, size2)				zstdevfa(in1, size1[0], size1[1], in2)


/*by rows or columns */

#define s0c0s0stdevfs0(in1,in2,in3)				(in3==1) ? creals(crowstdevfs(FloatComplex(in1, 0.0f),in2)) : creals(ccolumnstdevfs(FloatComplex(in1, 0.0f),in2))	

#define d0z0d0stdevfd0(in1,in2,in3)				(in3==1) ? zreals(zrowstdevfs(DoubleComplex(in1, 0.0),in2)) : zreals(zcolumnstdevfs(DoubleComplex(in1, 0.0),in2))	

#define c0c0s0stdevfc0(in1,in2,in3)				(in3==1) ? crowstdevfs(in1,in2) : ccolumnstdevfs(in1,in2)	

#define z0z0d0stdevfz0(in1,in2,in3)				(in3==1) ? zrowstdevfs(in1,in2) : zcolumnstdevfs(in1,in2)
	

#define s2c2s0stdevfs2(in1, size1, in2, size2,in3,out)		if (in3 == 1) {float* temp;\
								temp=malloc((uint)(size1[0]*size1[1])*sizeof(float));\
								sfilla(temp,size1[0],size1[1],0);\
								crowstdevfa(FloatComplexMatrix(in1,temp,size1[0]*size1[1]),size1[0],size1[1],in2,FloatComplexMatrix(out,temp,size1[0]*size1[1]));\
								free(temp);\
								}\
								else\
								{float* temp;\
								temp=malloc((uint)(size1[0]*size1[1])*sizeof(float));\
								sfilla(temp,size1[0],size1[1],0);\
								ccolumnstdevfa(FloatComplexMatrix(in1,temp,size1[0]*size1[1]),size1[0],size1[1],in2,FloatComplexMatrix(out,temp,size1[0]*size1[1]));\
								free(temp);\
								}	

#define d2z2d0stdevfd2(in1, size1, in2, size2,in3,out)		if (in3 == 1) {double* temp;\
								temp=malloc((uint)(size1[0]*size1[1])*sizeof(double));\
								dfilla(temp,size1[0],size1[1],0);\
								zrowstdevfa(DoubleComplexMatrix(in1,temp,size1[0]*size1[1]),size1[0],size1[1],in2,DoubleComplexMatrix(out,temp,size1[0]));\
								free(temp);\
								}\
								else\
								{double* temp;\
								temp=malloc((uint)(size1[0]*size1[1])*sizeof(double));\
								dfilla(temp,size1[0],size1[1],0);\
								zcolumnstdevfa(DoubleComplexMatrix(in1,temp,size1[0]*size1[1]),size1[0],size1[1],in2,DoubleComplexMatrix(out,temp,size1[0]));\
								free(temp);\
								}	


/*complex output*/

#define s2c2s0stdevfc2(in1, size1, in2, size2,in3,out)		if (in3 == 1) {float* temp;\
								temp=malloc((uint)(size1[0]*size1[1])*sizeof(float));\
								sfilla(temp,size1[0],size1[1],0);\
								crowstdevfa(FloatComplexMatrix(in1,temp,size1[0]*size1[1]),size1[0],size1[1],in2,out);\
								free(temp);\
								}\
								else\
								{float* temp;\
								temp=malloc((uint)(size1[0]*size1[1])*sizeof(float));\
								sfilla(temp,size1[0],size1[1],0);\
								ccolumnstdevfa(FloatComplexMatrix(in1,temp,size1[0]*size1[1]),size1[0],size1[1],in2,out);\
								free(temp);\
								}	

#define d2z2d0stdevfz2(in1, size1, in2, size2,in3,out)		if (in3 == 1) {double* temp;\
								temp=malloc((uint)(size1[0]*size1[1])*sizeof(double));\
								dfilla(temp,size1[0],size1[1],0);\
								zrowstdevfa(DoubleComplexMatrix(in1,temp,size1[0]*size1[1]),size1[0],size1[1],in2,out);\
								free(temp);\
								}\
								else\
								{double* temp;\
								temp=malloc((uint)(size1[0]*size1[1])*sizeof(double));\
								dfilla(temp,size1[0],size1[1],0);\
								zcolumnstdevfa(DoubleComplexMatrix(in1,temp,size1[0]*size1[1]),size1[0],size1[1],in2,out);\
								free(temp);\
								}	



#define c2c2s0stdevfc2(in1, size1, in2, size2,in3,out)		(in3==1) ? crowstdevfa(in1,size1[0],size1[1],in2,out) : ccolumnstdevfa(in1,size1[0],size1[1],in2,out)

#define z2z2d0stdevfz2(in1, size1, in2, size2,in3,out)		(in3==1) ? zrowstdevfa(in1,size1[0],size1[1],in2,out) : zcolumnstdevfa(in1,size1[0],size1[1],in2,out)	


/**/

#endif /* !__INT_STDEVF_H__ */

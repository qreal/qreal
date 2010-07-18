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

#ifndef __INT_MEANF_H__
#define __INT_MEANF_H__

#define s0s0meanfs0(in1,in2)					smeanfs(in1,in2)

#define d0d0meanfd0(in1,in2)					dmeanfs(in1,in2)

#define c0s0meanfc0(in1,in2)					cmeanfs(in1,FloatComplex(in2 , 0.0f))

#define z0d0meanfz0(in1,in2)					zmeanfs(in1,DoubleComplex(in2, 0.0))

#define s2s2meanfs0(in1, size1, in2, size2)				smeanfa(in1, size1[0]*size1[1], in2)

#define d2d2meanfd0(in1, size1, in2, size2)				dmeanfa(in1, size1[0]*size1[1], in2)

#define c2s2meanfc0(in1, size1, in2, size2)				cmeanfcs(in1, size1[0], size1[1], in2)

#define z2d2meanfz0(in1, size1, in2, size2)				zmeanfzd(in1, size1[0], size1[1], in2)


/*by rows or columns */

#define s0s0s0meanfs0(in1,in2,in3)				(in3==1) ? srowmeanfs(in1,in2) : scolumnmeanfs(in1,in2)	

#define d0d0d0meanfd0(in1,in2,in3)				(in3==1) ? drowmeanfs(in1,in2) : dcolumnmeanfs(in1,in2)	

#define c0s0s0meanfc0(in1,in2,in3)				(in3==1) ? crowmeanfs(in1,FloatComplex(in2 , 0.0f))  : ccolumnmeanfs(in1,FloatComplex(in2 , 0.0f))	

#define z0d0d0meanfz0(in1,in2,in3)				(in3==1) ? zrowmeanfs(in1,DoubleComplex(in2, 0.0))   : zcolumnmeanfs(in1,DoubleComplex(in2, 0.0))
	

#define s2s2s0meanfs2(in1, size1, in2, size2,in3,out)		(in3==1) ? srowmeanfa(in1,size1[0],size1[1],in2,out) : scolumnmeanfa(in1,size1[0],size1[1],in2,out)	

#define d2d2d0meanfd2(in1, size1, in2, size2,in3,out)		(in3==1) ? drowmeanfa(in1,size1[0],size1[1],in2,out) : dcolumnmeanfa(in1,size1[0],size1[1],in2,out)





#define c2s2s0meanfc2(in1, size1, in2, size2,in3,out)		 if (in3==1) {float* temp;\
								 temp=malloc((uint)(size1[0]*size1[1])*sizeof(float));\
								 sfilla(temp,size1[0],size1[1],0);\
								 crowmeanfa(in1,size1[0],size1[1],FloatComplexMatrix(in2,temp,size1[0]*size1[1]),out);\
								 free(temp);\
								}\
								else\
								{float* temp;\
								 temp=malloc((uint)(size1[0]*size1[1])*sizeof(float));\
								 sfilla(temp,size1[0],size1[1],0);\
								 ccolumnmeanfa(in1,size1[0],size1[1],FloatComplexMatrix(in2,temp,size1[0]*size1[1]),out);\
								 free(temp);\
								}	

#define z2d2d0meanfz2(in1, size1, in2, size2,in3,out)		if (in3==1)  {double* temp;\
								temp=malloc((uint)(size1[0]*size1[1])*sizeof(double));\
								dfilla(temp,size1[0],size1[1],0);\
								zrowmeanfa(in1,size1[0],size1[1],DoubleComplexMatrix(in2,temp,size1[0]*size1[1]),out);\
								free(temp);\
								}\
								else\
								{double* temp;\
								temp=malloc((uint)(size1[0]*size1[1])*sizeof(double));\
								dfilla(temp,size1[0],size1[1],0);\
								zcolumnmeanfa(in1,size1[0],size1[1],DoubleComplexMatrix(in2,temp,size1[0]*size1[1]),out);\
								free(temp);\
								}	

		
/**/


#define s0c0meanfs0(in1,in2)					creals (cmeanfs(FloatComplex(in1 , 0.0f),in2))
#define s0c0meanfc0(in1,in2)					cmeanfs(FloatComplex(in1 , 0.0f),in2)

#define d0z0meanfd0(in1,in2)					zreals(zmeanfs(DoubleComplex(in1, 0.0 ),in2))
#define d0z0meanfz0(in1,in2)					zmeanfs(DoubleComplex(in1, 0.0 ),in2)

#define c0c0meanfc0(in1,in2)					cmeanfs(in1,in2)

#define z0z0meanfz0(in1,in2)					zmeanfs(in1,in2)


#define s2c2meanfs0(in1, size1, in2, size2)				creals (cmeanfsc(in1, size1[0],size1[1], in2))
#define s2c2meanfc0(in1, size1, in2, size2)				cmeanfsc(in1, size1[0],size1[1], in2)

#define d2z2meanfd0(in1, size1, in2, size2)				zreals(zmeanfdz(in1, size1[0],size1[1], in2))
#define d2z2meanfz0(in1, size1, in2, size2)				zmeanfdz(in1, size1[0],size1[1], in2)

#define c2c2meanfc0(in1, size1, in2, size2)				cmeanfa(in1, size1[0]* size1[1], in2)

#define z2z2meanfz0(in1, size1, in2, size2)				zmeanfa(in1, size1[0]* size1[1], in2)


/*by rows or columns */

#define s0c0s0meanfs0(in1,in2,in3)				(in3==1) ? crowmeanfs(FloatComplex(in1, 0.0f),in2) : ccolumnmeanfs(FloatComplex(in1, 0.0f),in2)	

#define d0z0d0meanfd0(in1,in2,in3)				(in3==1) ? zrowmeanfs(DoubleComplex(in1, 0.0),in2) : zcolumnmeanfs(DoubleComplex(in1, 0.0),in2)	

#define c0c0s0meanfc0(in1,in2,in3)				(in3==1) ? crowmeanfs(in1,in2) : ccolumnmeanfs(in1,in2)	

#define z0z0d0meanfz0(in1,in2,in3)				(in3==1) ? zrowmeanfs(in1,in2) : zcolumnmeanfs(in1,in2)
	

#define s2c2s0meanfs2(in1, size1, in2, size2,in3,out)		if (in3==1)  {float* temp;\
								temp=malloc((uint)(size1[0]*size1[1])*sizeof(float));\
								sfilla(temp,size1[0],size1[1],0);\
								crowmeanfa(FloatComplexMatrix(in1,temp,size1[0]*size1[1]),size1[0],size1[1],in2,out);\
								free(temp);\
								}\
								else\
								{float* temp;\
								temp=malloc((uint)(size1[0]*size1[1])*sizeof(float));\
								sfilla(temp,size1[0],size1[1],0);\
								ccolumnmeanfa(FloatComplexMatrix(in1,temp,size1[0]*size1[1]),size1[0],size1[1],in2,out);\
								free(temp);\
								}	

#define d2z2d0meanfd2(in1, size1, in2, size2,in3,out)		if (in3==1)  {double* temp;\
								temp=malloc((uint)(size1[0]*size1[1])*sizeof(double));\
								dfilla(temp,size1[0],size1[1],0);\
								zrowmeanfa(DoubleComplexMatrix(in1,temp,size1[0]*size1[1]),size1[0],size1[1],in2,out);\
								free(temp);\
								}\
								else\
								{double* temp;\
								temp=malloc((uint)(size1[0]*size1[1])*sizeof(double));\
								dfilla(temp,size1[0],size1[1],0);\
								zcolumnmeanfa(DoubleComplexMatrix(in1,temp,size1[0]*size1[1]),size1[0],size1[1],in2,out);\
								free(temp);\
								}	

/*complex output*/
#define s2c2s0meanfc2(in1, size1, in2, size2,in3,out)		if (in3==1)  {float* temp;\
								temp=malloc((uint)(size1[0]*size1[1])*sizeof(float));\
								sfilla(temp,size1[0],size1[1],0);\
								crowmeanfa(FloatComplexMatrix(in1,temp,size1[0]*size1[1]),size1[0],size1[1],in2,out);\
								free(temp);\
								}\
								else\
								{float* temp;\
								temp=malloc((uint)(size1[0]*size1[1])*sizeof(float));\
								sfilla(temp,size1[0],size1[1],0);\
								ccolumnmeanfa(FloatComplexMatrix(in1,temp,size1[0]*size1[1]),size1[0],size1[1],in2,out);\
								free(temp);\
								}	

#define d2z2d0meanfz2(in1, size1, in2, size2,in3,out)		if (in3==1)  {double* temp;\
								temp=malloc((uint)(size1[0]*size1[1])*sizeof(double));\
								dfilla(temp,size1[0],size1[1],0);\
								zrowmeanfa(DoubleComplexMatrix(in1,temp,size1[0]*size1[1]),size1[0],size1[1],in2,out);\
								free(temp);\
								}\
								else\
								{double* temp;\
								temp=malloc((uint)(size1[0]*size1[1])*sizeof(double));\
								dfilla(temp,size1[0],size1[1],0);\
								zcolumnmeanfa(DoubleComplexMatrix(in1,temp,size1[0]*size1[1]),size1[0],size1[1],in2,out);\
								free(temp);\
								}	



#define c2c2s0meanfc2(in1, size1, in2, size2,in3,out)		(in3==1) ? crowmeanfa(in1,size1[0],size1[1],in2,out) : ccolumnmeanfa(in1,size1[0],size1[1],in2,out)

#define z2z2d0meanfz2(in1, size1, in2, size2,in3,out)		(in3==1) ? zrowmeanfa(in1,size1[0],size1[1],in2,out) : zcolumnmeanfa(in1,size1[0],size1[1],in2,out)	
	

#endif /* !__INT_MEANF_H__ */

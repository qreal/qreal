/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008 - INRIA - Arnaud TORSET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef WITHOUT_LAPACK
#include "lapack.h"
#else
#include "division.h"
#endif

#include "determ.h"
#include "multiplication.h"
#include "subtraction.h"
#include "addition.h"
#include <malloc.h>


floatComplex cdeterma(floatComplex *in, int size){
#ifndef WITHOUT_LAPACK
	int i=0,info=0;
	doubleComplex *inCopy,out_tmp;
	int *vectPivot;
	floatComplex out, tmp1,tmp2;


	/*Calculation of the determinant*/
	switch(size){
		case 2 : out = cdiffs(cmuls(in[0],in[3]),cmuls(in[1],in[2]));
			 break;
		case 3 : /*regle de Sarrus*/
			 out=FloatComplex(0,0);
			 /*Addition part*/
			 tmp1 =  cmuls(in[0],in[4]);
			 tmp2 = cmuls(tmp1,in[8]);
			 out = cadds(out,tmp2);
			 tmp1 =  cmuls(in[1],in[5]);
			 tmp2 = cmuls(tmp1,in[6]);
			 out = cadds(out,tmp2);
			 tmp1 =  cmuls(in[2],in[3]);
			 tmp2 = cmuls(tmp1,in[7]);
			 out = cadds(out,tmp2);
			 /*Subtraction part*/			 
			 tmp1 =  cmuls(in[0],in[5]);
			 tmp2 = cmuls(tmp1,in[7]);
			 out = cdiffs(out,tmp2);
			 tmp1 =  cmuls(in[1],in[3]);
			 tmp2 = cmuls(tmp1,in[8]);
			 out = cdiffs(out,tmp2);
			 tmp1 =  cmuls(in[2],in[4]);
			 tmp2 = cmuls(tmp1,in[6]);
			 out = cdiffs(out,tmp2);
			 break;
			 
		default : 
		
		
			  /*We must have a doubleComplex matrix cause
			    zgetrf accept only double parameters*/
			    	
			  /*Copy the input matrix*/
			  inCopy=(doubleComplex*)malloc((unsigned int)(size*size)*sizeof(doubleComplex));
			  for (i=0;i<size*size;i++){
			  	inCopy[i]=DoubleComplex((double)creals(in[i]),(double)cimags(in[i]));
			  }



			  vectPivot=(int*)malloc((unsigned int)size*sizeof(int));
			  zgetrf_  ( &size, &size, inCopy, &size, vectPivot, &info);
						
			  out_tmp=DoubleComplex(1,0);
			  for (i=0;i<size;i++){
			  	if (vectPivot[i]!=i+1) out_tmp=DoubleComplex(-zreals(out_tmp),-zimags(out_tmp));
			  	out_tmp=zmuls(inCopy[i*(size+1)],out_tmp);
			  }
			  out=FloatComplex((float)zreals(out_tmp),(float)zimags(out_tmp));

			  free(vectPivot);
			  free(inCopy);
	}
#else
	int i=0,j=0,k=0;
	floatComplex *inCopy;
	floatComplex tmp1,tmp2;
	floatComplex pivot;
	floatComplex out;
	
		
	switch(size){
		case 2 : out = cdiffs(cmuls(in[0],in[3]),cmuls(in[1],in[2]));
			 break;
		case 3 : /*regle de Sarrus*/
			 out=FloatComplex(0,0);
			 /*Addition part*/
			 tmp1 =  cmuls(in[0],in[4]);
			 tmp2 = cmuls(tmp1,in[8]);
			 out = cadds(out,tmp2);
			 tmp1 =  cmuls(in[1],in[5]);
			 tmp2 = cmuls(tmp1,in[6]);
			 out = cadds(out,tmp2);
			 tmp1 =  cmuls(in[2],in[3]);
			 tmp2 = cmuls(tmp1,in[7]);
			 out = cadds(out,tmp2);
			 /*Subtraction part*/			 
			 tmp1 =  cmuls(in[0],in[5]);
			 tmp2 = cmuls(tmp1,in[7]);
			 out = cdiffs(out,tmp2);
			 tmp1 =  cmuls(in[1],in[3]);
			 tmp2 = cmuls(tmp1,in[8]);
			 out = cdiffs(out,tmp2);
			 tmp1 =  cmuls(in[2],in[4]);
			 tmp2 = cmuls(tmp1,in[6]);
			 out = cdiffs(out,tmp2);
			 break;
			 
		default : 
				  /*Copy the input matrix*/
			  inCopy=malloc((unsigned int)(size*size)*sizeof(floatComplex));
			  for (i=0;i<size*size;i++) inCopy[i]=in[i];
			  
    			  for (i=0;i<size;i++){
				for (j=i+1;j<size;j++){
					pivot = crdivs(inCopy[i*size+j],inCopy[i*size+i]);
					for (k=0;k<size-i;k++){
						inCopy[i*size+j+k*size]=cdiffs(inCopy[i*size+j+k*size],cmuls(pivot,inCopy[i*size+i+k*size]));
					}
				}
			  }
			  out=FloatComplex(1,0);
			  for (i=0;i<size;i++){
			  	out = cmuls(out,inCopy[i*size+i]);
			  }
			  free(inCopy);
			  break;
	}
#endif

	return out;		 
}



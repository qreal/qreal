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

#include <stdlib.h>
#ifndef WITHOUT_LAPACK
#include "lapack.h"
#else
#include "division.h"
#endif
#include "determ.h"
#include "multiplication.h"
#include "subtraction.h"
#include "addition.h"


doubleComplex zdeterma(doubleComplex * in, int size){
#ifndef WITHOUT_LAPACK
	doubleComplex tmp1,tmp2,out;
	int i=0,info=0;
	doubleComplex *inCopy;
	int* vectPivot;
	
	switch(size){
		case 2 : out = zdiffs(zmuls(in[0],in[3]),zmuls(in[1],in[2]));
			 break;
		case 3 : /*regle de Sarrus*/
			 out=DoubleComplex(0,0);
			 /*Addition Part*/
			 tmp1 = zmuls(in[0],in[4]);
			 tmp2 = zmuls(tmp1,in[8]);
			 out = zadds(out,tmp2);
			 tmp1 = zmuls(in[1],in[5]);
			 tmp2 = zmuls(tmp1,in[6]);
			 out = zadds(out,tmp2);
			 tmp1 = zmuls(in[2],in[3]);
			 tmp2 = zmuls(tmp1,in[7]);
			 out = zadds(out,tmp2);
			 /*Subtraction Part*/
			 tmp1 = zmuls(in[0],in[5]);
			 tmp2 = zmuls(tmp1,in[7]);
			 out = zdiffs(out,tmp2);
			 tmp1 = zmuls(in[1],in[3]);
			 tmp2 = zmuls(tmp1,in[8]);
			 out = zdiffs(out,tmp2);
			 tmp1 = zmuls(in[2],in[4]);
			 tmp2 = zmuls(tmp1,in[6]);
			 out = zdiffs(out,tmp2);
			 break;
			 
		default : 
			  /*Allocate inCopy and Copy in in inCopy*/
			  inCopy=(doubleComplex*)malloc((unsigned int)(size*size)*sizeof(doubleComplex));
			  for (i=0;i<size*size;i++) inCopy[i]=in[i];
			  
			  /*Calculation of determinant*/
			  vectPivot = (int*)malloc((unsigned int)size*sizeof(int));
			  zgetrf_ ( &size, &size, inCopy, &size, vectPivot, &info);
			  
			  out=DoubleComplex(1,0);
			  for (i=0;i<size;i++){
			  	if (vectPivot[i]!=i+1) out=DoubleComplex(-zreals(out),-zimags(out));
			  	out=zmuls(inCopy[i*(size+1)],out);
			  }
			  free(vectPivot);
			  free(inCopy);
			  break;
	}
	
#else
	int i=0,j=0,k=0;
	doubleComplex *inCopy;
	doubleComplex tmp1,tmp2;
	doubleComplex pivot;
	doubleComplex out;
	
		
	switch(size){
		case 2 : out = zdiffs(zmuls(in[0],in[3]),zmuls(in[1],in[2]));
			 break;
		case 3 : /*regle de Sarrus*/
			 out=DoubleComplex(0,0);
			 /*Addition Part*/
			 tmp1 = zmuls(in[0],in[4]);
			 tmp2 = zmuls(tmp1,in[8]);
			 out = zadds(out,tmp2);
			 tmp1 = zmuls(in[1],in[5]);
			 tmp2 = zmuls(tmp1,in[6]);
			 out = zadds(out,tmp2);
			 tmp1 = zmuls(in[2],in[3]);
			 tmp2 = zmuls(tmp1,in[7]);
			 out = zadds(out,tmp2);
			 /*Subtraction Part*/
			 tmp1 = zmuls(in[0],in[5]);
			 tmp2 = zmuls(tmp1,in[7]);
			 out = zdiffs(out,tmp2);
			 tmp1 = zmuls(in[1],in[3]);
			 tmp2 = zmuls(tmp1,in[8]);
			 out = zdiffs(out,tmp2);
			 tmp1 = zmuls(in[2],in[4]);
			 tmp2 = zmuls(tmp1,in[6]);
			 out = zdiffs(out,tmp2);
			 break;
			 
		default : 
				  /*Copy the input matrix*/
			  inCopy=malloc((unsigned int)(size*size)*sizeof(doubleComplex));
			  for (i=0;i<size*size;i++) inCopy[i]=in[i];
			  
    			  for (i=0;i<size;i++){
				for (j=i+1;j<size;j++){
					pivot = zrdivs(inCopy[i*size+j],inCopy[i*size+i]);
					for (k=0;k<size-i;k++){
						inCopy[i*size+j+k*size]=zdiffs(inCopy[i*size+j+k*size],zmuls(pivot,inCopy[i*size+i+k*size]));
					}
				}
			  }
			  out=DoubleComplex(1,0);
			  for (i=0;i<size;i++){
			  	out = zmuls(out,inCopy[i*size+i]);
			  }
			  free(inCopy);
			  break;
	}
#endif
	return out;
	
}



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
 
 
/*Resolve the Yule-Walker equations:

       |r(0)   r(1)   ... r(N-1)|| a(1) | |sigma2|
       |r(1)   r(0)   ... r(n-1)|| a(2) | |  0   |
       |  :      :    ...    :  ||   :  |=|  0   |
       |  :      :    ...    :  ||   :  | |  0   |
       |r(N-1) r(N-2) ...  r(0) ||a(N-1)| |  0   |

  using Levinson's algorithm.
  r      :Correlation coefficients
  ar     :Auto-Regressive model parameters
  sigma2 :Scale constant
  rc     :Reflection coefficients 
*/


#include "lev.h"
#include "malloc.h"
#include "stdlib.h"


float sleva(float* in,int size, float* ar, float* rc){
	int i=0, j=0;
	float accu=0;
	float* ak1;
	float sigma2;
	
	/* FIXME : malloc here */ 
	ak1=(float*)malloc((unsigned int)size*sizeof(float));
	
	/* initialize levinson's algorithm */
	ar[0]=-in[1]/in[0];
	rc[0]=ar[0];
	sigma2=(1-ar[0]*ar[0])*in[0];
	ak1[0]=0;
	
	/* iterative solution to yule-walker equations */
	for (i=1;i<size-1;i++){
		accu=0;
		for (j=0;j<i;j++){
			accu+=ar[j]*in[i-j];
		}
		ak1[i]=-(in[i+1]+accu)/sigma2;
		rc[i]=ak1[i];
		for (j=0;j<i;j++){
			ak1[j] = ar[j]+ak1[i]*ar[i-1-j];
		}
		sigma2=(1-ak1[i]*ak1[i])*sigma2;
		for (j=0;j<=i;j++){
			ar[j]=ak1[j];
		}
	}	
	free(ak1);
	return sigma2;
}


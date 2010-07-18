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
#include "conj.h"
#include "multiplication.h"
#include "addition.h"
#include "division.h"
#include "subtraction.h"


floatComplex cleva(floatComplex* in,int size, floatComplex* ar, floatComplex* rc){
	int i=0, j=0;
	floatComplex accu=FloatComplex(0,0);
	floatComplex* ak1;
	floatComplex temp;
	floatComplex sigma2;
	
	/* FIXME : malloc here */ 
	ak1=(floatComplex*)malloc((unsigned int)size*sizeof(floatComplex));
	
	/* initialize levinson's algorithm */
	temp=crdivs(in[1],in[0]);
	ar[0]=FloatComplex(-creals(temp),-cimags(temp));
	
	rc[0]=ar[0];
	
	temp = cmuls(ar[0],cconjs(ar[0]));
	sigma2=cmuls(cdiffs(FloatComplex(1,0), temp), in[0]);
	
	ak1[0]=FloatComplex(0,0);
	
	/* iterative solution to yule-walker equations */
	for (i=1;i<size-1;i++){
		accu=FloatComplex(0,0);
		for (j=0;j<i;j++){
			temp = cmuls(cconjs(ar[j]),cconjs(in[i-j]));
			accu=cadds(accu, temp);
		}
		temp = cadds(in[i+1],accu);	
		temp = crdivs(temp,sigma2);
		ak1[i]=FloatComplex(-creals(temp),-cimags(temp));
		
		rc[i]=FloatComplex(creals(ak1[i]),cimags(ak1[i]));
		for (j=0;j<i;j++){
			temp = cmuls(ak1[i], cconjs(ar[i-1-j]));
			ak1[j] = cadds(ar[j],temp);
		}
		
		temp = cmuls(ak1[i], cconjs(ak1[i]));
		temp = cdiffs(FloatComplex(1,0),temp);
		sigma2 = cmuls(temp,sigma2);
		for (j=0;j<=i;j++){
			ar[j]=FloatComplex(creals(ak1[j]),cimags(ak1[j]));
		}
	}	
	free(ak1);
	return sigma2;
}

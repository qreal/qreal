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

doubleComplex zleva(doubleComplex* in,int size, doubleComplex* ar, doubleComplex* rc){
	int i=0, j=0;
	doubleComplex accu=DoubleComplex(0,0);
	doubleComplex* ak1;
	doubleComplex temp;
	doubleComplex sigma2;
	
	/* FIXME : malloc here */ 
	ak1=(doubleComplex*)malloc((unsigned int)size*sizeof(doubleComplex));
	
	/* initialize levinson's algorithm */
	temp=zrdivs(in[1],in[0]);
	ar[0]=DoubleComplex(-zreals(temp),-zimags(temp));
	
	rc[0]=ar[0];
	
	temp = zmuls(ar[0],zconjs(ar[0]));
	sigma2=zmuls(zdiffs(DoubleComplex(1,0), temp), in[0]);
	
	ak1[0]=DoubleComplex(0,0);
	
	/* iterative solution to yule-walker equations */
	for (i=1;i<size-1;i++){
		accu=DoubleComplex(0,0);
		for (j=0;j<i;j++){
			temp = zmuls(zconjs(ar[j]),zconjs(in[i-j]));
			accu=zadds(accu, temp);
		}
		temp = zadds(in[i+1],accu);	
		temp = zrdivs(temp,sigma2);
		ak1[i]=DoubleComplex(-zreals(temp),-zimags(temp));
		
		rc[i]=DoubleComplex(zreals(ak1[i]),zimags(ak1[i]));
		for (j=0;j<i;j++){
			temp = zmuls(ak1[i], zconjs(ar[i-1-j]));
			ak1[j] = zadds(ar[j],temp);
		}
		
		temp = zmuls(ak1[i], zconjs(ak1[i]));
		temp = zdiffs(DoubleComplex(1,0),temp);
		sigma2 = zmuls(temp,sigma2);
		for (j=0;j<=i;j++){
			ar[j]=DoubleComplex(zreals(ak1[j]),zimags(ak1[j]));
		}
	}	
	free(ak1);
	return sigma2;
}



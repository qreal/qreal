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

#include "levin.h"
#include "levinUtils.h"
#include "matrixInversion.h"
#include "matrixMultiplication.h"
#include "zeros.h"
#include <malloc.h>
#include <stdio.h>

void dlevina (int n, double* cov, int lCov, int cCov, double* la, double* sig, double* lb){
/*
	[la and lb]
	In Scilab, the return value la is a list of n elements. Each element is a matrix cCov*cCov,
	and each element of the matrix is a polynome whose degree is n, so the polynome got n+1 elements.
	The greatest size of a element of the list is : (n+1)*cCov*cCov.

	Here, la is a matrix which contain all elements of the list, its size is n*(n+1)*cCov*cCov. We take the
	maximum size for all elements.
	The first element of the list is the first (n+1)*cCov*cCov elements of la, namely la[0] to la[(n+1)*cCov*cCov-1].
	The second element of the list is the elements of la between (n+1)*cCov*cCov and 2*(n+1)*cCov*cCov,namely la[(n+1)*cCov*cCov] 
	to la[2*(n+1)*cCov*cCov-1],...

	Enter now in a element of the list. Take the first for example.
	This is, like said before, a cCov*cCov matrix. In la, it contains (n+1)*cCov*cCov. Each element of the matrix contains (n+1)
	elements. As it's stocked by columns, if we represent a matrix like [a   c], for example, the elements 0 to n of la represent 
							     					  [b   d]
	a, the elements (n+1) to 2(n+1)-1 represent b,...
	
	To finish, look at the elements of the matrix, the polynomes. The coefficients of the polynomes are stocked in increasing order.

	For example, if la in Scilab is  : list( [3+2x  5-2x ]) 
					     		   ( [-5+x  -2+4x])
					     		   ([3+x-x^2    -1+2x    ])
					     		   ([1+6x+3x^2  -2x-x^2  ])
		the result in dlevin will be : 
		-la is a table of 2*3*2*2 elements(n=2,cCov=2);
		-la[]={3,2,0,  -5,1,0,   5,-2,0,   -2,4,0,   3,1,-1,   1,6,3   -1,2,0,   0,-2,-1}.

	It's the same for lb.

	[sig]
	In Scilab, the return value sig is a list of n elements. Each element is a matrix cCov*cCov,
	and each element of the matrix is a scalar, so 1 element.
	The greatest size of a element of the list is : cCov*cCov.

	Let see an example so know how it's stocked.
	In Scilab, if sig is : list( [1  3]) 
				   	   ( [2  4])
				   	   ( [5  7])
				   	   ( [6  8])
	the result in dlevin will be :
		-sig={1,2,  5,6,  3,4,  7,8}.
	It's as if we put the matrix the ones under the others and we take the first column, the second,...

	>>>CAREFUL<<<
	la/lb and sig are stored differently 


*/


	int i=0;
	
	
/*version ISO C99	
	double tmp1[n*cCov*cCov], tmp2[n*cCov*cCov];
	double sig1[cCov], gam[cCov];
	double R1[n*cCov],R2[n*cCov],R3[n*cCov],R4[n*cCov];
*/	
/*version pas ISO C99	*/	
	double *tmp1, *tmp2;
	double *sig1, *gam;
	double *R1,*R2,*R3,*R4;

	/* FIXME : malloc here */

	tmp1=(double *)malloc((unsigned int)((n+1)*cCov*cCov)*sizeof(double));
	tmp2=(double *)malloc((unsigned int)((n+1)*cCov*cCov)*sizeof(double));
	sig1=(double *)malloc((unsigned int)(cCov*cCov)*sizeof(double));
	gam=(double *)malloc((unsigned int)(cCov*cCov)*sizeof(double));
	R1=(double *)malloc((unsigned int)(n*cCov*cCov)*sizeof(double));
	R2=(double *)malloc((unsigned int)(n*cCov*cCov)*sizeof(double));
	R3=(double *)malloc((unsigned int)(n*cCov*cCov)*sizeof(double));
	R4=(double *)malloc((unsigned int)(n*cCov*cCov)*sizeof(double));
	


	/* 
	 * Initializations
	 * */
	dzerosa(sig,n*cCov*cCov,1);
	dzerosa(la,n*(n+1)*cCov*cCov,1);
	dzerosa(lb,n*(n+1)*cCov*cCov,1);

	/*equal to eye(la) and eye(lb)
	  but we can't use eye cause to the indexation*/
	for (i=0;i<cCov;i++){
		la[i*((n+1)*(cCov+1))]=1;
		lb[i*((n+1)*(cCov+1))]=1;
	}
		
	dr1(cov,lCov,cCov,n,R1);
	dr2(cov,lCov,cCov,n,R2);
	dr3(cov,lCov,cCov,n,R3);
	dr4(cov,lCov,cCov,n,R4);

/* case i=0 */


	/*computation of sig */
	dlevinmul(la,R4,n,cCov,0,sig,n*cCov,0,'d');
	/*computation of gam1 */
	dlevinmul(lb,R2,n,cCov,0,gam,cCov,0,'u');	
	/*computation of c1*r1 */
	dlevinmul(la,R1,n,cCov,0,tmp1,cCov,0,'u');
	/*computation of inv(gam1) */
	dinverma(gam,sig1,cCov);
	/*computation of k1 = c1*r1*inv(gam1) */
	dmulma(tmp1,cCov,cCov,sig1,cCov,cCov,tmp2);
	/*computation of k1*lb */
	dlevinmul2(tmp2,lb,0,n,cCov,tmp1);
	/*computation of k1*lb*z */
	ddecalage(tmp1,0,n,cCov,tmp1);
	/*computation of la */
	dlevinsub(la,tmp1,n,cCov,0,0,la);

	/*computation of sig1 (we extract the value if sig at time 0)*/
	dlevinsig(sig,0,cCov,n*cCov,sig1);
	/*computation of c2*r3 */
	dlevinmul(lb,R3,n,cCov,0,tmp1,cCov,0,'d');
	/*computation of inv(sig1)*/
	dinverma(sig1,gam,cCov);
	/*computation of k2 = c2*r3*inv(sig1) */
	dmulma(tmp1,cCov,cCov,gam,cCov,cCov,tmp2);
	/*computation of k2*la (here it's lb cause la have been modified
	  and the precedent values hadn't been saved)*/
	dlevinmul2(tmp2,lb,0,n,cCov,tmp1);
	/*computation of lb*z */
	ddecalage(lb,0,n,cCov,lb);
	/*computation of lb */
	dlevinsub(lb,tmp1,n,cCov,0,0,lb);


	for (i=1;i<n;i++){
		dlevinmul(la,R4,n,cCov,i,sig,n*cCov,1,'d');
		dlevinmul(lb,R2,n,cCov,i,gam,cCov,0,'u');
		dlevinmul(la,R1,n,cCov,i,tmp1,cCov,0,'u');
		dinverma(gam,sig1,cCov);
		dmulma(tmp1,cCov,cCov,sig1,cCov,cCov,tmp2);
		
		dlevinmul2(tmp2,lb,i-1,n,cCov,tmp1);
		ddecalage(tmp1,0,n,cCov,tmp1);
		dlevinsub(la,tmp1,n,cCov,i,i,la);/*a*/

		/*computation of sig1 (we extract the value if sig at time i)*/
		dlevinsig(sig,i,cCov,n*cCov,sig1);		
		dlevinmul(lb,R3,n,cCov,i,tmp1,cCov,0,'d');
		dinverma(sig1,gam,cCov);
		dmulma(tmp1,cCov,cCov,gam,cCov,cCov,tmp2);
		/*computation of k2*la (now it's la at time (i-1))*/
		dlevinmul2(tmp2,la,i-1,n,cCov,tmp1);
		ddecalage(lb,(i-1)*(n+1)*cCov*cCov,n,cCov,tmp2);
		dlevinsub(tmp2,tmp1,n,cCov,0,i,lb);
	}


	free(R4);
	free(R3);
	free(R2);
	free(R1);
	free(gam);
	free(sig1);
	free(tmp2);
	free(tmp1);
}

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





/* creation of arrays used in levin */

void dr1(double *in,int lines,int columns, int n, double * out);
void dr2(double *in,int lines,int columns, int n, double * out);
void dr3(double *in,int lines,int columns, int n, double * out);
void dr4(double *in,int lines,int columns, int n, double * out);
void sr1(float *in,int lines,int columns, int n, float * out);
void sr2(float *in,int lines,int columns, int n, float * out);
void sr3(float *in,int lines,int columns, int n, float * out);
void sr4(float *in,int lines,int columns, int n, float * out);



/*multiplications used in levin program, differents from the classic multiplication*/
/*
	in1,in2 : matrices to multiply
	n : 
	columns : number of columns of in2
	ind_boucle : indice of the loop
	out : result matrix
	lines : number of lines of the result
	deb_out : 0 if the result start to the indice 0, 1 otherwise
	choix : 'u' or 'd', depends of in2. We take either the k first elements of in2 ('u') or the k last ('d'), 
		k is a nomber which depends of the indice of loop, the columns ... 
	*/
void dlevinmul(double* in1, double* in2, int n, int columns, int ind_boucle, double* out, int lines, int deb_out,char choix);
void dlevinmul2(double* in1, double *in2,int ind_boucle,int n,int columns,double* out);
void slevinmul(float* in1, float* in2, int n, int columns, int ind_boucle, float* out, int lines, int deb_out,char choix);
void slevinmul2(float* in1, float *in2,int ind_boucle,int n,int columns,float* out);


/*take the values of sig wanted*/
void dlevinsig(double *sig,int n, int columns, int lines, double *sig1);
void slevinsig(float *sig,int n, int columns, int lines, float *sig1);

/*a subtraction used in levin program, is different from the classic subtraction cause of the indices*/
/*
	in1,in2 : matrices to subtract
	n : 
	columns : number of columns of in2
	deb_in : place of the first element of in1
	ind_boucle : indice of the loop
	out : result matrix
	*/
void dlevinsub(double* in1, double* in2, int n, int columns, int deb_in, int ind_boucle, double* out);
void slevinsub(float* in1, float* in2, int n, int columns, int deb_in, int ind_boucle, float* out);


/*used for the multiplication by z which is equal to step forward*/
void ddecalage(double* in, int deb_in,int n,int columns,double * out);
void sdecalage(float* in, int deb_in,int n,int columns,float * out);





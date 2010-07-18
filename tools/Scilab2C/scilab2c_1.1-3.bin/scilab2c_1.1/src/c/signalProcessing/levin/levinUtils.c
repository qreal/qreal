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





#include "levinUtils.h"
#include "stdio.h"
/* Double Functions */
void dr1(double *in,int lines, int columns, int n, double * out){
	int i,j;
	for (i=0;i<n*columns;i++) {
		for (j=0;j<columns;j++){
			out[i+j*n*columns]=in[(n-i/columns)*columns + i%columns + j*lines];
		}
	}
}

void dr2(double *in,int lines, int columns, int n, double * out){
	int i,j;
	for (i=0;i<n*columns;i++) {
		for (j=0;j<columns;j++){
			out[i+j*n*columns]=in[(n-1-i/columns)*columns + i%columns + j*lines];
		}
	}
}


void dr3(double *in,int lines, int columns, int n, double * out){
	int i,j;
	for (i=0;i<n*columns;i++) {
		for (j=0;j<columns;j++){
			out[i+j*n*columns]=in[(i/columns+1)*columns + j%columns + (i%columns)*lines];
		}
	}
}


void dr4(double *in,int lines, int columns, int n, double * out){
	int i,j;
	for (i=0;i<n*columns;i++) {
		for (j=0;j<columns;j++){
			if ( (j*n*columns-1<i+j*n*columns) && (i+j*n*columns<j*n*columns+columns) ) 
				out[i+j*n*columns]=in[i+j*lines];
			else  out[i+j*n*columns]=in[i + j%columns + (i%columns)*(lines-1)];
		}
	}
}



void dlevinmul(double* in1, double* in2, int n, int columns, int ind_boucle, double* out, int lines, int deb_out,char choix){
	int i=0, j=0, debut=0,start_out=0;
	double accu;	
	
	if (ind_boucle==0) 	debut=0;
	else	debut = (ind_boucle-1)*(n+1)*columns*columns;

	start_out = deb_out*ind_boucle*columns;

	switch(choix){
		case 'u' : 
			for (i=0;i<columns*columns;i++){
				accu=0;
				for(j=0;j<ind_boucle*columns+columns;j++){
					accu += in1[debut+i%columns*(n+1) + (columns*(n+1))*(j%columns)+j/columns] * in2[n*columns-((ind_boucle+1)*columns)+j+(i/columns)*n*columns];
				}
				out[start_out+i%columns+(i/columns)*lines] = accu;
			}
			break;
		case 'd' : 
			for (i=0;i<columns*columns;i++){
				accu=0;
				for(j=0;j<ind_boucle*columns+columns;j++){	
					accu+=in1[debut+i%columns*(n+1) + (columns*(n+1))*(j%columns)+j/columns]*in2[j+(i/columns)*n*columns];	
				}
				out[start_out+i%columns+(i/columns)*lines] = accu;
			}
			break;
		default : break;
	}
}



void dlevinsig(double *sig,int n, int columns, int lines, double *sig1){
	int i=0;
	for (i=0;i<columns*columns;i++){
		sig1[i]=sig[n*columns + i%columns +(i/columns)*lines];
	}
}


void dlevinmul2(double* in1, double *in2,int ind_boucle,int n,int columns,double* out){
	int i=0,j=0, debut;
	double accu;
	debut = ind_boucle*(n+1)*columns*columns;
	for(i=0;i<(n+1)*columns*columns;i++){
		accu=0;
		for (j=0;j<columns;j++){
			accu += in1[(i/(n+1))%columns+j*columns]*in2[debut+i%(n+1)+(i/((n+1)*columns))*(n+1)*columns+j*(n+1)];
		}
		out[i]=accu;
	}
}


void dlevinsub(double* in1, double* in2, int n, int columns, int deb_in, int ind_boucle, double* out){
	int i=0;
	int deb_out= ind_boucle*(n+1)*columns*columns;
	int deb_in1;
	
	if (deb_in==0) deb_in1=0;
	else deb_in1=(deb_in-1)*(n+1)*columns*columns;

	for (i=0;i<(n+1)*columns*columns;i++){
		out[deb_out + i] = in1[deb_in1+i]-in2[i];
	}
}



void ddecalage(double* in, int deb_in,int n,int columns,double *out){
	int i=0,j=0;

	for (i=0;i<columns*columns;i++){
			for(j=n;j>0;j--) {out[i*(n+1)+j]=in[i*(n+1)+deb_in+j-1];}
			out[i*(n+1)]=0;
		}

}





/* Float Functions */

void sr1(float *in,int lines, int columns, int n, float * out){
	int i,j;
	for (i=0;i<n*columns;i++) {
		for (j=0;j<columns;j++){
			out[i+j*n*columns]=in[(n-i/columns)*columns + i%columns + j*lines];
		}
	}
}

void sr2(float *in,int lines, int columns, int n, float * out){
	int i,j;
	for (i=0;i<n*columns;i++) {
		for (j=0;j<columns;j++){
			out[i+j*n*columns]=in[(n-1-i/columns)*columns + i%columns + j*lines];
		}
	}
}


void sr3(float *in,int lines, int columns, int n, float * out){
	int i,j;
	for (i=0;i<n*columns;i++) {
		for (j=0;j<columns;j++){
			out[i+j*n*columns]=in[(i/columns+1)*columns + j%columns + (i%columns)*lines];
		}
	}
}


void sr4(float *in,int lines, int columns, int n, float * out){
	int i,j;
	for (i=0;i<n*columns;i++) {
		for (j=0;j<columns;j++){
			if ( (j*n*columns-1<i+j*n*columns) && (i+j*n*columns<j*n*columns+columns) ) 
				out[i+j*n*columns]=in[i+j*lines];
			else  out[i+j*n*columns]=in[i + j%columns + (i%columns)*(lines-1)];
		}
	}
}



void slevinmul(float* in1, float* in2, int n, int columns, int ind_boucle, float* out, int lines, int deb_out,char choix){
	int i=0, j=0, debut=0,start_out=0;
	float accu;	
	
	if (ind_boucle==0) 	debut=0;
	else	debut = (ind_boucle-1)*(n+1)*columns*columns;

	start_out = deb_out*ind_boucle*columns;

	switch(choix){
		case 'u' : 
			for (i=0;i<columns*columns;i++){
				accu=0;
				for(j=0;j<ind_boucle*columns+columns;j++){
					accu += in1[debut+i%columns*(n+1) + (columns*(n+1))*(j%columns)+j/columns] * in2[n*columns-((ind_boucle+1)*columns)+j+(i/columns)*n*columns];
				}
				out[start_out+i%columns+(i/columns)*lines] = accu;
			}
			break;
		case 'd' : 
			for (i=0;i<columns*columns;i++){
				accu=0;
				for(j=0;j<ind_boucle*columns+columns;j++){	
					accu+=in1[debut+i%columns*(n+1) + (columns*(n+1))*(j%columns)+j/columns]*in2[j+(i/columns)*n*columns];	
				}
				out[start_out+i%columns+(i/columns)*lines] = accu;
			}
			break;
		default : break;
	}
}



void slevinsig(float *sig,int n, int columns, int lines, float *sig1){
	int i=0;
	for (i=0;i<columns*columns;i++){
		sig1[i]=sig[n*columns + i%columns +(i/columns)*lines];
	}
}


void slevinmul2(float* in1, float *in2,int ind_boucle,int n,int columns,float* out){
	int i=0,j=0, debut;
	float accu;
	debut = ind_boucle*(n+1)*columns*columns;
	for(i=0;i<(n+1)*columns*columns;i++){
		accu=0;
		for (j=0;j<columns;j++){
			accu += in1[(i/(n+1))%columns+j*columns]*in2[debut+i%(n+1)+(i/((n+1)*columns))*(n+1)*columns+j*(n+1)];
		}
		out[i]=accu;
	}
}


void slevinsub(float* in1, float* in2, int n, int columns, int deb_in, int ind_boucle, float* out){
	int i=0;
	int deb_out= ind_boucle*(n+1)*columns*columns;
	int deb_in1;
	
	if (deb_in==0) deb_in1=0;
	else deb_in1=(deb_in-1)*(n+1)*columns*columns;

	for (i=0;i<(n+1)*columns*columns;i++){
		out[deb_out + i] = in1[deb_in1+i]-in2[i];
	}
}


void sdecalage(float* in, int deb_in,int n,int columns,float *out){
	int i=0,j=0;

	for (i=0;i<columns*columns;i++){
			for(j=n;j>0;j--) {out[i*(n+1)+j]=in[i*(n+1)+deb_in+j-1];}
			out[i*(n+1)]=0;
		}

}


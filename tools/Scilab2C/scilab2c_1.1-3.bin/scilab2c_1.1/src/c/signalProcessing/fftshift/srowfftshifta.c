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


#include "fftshift.h"

static void Pair(float* in,int rows,int columns,float* out){
	int i=0,j=0;
	
	for (i=0;i<rows;i++)
		for(j=0;j<columns;j++)
			out[i+j*rows]=in[j*rows+((rows/2)+i)%rows];
}


static void Odd(float* in,int rows,int columns,float* out){
	int i=0,j=0;

	/*
		same as Pair with some change in the indice
	*/
	for (i=0;i<rows;i++)
		for(j=0;j<columns;j++)
			out[i+j*rows]=in[j*rows+(((rows+1)/2)+i)%rows];
}






void srowfftshifta(float* in,int rows,int columns,float* out){	
	switch(rows%2){
		case 0 : Pair(in,rows,columns,out); break; 
		case 1 : Odd(in,rows,columns,out); break; 
		default : break;
	}
}


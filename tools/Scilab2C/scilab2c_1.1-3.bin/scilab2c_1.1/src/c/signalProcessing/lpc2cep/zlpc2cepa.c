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


#include "lpc2cep.h"
#include "fft.h"
#include "ifft.h"
#include "logm.h"
 
void zlpc2cepa(doubleComplex* in, int size, doubleComplex* out){
	int i;

	for (i=0;i<size*size;i++)
		out[i]=in[i];
	

	zfftma(out,size, size, out);
	zlogma(out,size,out);
	zifftma(out,size, size, out);

}


 
 

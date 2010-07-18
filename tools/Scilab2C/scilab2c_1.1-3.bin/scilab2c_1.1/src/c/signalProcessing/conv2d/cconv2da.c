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

#include "conv2d.h"
#include "multiplication.h"
#include "addition.h"

void cconv2da(floatComplex *in1, int lines1, int columns1, floatComplex *in2, int lines2, int columns2, floatComplex *out){
	int i,j,k1,k2;
	floatComplex accu;
	
	for (i=0;i<lines1+lines2-1;i++){
		for (j=0;j<columns1+columns2-1;j++){
			accu=FloatComplex(0,0);
			for (k1=0;k1<i+1;k1++){
				if ( (k1<lines1)&&((i-k1)<lines2) ){
					for(k2=0;k2<j+1;k2++){
						if ( (k2<columns1)&&((j-k2)<columns2) )
							accu = cadds(accu , cmuls(in1[k1+k2*lines1],in2[i-k1+(j-k2)*lines2]));
					}
				}
			}
			out[i+j*(lines1+lines2-1)]=accu;
		}
	}		
}



/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan SIMON
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#define IFFT842 1
#define DFFT2  0

#include <malloc.h>
#include <math.h>
#include "ifft.h"
#include "lapack.h"
#include "ifft_internal.h"


void zifftma ( doubleComplex* in , int rows, int cols, doubleComplex* out)
{

	int choosenAlgo = DFFT2 ;

	int size = rows*cols ;
	int sizeTemp = 0;

	int rowsTemp = 0 ;
	int colsTemp = 0 ;

	int ierr = 0 ;
	int isn = 1;
	int i = 0;

	int increment=1;

	double* realIn = (double*) malloc ( sizeof (double) * (unsigned int) size );
	double* imagIn = (double*) malloc ( sizeof (double) * (unsigned int) size );
	doubleComplex* inCopy = (doubleComplex*) malloc ( sizeof (doubleComplex) * (unsigned int) size*2 );

	doubleComplex* inTemp = (doubleComplex*) malloc ( sizeof (doubleComplex) * (unsigned int) size*2 );



	zimaga ( in , size , imagIn) ;
	zreala ( in , size , realIn) ;
	for (i=0;i<size;i++) inCopy[i]=in[i];

	if ( rows  ==  1 || cols == 1 )
	{
		/*test if size is a power of 2*/
		sizeTemp = (int) pow(2.0, (int) (log(size + 0.5) / log(2.0)));


		if ( size == sizeTemp )
		{
			if ( size <=  pow(2.0, 15.0))
			{
				ifft842 ( inCopy , size  , 1 );
				choosenAlgo = IFFT842 ;
			}
			else
			{
				difft2 ( realIn , imagIn , 1 , size , 1 , isn , ierr);
			}


		}
		else
		{
			difft2 ( realIn , imagIn , 1 , size , 1 , isn , ierr);
		}

	}

	else
	{
		/*test if rows and/or cols is a power of 2*/
		rowsTemp = (int) pow(2.0, (int)(log(rows + 0.5) / log(2.0))) ;
		colsTemp = (int) pow(2.0 ,(int)(log(cols + 0.5) / log(2.0))) ;


		if (rows == rowsTemp)
		{
			if (rows <=  pow(2.0, 15.0))
			{
				for ( i = 0 ; i < cols ; i++ )
				{
					ifft842 ( &inCopy[ rows*i] , rows , 1);
					zimaga ( inCopy , size , imagIn) ;
					zreala ( inCopy , size , realIn) ;
				}
			}
			else
			{
				difft2 ( realIn, imagIn ,cols , rows , 1 , isn , ierr);
				inCopy=DoubleComplexMatrix(realIn,imagIn,size);
			}
		}
		else
		{
			difft2 ( realIn, imagIn ,cols , rows , 1 , isn , ierr);
			inCopy=DoubleComplexMatrix(realIn,imagIn,size);
		}

		/*second call*/
		if ( colsTemp == cols )
		{
			if ( cols <= pow(2.0, 15.0) )
			{
				/*compute the fft on each line of the matrix */
				for (i = 0 ; i < rows ; i++ )
				{
					C2F(zcopy) ( &cols, inCopy + i, &rows, inTemp , &increment );

					ifft842( inTemp , cols , 1);
					choosenAlgo = IFFT842 ;
					C2F(zcopy) ( &cols, inTemp , &increment, inCopy + i, &rows);

				}
			}
			else
			{
				difft2 ( realIn, imagIn, 1, cols, rows, isn, ierr);
			}
		}
		else
		{
			difft2 ( realIn, imagIn, 1, cols, rows, isn, ierr);
		}

	}



	if ( choosenAlgo == IFFT842 )
	{
		for ( i = 0 ; i < size ; i++)
		{
			out[i] = DoubleComplex ( zreals(inCopy[i]) , zimags(inCopy[i]) );
		}
	}
	else
	{
		for ( i = 0 ; i < size ; i++)
		{
			out[i] = DoubleComplex ( realIn[i] , imagIn[i] );
		}

	}

  free(realIn);
  free(imagIn);
  free(inCopy);
  free(inTemp);


}

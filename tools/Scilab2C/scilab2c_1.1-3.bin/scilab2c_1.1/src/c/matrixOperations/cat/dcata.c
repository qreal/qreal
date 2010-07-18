/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "cat.h"
/*
** Emulate Scilab (from help cat) :
** then the concatenation is done according to the rows
**
** A1=[1 2 3 ; 4 5 6]; A2=[7 8 9 ; 10 11 12]; y=cat(1,A1,A2) => y=[1 2 3 ; 4 5 6 ;7 8 9; 10 11 12]
**
*/

void		drowcata(double *in1, int lines1, int columns1, double *in2, int lines2, int columns2, double* out) {
  int i = 0;
  int j = 0;
  
  for (i = 0 ; i < columns1 && i < columns2 ; ++i)
    {
      for (j = 0 ; j < lines1 ; ++j)
	{
	  out[i*(lines1 + lines2) + j] = in1[i*lines1 + j];
	}
       for (j = 0 ; j < lines2 ; ++j)
	{
	  out[i*(lines1 + lines2) + lines1 + j] = in2[i*lines2 + j];
	}
    }
}


/*
** Emulate Scilab (from help cat) :
** the concatenation is done according to the columns
**
** A1=[1 2 3;4 5 6]; A2=[7 8 9 ;10 11 12]; y=cat(2,A1,A2) => y=[1 2 3 7 8 9 ; 4 5 6 10 11 12]
**
*/
void		dcolumncata(double *in1, int lines1, int columns1, double *in2, int lines2, int columns2, double* out) {
  int i = 0;
  for (i = 0 ; i < lines1 * columns1 ; ++i) {
    out[i] = in1[i];
  }
  for (i = 0; i < lines2 * columns2 ; ++i) {
    out[i + lines1 * columns1] = in2[i];
  }
}

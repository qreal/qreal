/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2006-2007 - INRIA - Bruno JOFRET
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

/*
   Update 23/02/09 by Arnaud Torset : Add matrix comparaison, remove include(floatComplex and doubleComplex)
*/

#ifndef __OPLOGGE_H__
#define __OPLOGGE_H__


#define s0s0OpLogGes0(in1,in2)				(float)  (in1 >= in2)
#define d0d0OpLogGed0(in1,in2)				(double)  (in1 >= in2)


#define s2s0OpLogGes2(in1, size1, in2, out)		{int i;\
  for(i = 0 ; i < size1[0] * size1[1] ; ++i) out[i] = s0s0OpLogGes0(in1[i],in2);\
  }

#define s0s2OpLogGes2(in1, in2, size2, out)		{int i;		\
  for(i = 0 ; i < size2[0] * size2[1] ; ++i) out[i] = s0s0OpLogGes0(in1,in2[i]);\
  }

/* we must have size1=size2 */
#define s2s2OpLogGes2(in1, size1, in2, size2, out)		{int i;	\
  for(i = 0 ; i < size1[0] * size1[1] ; ++i) out[i] = s0s0OpLogGes0(in1[i],in2[i]);\
  }

#define d2d0OpLogGed2(in1, size1, in2, out)		{int i;\
  for(i = 0 ; i < size1[0] * size1[1] ; ++i) out[i] = d0d0OpLogGed0(in1[i],in2);\
  }

#define d0d2OpLogGed2(in1, in2, size2, out)		{int i;		\
  for(i = 0 ; i < size2[0] * size2[1] ; ++i) out[i] = d0d0OpLogGed0(in1,in2[i]);\
  }

/* we must have size1=size2 */
#define d2d2OpLogGed2(in1, size1, in2, size2, out)		{int i;	\
  for(i = 0 ; i < size1[0] * size1[1] ; ++i) out[i] = d0d0OpLogGed0(in1[i],in2[i]);\
  }
#endif /* !__OPLOGGE_H__ */

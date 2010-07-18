/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* THIS IS AN AUTOMATICALLY GENERATED FILE : DO NOT EDIT BY HAND. */

#ifndef __INT_OPEXT_H__
#define __INT_OPEXT_H__

#define s2s0OpExts0(in1,size,in2)		in1[((int)in2)-1];
#define d2d0OpExtd0(in1,size,in2)		in1[((int)in2)-1];
#define c2s0OpExtc0(in1,size,in2)		in1[((int)in2)-1];
#define z2d0OpExtz0(in1,size,in2)		in1[((int)in2)-1];


#define s2s0s0OpExts0(in1,size,row,col)		in1[(((int) col)-1)*size[0]+((int) row)-1];
#define d2d0d0OpExtd0(in1,size,row,col)		in1[(((int) col)-1)*size[0]+((int) row)-1];
#define c2s0s0OpExtc0(in1,size,row,col)		in1[(((int) col)-1)*size[0]+((int) row)-1];
#define z2d0d0OpExtz0(in1,size,row,col)		in1[(((int) col)-1)*size[0]+((int) row)-1];


#define s2s2OpExts2(in1,size1,in2,size2,out)	{int i;			\
    for (i=0;i<size2[0]*size2[1];i++) out[i]=in1[(int)in2[i]-1];	\
  }
								
#define d2d2OpExtd2(in1,size1,in2,size2,out)	{int i;			\
    for (i=0;i<size2[0]*size2[1];i++) out[i]=in1[(int)in2[i]-1];	\
  }
								
#define c2s2OpExtc2(in1,size1,in2,size2,out)	{int i;			\
    for (i=0;i<size2[0]*size2[1];i++) out[i]=in1[(int)in2[i]-1];	\
  }
								
#define z2d2OpExtz2(in1,size1,in2,size2,out)	{int i;			\
    for (i=0;i<size2[0]*size2[1];i++) out[i]=in1[(int)in2[i]-1];	\
  }


#define s2s2s0OpExts2(in1,size1,rows,size2,col,out)	{int i;		\
    for (i=0;i<size2[0]*size2[1];i++) out[i]=s2s0s0OpExts0(in1,size1,(int)rows[i],((int) col));	\
  }
								
#define d2d2d0OpExtd2(in1,size1,rows,size2,col,out)	{int i;		\
    for (i=0;i<size2[0]*size2[1];i++) out[i]=d2d0d0OpExtd0(in1,size1,(int)rows[i],((int) col));	\
  }
								
#define c2s2s0OpExtc2(in1,size1,rows,size2,col,out)	{int i;		\
    for (i=0;i<size2[0]*size2[1];i++) out[i]=c2s0s0OpExtc0(in1,size1,(int)rows[i],((int) col));	\
  }
								
#define z2d2d0OpExtz2(in1,size1,rows,size2,col,out)	{int i;		\
    for (i=0;i<size2[0]*size2[1];i++) out[i]=z2d0d0OpExtz0(in1,size1,(int)rows[i],((int) col));	\
  }
									
#define s2s0s2OpExts2(in1,size1,row,cols,size2,out)	{int i;		\
    for (i=0;i<size2[0]*size2[1];i++) out[i]=s2s0s0OpExts0(in1,size1,((int) row),(int)cols[i]);	\
  }
								
#define d2d0d2OpExtd2(in1,size1,row,cols,size2,out)	{int i;		\
    for (i=0;i<size2[0]*size2[1];i++) out[i]=d2d0d0OpExtd0(in1,size1,((int) row),(int)cols[i]);	\
  }
								
#define c2s0s2OpExtc2(in1,size1,row,cols,size2,out)	{int i;		\
    for (i=0;i<size2[0]*size2[1];i++) out[i]=c2s0s0OpExtc0(in1,size1,((int) row),(int)cols[i]);	\
  }
								
#define z2d0d2OpExtz2(in1,size1,row,cols,size2,out)	{int i;		\
    for (i=0;i<size2[0]*size2[1];i++) out[i]=z2d0d0OpExtz0(in1,size1,((int) row),(int)cols[i]);	\
  }
								
								
#define s2s2s2OpExts2(in1,size1,rows,size2,cols,size3,out)	{int i,j; \
    for (i=0;i<size2[0]*size2[1];i++)					\
      for (j=0;j<size3[0]*size3[1];j++)					\
	out[i+j*size2[0]*size2[1]] = in1[((int)(cols[j])-1)*size1[0]+(int)(rows[i])-1];	\
  }
								
#define d2d2d2OpExtd2(in1,size1,rows,size2,cols,size3,out)	{int i,j; \
    for (i=0;i<size2[0]*size2[1];i++)					\
      for (j=0;j<size3[0]*size3[1];j++)					\
	out[i+j*size2[0]*size2[1]] = in1[((int)(cols[j])-1)*size1[0]+(int)(rows[i])-1];	\
  }
								
#define c2s2s2OpExtc2(in1,size1,rows,size2,cols,size3,out)	{int i,j; \
    for (i=0;i<size2[0]*size2[1];i++)					\
      for (j=0;j<size3[0]*size3[1];j++)					\
	out[i+j*size2[0]*size2[1]] = in1[((int)(cols[j])-1)*size1[0]+(int)(rows[i])-1];	\
  }
								
#define z2d2d2OpExtz2(in1,size1,rows,size2,cols,size3,out)	{int i,j; \
    for (i=0;i<size2[0]*size2[1];i++)					\
      for (j=0;j<size3[0]*size3[1];j++)					\
	out[i+j*size2[0]*size2[1]] = in1[((int)(cols[j])-1)*size1[0]+(int)(rows[i])-1];	\
  }							

#endif /* !__INT_OPEXT_H__ */

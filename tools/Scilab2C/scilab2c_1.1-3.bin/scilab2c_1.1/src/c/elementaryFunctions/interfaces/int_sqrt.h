/*
**  -*- C -*-
**
** int_sqrt.h
** Made by  Raffaele Nutricato <raffaele.nutricato@tiscali.it>
**
**
** Copyright POLIBA 2008
*/
/*
   1. Search for sqrt in INIT_FillSCI2LibCDirs.sci
   2. Search for // --- Function List Class. --- in INIT_FillSCI2LibCDirs.sci
   3. You will find
      PrintStringInfo('s0'+ArgSeparator+'s0',ClassFileName,'file','y');
      PrintStringInfo('s0'+ArgSeparator+'c0',ClassFileName,'file','y');
      PrintStringInfo('d0'+ArgSeparator+'d0',ClassFileName,'file','y');
      PrintStringInfo('d0'+ArgSeparator+'z0',ClassFileName,'file','y');
      PrintStringInfo('c0'+ArgSeparator+'c0',ClassFileName,'file','y');
      PrintStringInfo('z0'+ArgSeparator+'z0',ClassFileName,'file','y');

      PrintStringInfo('s2'+ArgSeparator+'s2',ClassFileName,'file','y');
      PrintStringInfo('s2'+ArgSeparator+'c2',ClassFileName,'file','y');
      PrintStringInfo('d2'+ArgSeparator+'d2',ClassFileName,'file','y');
      PrintStringInfo('d2'+ArgSeparator+'z2',ClassFileName,'file','y');
      PrintStringInfo('c2'+ArgSeparator+'c2',ClassFileName,'file','y');
      PrintStringInfo('z2'+ArgSeparator+'z2',ClassFileName,'file','y');
   4. These are all the functions to be implemented.
   5. According to functions available in src you have to generate the mapping between
      functions in step3 and functions in src.
*/

#ifndef __INT_SQRT_H__
#define __INT_SQRT_H__

#define s0sqrts0(in)		ssqrts(in)

#define s0sqrtc0(in)		csqrts(FloatComplex(in,0))

#define d0sqrtd0(in)		dsqrts(in)

#define d0sqrtz0(in)		zsqrts(DoubleComplex(in,0))

#define c0sqrtc0(in)		csqrts(in)

#define z0sqrtz0(in)		zsqrts(in)

#define s2sqrts2(in,size,out)		ssqrta(in, size[0]*size[1], out)

#define s2sqrtc2(in,size,out)		csqrta(FloatComplexMatrix(in,0), size[0]*size[1], out)

#define d2sqrtd2(in,size,out)		dsqrta(in, size[0]*size[1], out)

#define d2sqrtz2(in,size,out)		zsqrta(DoubleComplexMatrix(in,0), size[0]*size[1], out)

#define c2sqrtc2(in,size,out)		csqrta(in, size[0]*size[1], out)

#define z2sqrtz2(in,size,out)		zsqrta(in, size[0]*size[1], out)

#endif /* !__INT_SQRT_H__ */

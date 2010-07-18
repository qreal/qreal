/*
**  -*- C -*-
**
** int_length.h
** Made by  Raffaele Nutricato <raffaele.nutricato@tiscali.it>
**
**
** Copyright POLIBA 2008
*/
/*
   1. Search for length in INIT_FillSCI2LibCDirs.sci
   2. Search for // --- Function List Class. --- in INIT_FillSCI2LibCDirs.sci
   3. You will find
      PrintStringInfo('s0'+ArgSeparator+'s0',ClassFileName,'file','y');
      PrintStringInfo('d0'+ArgSeparator+'d0',ClassFileName,'file','y');
      PrintStringInfo('c0'+ArgSeparator+'s0',ClassFileName,'file','y');
      PrintStringInfo('z0'+ArgSeparator+'d0',ClassFileName,'file','y');
      PrintStringInfo('g0'+ArgSeparator+'s0',ClassFileName,'file','y');
      PrintStringInfo('g0'+ArgSeparator+'d0',ClassFileName,'file','y');

      PrintStringInfo('s2'+ArgSeparator+'s0',ClassFileName,'file','y');
      PrintStringInfo('d2'+ArgSeparator+'d0',ClassFileName,'file','y');
      PrintStringInfo('c2'+ArgSeparator+'s0',ClassFileName,'file','y');
      PrintStringInfo('z2'+ArgSeparator+'d0',ClassFileName,'file','y');
      PrintStringInfo('g2'+ArgSeparator+'s0',ClassFileName,'file','y');
      PrintStringInfo('g2'+ArgSeparator+'d0',ClassFileName,'file','y');
   4. These are all the functions to be implemented.
   5. According to functions available in src you have to generate the mapping between
      functions in step3 and functions in src.
*/

#ifndef __INT_LENGTH_H__
#define __INT_LENGTH_H__

#define s0lengths0(in) \
slengths(in)

#define d0lengthd0(in) \
dlengths(in)

#define c0lengths0(in) \
clengths(in)

#define z0lengthd0(in) \
zlengths(in)

#define g0lengths0(in) \
slengths(in)

#define g0lengthd0(in) \
dlengths(in)

#define s2lengths0(inptr,insizeptr) \
slengtha(inptr, insizeptr[0]*insizeptr[1])

#define d2lengthd0(inptr,insizeptr) \
dlengtha(inptr, insizeptr[0]*insizeptr[1])

#define c2lengths0(inptr,insizeptr) \
clengtha(inptr, insizeptr[0]*insizeptr[1])

#define z2lengthd0(inptr,insizeptr) \
zlengtha(inptr, insizeptr[0]*insizeptr[1])

#define g2lengths0(inptr,insizeptr) \
slengtha(inptr, insizeptr[0]*insizeptr[1])

#define g2lengthd0(inptr,insizeptr) \
dlengtha(inptr, insizeptr[0]*insizeptr[1])
#endif /* !__INT_LENGTH_H__ */

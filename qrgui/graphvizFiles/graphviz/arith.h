/* $Id: arith.h,v 1.16 2009/06/03 01:10:51 ellson Exp $ $Revision: 1.16 $ */
/* vim:set shiftwidth=4 ts=8: */

/**********************************************************
*      This software is part of the graphviz package      *
*                http://www.graphviz.org/                 *
*                                                         *
*            Copyright (c) 1994-2004 AT&T Corp.           *
*                and is licensed under the                *
*            Common Public License, Version 1.0           *
*                      by AT&T Corp.                      *
*                                                         *
*        Information and Software Systems Research        *
*              AT&T Research, Florham Park NJ             *
**********************************************************/

/* geometric functions (e.g. on points and boxes) with application to, but
 * no specific dependance on graphs */

#ifndef GV_ARITH_H
#define GV_ARITH_H

/* for sincos */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include <limits.h>
#ifdef HAVE_VALUES_H
#include <values.h>
#endif
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MIN
#undef MIN
#endif
#define MIN(a,b)	((a)<(b)?(a):(b))

#ifdef MAX
#undef MAX
#endif
#define MAX(a,b)	((a)>(b)?(a):(b))

#ifdef ABS
#undef ABS
#endif
#define ABS(a)		((a) >= 0 ? (a) : -(a))

#define AVG(a,b)	((a + b) / 2)
#define SGN(a)		(((a)<0)? -1 : 1)
#define CMP(a,b)	(((a)<(b)) ? -1 : (((a)>(b)) ? 1 : 0))

#ifndef INT_MAX
#define	INT_MAX		((int)(~(unsigned)0 >> 1))
#endif

#ifndef INT_MIN
#define INT_MIN		(-INT_MAX - 1)
#endif

#ifndef MAXSHORT
#define	MAXSHORT	(0x7fff)
#endif

#ifndef MAXDOUBLE
#define MAXDOUBLE	1.7976931348623157e+308
#endif

#ifndef MAXFLOAT
#define MAXFLOAT	((float)3.40282347e+38)
#endif

#ifdef BETWEEN
#undef BETWEEN
#endif
#define BETWEEN(a,b,c)	(((a) <= (b)) && ((b) <= (c)))

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

#ifndef SQRT2
#define SQRT2		1.41421356237309504880
#endif

#define ROUND(f)        ((f>=0)?(int)(f + .5):(int)(f - .5))
#define RADIANS(deg)	((deg)/180.0 * M_PI)
#define DEGREES(rad)	((rad)/M_PI * 180.0)

#define SQR(a) ((a) * (a))

#ifdef HAVE_SINCOS
    extern void sincos(double x, double *s, double *c);
#else
# define sincos(x,s,c) *s = sin(x); *c = cos(x)
#endif

#ifdef __cplusplus
}
#endif

#endif

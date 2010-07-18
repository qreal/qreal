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

/**
 ** \brief Return scilab code encoding for data type.
 **/

/**
 ** WARNING :
 ** We use #define to avoid compilation warnings
 ** such as "unused parameter" and better performance.
 **/

#ifndef __TYPE_H__
#define __TYPE_H__

#define REAL_FLOAT_CONSTANT_MATRIX		1.0f
#define REAL_DOUBLE_CONSTANT_MATRIX		1.0

#define COMPLEX_FLOAT_CONSTANT_MATRIX		1.0f
#define COMPLEX_DOUBLE_CONSTANT_MATRIX		1.0

/**
 ** Float scalar.
 **/
#define	stypes(in)		REAL_FLOAT_CONSTANT_MATRIX

/**
 ** Double scalar.
 **/
#define	dtypes(in)		REAL_DOUBLE_CONSTANT_MATRIX

/**
 ** Float complex scalar.
 **/
#define	ctypes(in)		COMPLEX_FLOAT_CONSTANT_MATRIX

/**
 ** Double complex scalar.
 **/
#define	ztypes(in)		COMPLEX_DOUBLE_CONSTANT_MATRIX

/**
 ** Float array.
 **/
#define	stypea(in, size)	REAL_FLOAT_CONSTANT_MATRIX

/**
 ** Double array.
 **/
#define	dtypea(in, size)	REAL_DOUBLE_CONSTANT_MATRIX

/**
 ** Float complex array.
 **/
#define	ctypea(in, size)	COMPLEX_FLOAT_CONSTANT_MATRIX

/**
 ** Double complex array.
 **/
#define	ztypea(in, size)	COMPLEX_DOUBLE_CONSTANT_MATRIX

#endif /* !__TYPE_H__ */

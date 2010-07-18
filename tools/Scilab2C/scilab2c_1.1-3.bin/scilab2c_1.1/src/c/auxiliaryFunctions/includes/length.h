/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *  Copyright (C) 2007-2008 - POLIBA - Raffaele Nutricato
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __LENGTH_H__
#define __LENGTH_H__

#include "dynlib_auxiliaryfunctions.h"

/**
 ** WARNING :
 ** We assume size of arrays are known, so we
 ** use #define to avoid compilation warnings
 ** such as "unused parameter"
 **/

/**
 ** \brief Float Size Scalar function
 ** Determine the size of an array.
 ** \param in : the float array we must determine size.
 ** \param size : the number of elements.
 ** \return the size of in.
 **/
#define	slengths(in)		1.0f

/**
 ** \brief Float length Scalar function
 ** Determine the length of an array.
 ** \param in : the float array we must determine length.
 ** \param length : the number of elements.
 ** \return the length of in.
 **/
#define	dlengths(in)		1.0

/**
 ** \brief Complex Float length Scalar function
 ** Determine the length of an array.
 ** \param in : the float array we must determine length.
 ** \param length : the number of elements.
 ** \return the length of in.
 **/
#define	clengths(in)		1.0f

/**
 ** \brief Complex Double length Array function
 ** Determine the length of an array.
 ** \param in : the float array we must determine length.
 ** \param length : the number of elements.
 ** \return the length of in.
 **/
#define	zlengths(in)		1.0

/**
 ** \brief Float length Array function
 ** Determine the length of an array.
 ** \param in : the float array we must determine length.
 ** \param length : the number of elements.
 ** \return the length of in.
 **/
#define	slengtha(in, size)	(float)size

/**
 ** \brief Double length Array function
 ** Determine the length of an array.
 ** \param in : the float array we must determine length.
 ** \param length : the number of elements.
 ** \return the length of in.
 **/
#define	dlengtha(in, size)	(double)size

/**
 ** \brief Complex Float length Array function
 ** Determine the length of an array.
 ** \param in : the float array we must determine length.
 ** \param length : the number of elements.
 ** \return the length of in.
 **/
#define	clengtha(in, size)	(float)size

/**
 ** \brief Complex Double length Array function
 ** Determine the length of an array.
 ** \param in : the float array we must determine length.
 ** \param length : the number of elements.
 ** \return the length of in.
 **/
#define	zlengtha(in, size)	(double)size

#endif /* !__LENGTH_H__ */

/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SIZE_H__
#define __SIZE_H__

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
#define	ssizes(in)		1.0f

/**
 ** \brief Float Size Scalar function
 ** Determine the size of an array.
 ** \param in : the float array we must determine size.
 ** \param size : the number of elements.
 ** \return the size of in.
 **/
#define	dsizes(in)		1.0

/**
 ** \brief Complex Float Size Scalar function
 ** Determine the size of an array.
 ** \param in : the float array we must determine size.
 ** \param size : the number of elements.
 ** \return the size of in.
 **/
#define	csizes(in)		1.0f

/**
 ** \brief Complex Double Size Array function
 ** Determine the size of an array.
 ** \param in : the float array we must determine size.
 ** \param size : the number of elements.
 ** \return the size of in.
 **/
#define	zsizes(in)		1.0

/**
 ** \brief Float Size Array function
 ** Determine the size of an array.
 ** \param in : the float array we must determine size.
 ** \param size : the number of elements.
 ** \return the size of in.
 **/
#define	ssizea(in, size)	size

/**
 ** \brief Double Size Array function
 ** Determine the size of an array.
 ** \param in : the float array we must determine size.
 ** \param size : the number of elements.
 ** \return the size of in.
 **/
#define	dsizea(in, size)	size

/**
 ** \brief Complex Float Size Array function
 ** Determine the size of an array.
 ** \param in : the float array we must determine size.
 ** \param size : the number of elements.
 ** \return the size of in.
 **/
#define	csizea(in, size)	size

/**
 ** \brief Complex Double Size Array function
 ** Determine the size of an array.
 ** \param in : the float array we must determine size.
 ** \param size : the number of elements.
 ** \return the size of in.
 **/
#define	zsizea(in, size)	size

#endif /* !__SIZE_H__ */

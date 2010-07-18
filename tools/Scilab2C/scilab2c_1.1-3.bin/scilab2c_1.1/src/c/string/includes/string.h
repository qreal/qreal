/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Allan SIMON
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */



#ifndef __STRING_H__
#define __STRING_H__

#include <stdio.h>
#include "dynlib_string.h"
#include "floatComplex.h"
#include "doubleComplex.h"

#ifdef  __cplusplus
extern "C" {
#endif

/*
** \brief convert of a float scalar into a char array
** \param in the float scalar to convert
** \param out the output char array
*/
EXTERN_STRING void sstrings (float in, char* out);

/*
** \brief convert of a double scalar into a char array
** \param in the double scalar to convert
** \param out the output char array
*/
EXTERN_STRING void dstrings (double in, char* out);

/*
** \brief convert of a float complex into a char array
** \param in the float complex to convert
** \param out the output char array
*/
EXTERN_STRING void cstrings (floatComplex in, char* out);

/*
** \brief convert of a double complex into a char array
** \param in the double complex to convert
** \param out the output char array
*/
EXTERN_STRING void zstrings (doubleComplex in, char* out);


/*
** \brief convert of a float scalar array into an array of char arrays
** \param in the float scalar array to convert
** \param out the output array of char arrays
*/
EXTERN_STRING void sstringa (float* in, int size, char** out);

/*
** \brief convert of a double scalar array into an array of char arrays
** \param in the double scalar array to convert
** \param out the output array of char arrays
*/
EXTERN_STRING void dstringa (double* in, int size, char** out);

/*
** \brief convert of a float complex array into an array of char arrays
** \param in the float complex array to convert
** \param out the output array of char arrays
*/
EXTERN_STRING void cstringa (floatComplex* in, int size, char** out );

/*
** \brief convert of a double complex array into an array of char arrays
** \param in the double complex array to convert
** \param out the output array of char arrays
*/
EXTERN_STRING void zstringa (doubleComplex* in, int size, char** out);

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* !__STRING_H__ */

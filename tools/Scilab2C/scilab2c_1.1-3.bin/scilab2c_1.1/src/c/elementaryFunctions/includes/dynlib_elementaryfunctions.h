/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2009 - DIGITEO - Allan CORNET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __DYNLIB_ELEMENTARYFUNCTIONS_H__
#define __DYNLIB_ELEMENTARYFUNCTIONS_H__

#ifdef _MSC_VER
	#if ELEMENTARYFUNCTIONS_EXPORTS
		#define EXTERN_ELEMFUNCT __declspec (dllexport)
	#else
		#define EXTERN_ELEMFUNCT __declspec (dllimport)
	#endif
#else
	#define EXTERN_ELEMFUNCT 
#endif

#endif /* __DYNLIB_ELEMENTARYFUNCTIONS_H__ */

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

#ifndef __IS_EMPTY_H__
#define __IS_EMPTY_H__



#include "floatComplex.h"
#include "doubleComplex.h"
#include "notFound.h"
#include "find.h"


/*
** \brief Float Is Empty function
*/
#define	sisemptys(in)		0.0f /*= false*/

/*
** \brief Double Is Empty function
*/
#define	disemptys(in)		0/*= false*/

/*
** \brief Float Complex Is Empty function
*/
#define	cisemptys(in)		0.0f/*= false*/

/*
** \brief Double Complex Is Empty function
*/
#define	zisemptys(in)		0/*= false*/

/*
** \brief Float Is Empty function
*/
#define	sisemptya(in,size)		(size==0) ? 1.0f : 0.0f 

/*
** \brief Double Is Empty function
*/
#define	disemptya(in,size)		(size==0) ? 1.0 : 0.0

/*
** \brief Float Complex Is Empty function
*/
#define	cisemptya(in,size)		(size==0) ? 1.0f : 0.0f 

/*
** \brief Double Complex Is Empty function
*/
#define	zisemptya(in,size)		(size==0) ? 1.0 : 0.0 

#endif /* !__IS_EMPTY_H__ */

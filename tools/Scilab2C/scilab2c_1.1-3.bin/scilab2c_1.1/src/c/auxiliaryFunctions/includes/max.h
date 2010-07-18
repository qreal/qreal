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

#ifndef __MAX_H__
#define __MAX_H__

#define max(a,b) (a>=b?a:b)

#define maxa(a,size1,b,size2,out) 	{int i;\
						for (i=0;i<size1[0]*size2[1];i++) out[i]=max(a[i],b[i]);\
						}

#endif /* !__MAX_H__ */

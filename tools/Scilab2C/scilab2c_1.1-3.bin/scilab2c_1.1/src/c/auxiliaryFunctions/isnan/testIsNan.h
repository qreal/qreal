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

#ifndef __TESTISNAN_H__
#define __TESTISNAN_H__

#include <stdio.h>
#include <assert.h>
#include "isnan.h"

#define ERROR 51

int sisnansTest(void);

int disnansTest(void);

int cisnansTest(void);

int zisnansTest(void);

int sisnanaTest(void);

int disnanaTest(void);

int cisnanaTest(void);

int zisnanaTest(void);

int testIsNan(void);

#endif /* ! __TESTISNAN_H__ */

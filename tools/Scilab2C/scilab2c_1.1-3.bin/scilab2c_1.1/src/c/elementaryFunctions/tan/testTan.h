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

#ifndef _TESTTAN_H_
#define _TESTTAN_H_

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "tan.h"
#include "constant.h"

void stansTest(void);
void stanaTest(void);

void dtansTest(void);
void dtanaTest(void);

void ctansTest(void);
void ctanaTest(void);

void ztansTest(void);
void ztanaTest(void);

int testTan(void);

#endif /* !_TESTTAN_H_ */

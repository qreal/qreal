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

#ifndef _TESTTYPE_H_
#define _TESTTYPE_H_

#include <stdio.h>
#include <assert.h>

#include "floatComplex.h"
#include "doubleComplex.h"
#include "type.h"

int stypesTest(void);

int dtypesTest(void);

int ctypesTest(void);

int ztypesTest(void);

int stypeaTest(void);

int dtypeaTest(void);

int ctypeaTest(void);

int ztypeaTest(void);

int testType(void);

#endif /* ! _TESTTYPE_H_ */

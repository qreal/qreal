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

#ifndef _TESTPOW_H_
#define _TESTPOW_H_

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "pow.h"
#include "constant.h"


void spowsTest(void);

void dpowsTest(void);

void cpowsTest(void);

void zpowsTest(void);

void spowaTest(void);

void dpowaTest(void);

void cpowaTest(void);

void zpowaTest(void);

int testPow(void);

#endif /* ! _TESTPOW_H_ */

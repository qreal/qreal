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

#include <stdio.h>
#include "implicitList.h"

static void testsimplicitLists(void)
{
  int i = 0;
  float one_to_five[5] = {0};
  float five_to_one[5] = {0};
  float five_to_one_by_minus_ten[5] = {0};
  float result[1001] = {0};
  /*
  ** 1:1:5
  */
  simplicitLists(1.0f, 1.0f, 5.0f, one_to_five);
  for (i = 0 ; i < 5 ; ++i)
    {
      printf("one_to_five[%d] = %e\n", i, one_to_five[i]);
    }

  /*
  ** 5:1:1
  */
  simplicitLists(5.0f, 1.0f, 1.0f, five_to_one);
  for (i = 0 ; i < 5 ; ++i)
    {
      printf("five_to_one[%d] = %e\n", i, five_to_one[i]);
    }

  /*
  ** 5:-10:1
  */
  simplicitLists(5.0f, -10.0f, 1.0f, five_to_one_by_minus_ten);
  for (i = 0 ; i < 5 ; ++i)
    {
      printf("five_to_one_by_minus_ten[%d] = %e\n", i, five_to_one_by_minus_ten[i]);
    }

  /*
  ** 0:1e-03:1
  */
  simplicitLists(0.0f, 1e-03f, 1.0f, result);
  for (i = 0 ; i < 1001 ; ++i)
    {
      printf("result[%d] = %e\n", i, result[i]);
    }
}


int main(void)
{
  testsimplicitLists();
  return 0;
}

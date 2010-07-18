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
#include <assert.h>
#include "implicitList.h"

static void testdimplicitLists(void)
{
  int i = 0;
  double one_to_five[5] = {0};
  double five_to_one[5] = {0};
  double five_to_one_by_minus_ten[5] = {0};
  double result[1001] = {0};

  /*
  ** 1:1:5
  */
  dimplicitLists(1.0, 1.0, 5.0, one_to_five);
  for (i = 0 ; i < 5 ; ++i)
    {
      printf("one_to_five[%d] = %e\n", i, one_to_five[i]);
      assert(one_to_five[i] == (i + 1) * 1.0);
    }

  /*
  ** 5:1:1
  */
  dimplicitLists(5.0, 1.0, 1.0, five_to_one);
  for (i = 0 ; i < 5 ; ++i)
    {
      printf("five_to_one[%d] = %e\n", i, five_to_one[i]);
      assert(five_to_one[i] == 0.0);
    }

  /*
  ** 5:-10:1
  */
  dimplicitLists(5.0, -10.0, 1.0, five_to_one_by_minus_ten);
  for (i = 0 ; i < 5 ; ++i)
    {
      printf("five_to_one_by_minus_ten[%d] = %e\n", i, five_to_one_by_minus_ten[i]);
    }

  /*
  ** 5:-10:1
  */
  dimplicitLists(5.0, -10.0, 1.0, five_to_one_by_minus_ten);
  for (i = 0 ; i < 5 ; ++i)
    {
      printf("five_to_one_by_minus_ten[%d] = %e\n", i, five_to_one_by_minus_ten[i]);
    }

  /*
  ** 0:1e-03:1
  */
  dimplicitLists(0.0, 1e-04, 0.1, result);
  for (i = 0 ; i < 1001 ; ++i)
    {
      printf("result[%d] = %e\n", i, result[i]);
    }
}

static void testzimplicitLists(void)
{
  int i = 0;
  double one_to_five[5] = {0};
  double five_to_one[5] = {0};
  double five_to_one_by_minus_ten[5] = {0};
  double result[1001] = {0};

  /*
  ** 1:1:5
  */
  zimplicitLists(DoubleComplex(1.0, 2.0),
		 DoubleComplex(1.0, -3.0),
		 DoubleComplex(5.0, 10.0), one_to_five);
  for (i = 0 ; i < 5 ; ++i)
    {
      printf("one_to_five[%d] = %e\n", i, one_to_five[i]);
    }

  /*
  ** 5:1:1
  */
  zimplicitLists(DoubleComplex(5.0, 2.0),
		 DoubleComplex(1.0, -3.0),
		 DoubleComplex(1.0, 10.0), five_to_one);
  for (i = 0 ; i < 5 ; ++i)
    {
      printf("five_to_one[%d] = %e\n", i, five_to_one[i]);
    }

  /*
  ** 5:-10:1
  */
  zimplicitLists(DoubleComplex(5.0, 2.0),
		 DoubleComplex(-10.0, -3.0),
		 DoubleComplex(1.0, 10.0), five_to_one_by_minus_ten);
  for (i = 0 ; i < 5 ; ++i)
    {
      printf("five_to_one_by_minus_ten[%d] = %e\n", i, five_to_one_by_minus_ten[i]);
    }

  /*
  ** 5:-10:1
  */
  zimplicitLists(DoubleComplex(5.0, 2.0),
		 DoubleComplex(-10.0, -3.0),
		 DoubleComplex(1.0, 10.0), five_to_one_by_minus_ten);
  for (i = 0 ; i < 5 ; ++i)
    {
      printf("five_to_one_by_minus_ten[%d] = %e\n", i, five_to_one_by_minus_ten[i]);
    }

  /*
  ** 0:1e-03:1
  */
  zimplicitLists(DoubleComplex(0.0, 2.0),
		 DoubleComplex(1e-04, -3.0),
		 DoubleComplex(0.1, 10.0), result);
  for (i = 0 ; i < 1001 ; ++i)
    {
      printf("result[%d] = %e\n", i, result[i]);
    }
}

int main(void)
{
  testdimplicitLists();
  testzimplicitLists();
  return 0;
}

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

#include "lnp1m1.h"
#include "abs.h"

/*
  PURPOSE :  Compute   v = log ( (1 + s)/(1 - s) )
  for small s, this is for |s| < SLIM = 0.20

  ALGORITHM :
  1/ if |s| is "very small" we use a truncated
  taylor dvp (by keeping 3 terms) from :
  2        4          6
  t = 2 * s * ( 1 + 1/3 s  + 1/5 s  + [ 1/7 s  + ....] )
  2        4
  t = 2 * s * ( 1 + 1/3 s  + 1/5 s  + er)

  The limit E until we use this formula may be simply
  gotten so that the negliged part er is such that :
  2        4
  (#) er <= epsm * ( 1 + 1/3 s  + 1/5 s )   for all |s|<= E

  As  er  = 1/7 s^6 + 1/9 s^8 + ...
  er <= 1/7 * s^6 ( 1 + s^2 + s^4 + ...) = 1/7  s^6/(1-s^2)

  the inequality (#) is forced if :

  1/7  s^6 / (1-s^2)  <= epsm * ( 1 + 1/3 s^2  + 1/5 s^4 )

  s^6 <= 7 epsm * (1 - 2/3 s^2 - 3/15 s^4 - 1/5 s^6)

  So that E is very near (7 epsm)^(1/6) (approximately 3.032d-3):

  2/ For larger |s| we used a minimax polynome :

  yi = s * (2  + d3 s^3 + d5 s^5 .... + d13 s^13 + d15 s^15)

  This polynome was computed (by some remes algorithm) following
  (*) the sin(x) example (p 39) of the book :

  "ELEMENTARY FUNCTIONS"
  "Algorithms and implementation"
  J.M. Muller (Birkhauser)

  (*) without the additionnal raffinement to get the first coefs
  very near floating point numbers)
*/
float slnp1m1s(float Var)
{
  static float D3	= 0.66666666666672679472f;
  static float D5	= 0.39999999996176889299f;
  static float D7	= 0.28571429392829380980f;
  static float D9	= 0.22222138684562683797f;
  static float D11	= 0.18186349187499222459f;
  static float D13	= 0.15250315884469364710f;
  static float D15	= 0.15367270224757008114f;
  static float E	= 3.032E-3f;
  static float C3	= 2.0f/3.0f;
  static float C5	= 2.0f/5.0f;

  float S2 = Var * Var;
  if( sabss(Var) <= E)
    return Var * (2 + S2 * (C3 + C5 * S2));
  else
    return Var * (2 + S2 * (D3 + S2 * (D5 + S2 * (D7 + S2 * (D9 + S2 * (D11 + S2 * (D13 + S2 * D15)))))));
}

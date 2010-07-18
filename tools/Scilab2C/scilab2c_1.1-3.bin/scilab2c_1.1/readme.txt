##
##  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
##  Copyright (C) 2006-2008 - INRIA - Bruno JOFRET
##
##  This file must be used under the terms of the CeCILL.
##  This source file is licensed as described in the file COPYING, which
##  you should have received as part of this distribution.  The terms
##  are also available at
##  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
##
##

Feel free to add everything you find useful for hArtes and the scilab2c tool.

/*
** Type definition
*/
We define types that way (but it can evolve in the future) :
I - Scalar
-----------

 I.1 - Real
  I.1.1 - Simple precision (float)
  I.1.2 - Double precision (double)

 I.2 - Complex
  I.2.1 - Simple precision (float)
  I.2.2 - Double precision (double)

 I.3 - Integer (NOT IMPLEMENTED YET)

 I.4 - Boolean (NOT IMPLEMENTED YET)

II - Matrix
------------

!! WARNING !!
Matrix are stored column ways.
Ex :
double M[4] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0}
- If M has 2 lines and 3 columns :
M = [ 1.0 3.0 5.0 ]
    [ 2.0 4.0 6.0 ]
- If M has 3 lines and 2 columns :
M = [ 1.0 4.0 ]
    [ 2.0 5.0 ]
    [ 3.0 6.0 ]

 I.1 - Real
  I.1.1 - Simple precision (float)
  I.1.2 - Double precision (double)

 I.2 - Complex
  I.2.1 - Simple precision (float)
  I.2.2 - Double precision (double)

 I.3 - Integer (NOT IMPLEMENTED YET)

 I.4 - Boolean (NOT IMPLEMENTED YET)


/*
** Functions Naming style
*/
We define this coding style for functions names :
<precision><function_name><variable_type>.

variable_type :
- 's' : Scalar
- 'a' : Matrix

precision :
- 's' : Real simple precision (float)
- 'd' : Real double precision (double)
- 'c' : Complex simple precision (float)
- 'z' : Complex double precision (double)

/*
** Transtyping functions
** FIXME : Add it into the users limitations.
*/

- acos(x) : Real input.
	Return a Real value	if x E [ -1 ; 1 ].
	Return a Complex value	if x E ] -inf ; -1 [ U ] 1 ; +inf [.

- acosh(x) : Real input.
	Return a Real value	if x E ] 1 ; +inf [.
	Return a Complex value	if x E ] -inf ; 1 ].

- asin(x) : Real input.
	Return a Real value	if x E [ -1 ; 1 ].
	Return a Complex value	if x E ] -inf ; -1 [ U ] 1 ; +inf [.

- atanh(x) : Real input.
	Return a Real value	if x E ] -1 ; 1[.
	Return a Complex value	if x E ] -inf ; -1 [ U ] 1 ; +inf [.

- log(x) : Real input.
	Return a Real value	if x E ] 0 ; +inf [
	Return a Complex value	if x E ] -inf ; 0 [

- log10(x) : Real input.
	Return a Real value	if x E ] 0 ; +inf [
	Return a Complex value	if x E ] -inf ; 0 [

- spec(A) : Real input.
	Return a Real value	if A symmetric and for some other cases (to complete)
	Return a Complex value	otherelse

- spec2(A) : Real input.
	Return Reals value	if A symmetric and for some other cases (to complete)
	Return Complexes value	otherelse
	
- logm(A) : Real input.
	Return either a Real value or a Complex value : don't know the conditions to have one
	
- powm(A) : Real input.
	Return either a Real value or a Complex value : don't know the conditions to have one
	
- chol(A) : Complex input
 	The diagonal must be real;

-max(a,b) : Real input only

-min(a,b) : Real input only
 	
Following fuctions must have their 2 input matrix with the same dimension(row1=row2 and column1=column2)
-operation element by element : add, diff, rdiv, ldiv, mul
-comparaison operators : LogEq, LogNe, LogGt, LogGe, LogLt, LogLe
-pow



Following fuctions must have a square matrix on input
-logm
-powm
-determ
-chol
-trace
-lpc2cep (because using logm)
-spec
-spec2
-inv


-Careful about the functions ceil, floor, fix/int, round with float precision. Exemple : 
floor(1,999999)=2 instead of 1 in Scilab
floor(1.99999)=1  egal to Scilab result



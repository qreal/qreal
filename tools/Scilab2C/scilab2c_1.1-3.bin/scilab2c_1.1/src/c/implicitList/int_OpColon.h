/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* THIS IS AN AUTOMATICALLY GENERATED FILE : DO NOT EDIT BY HAND. */

#ifndef __INT_OPCOLON_H__
#define __INT_OPCOLON_H__

#define s0s0OpColons0(in1,in2)			in1

#define d0d0OpColond0(in1,in2)			in1

#define c0c0OpColons0(in1,in2)			in1

#define z0z0OpColond0(in1,in2)			in1



#define s0s0OpColons2(in1,in2,out)			simplicitLists(in1,1.0,in2,out)

#define d0d0OpColond2(in1,in2,out)			dimplicitLists(in1,1.0,in2,out)

#define c0c0OpColons2(in1,in2,out)			cimplicitLists(in1,FloatComplex(1.0f,0.0f),in2,out)

#define z0z0OpColond2(in1,in2,out)			zimplicitLists(in1,DoubleComplex(1.0,0.0),in2,out)

/*mixed input*/
#define s0c0OpColons0(in1,in2)			in1

#define d0z0OpColond0(in1,in2)			in1

#define c0s0OpColons0(in1,in2)			in1

#define z0d0OpColond0(in1,in2)			in1



#define s0c0OpColons2(in1,in2,out)			cimplicitLists(FloatComplex(in1,0.0f),FloatComplex(1.0f,0.0f),in2,out)

#define d0z0OpColond2(in1,in2,out)			zimplicitLists(DoubleComplex(in1,0.0),DoubleComplex(1.0,0.0),in2,out)

#define c0s0OpColons2(in1,in2,out)			cimplicitLists(in1,FloatComplex(1.0f,0.0f),FloatComplex(in2,0.0f),out)

#define z0d0OpColond2(in1,in2,out)			zimplicitLists(in1,DoubleComplex(1.0,0.0),DoubleComplex(in2,0.0),out)


/*three input*/

#define s0s0s0OpColons0(in1,in2,in3)		in1
#define s0s0c0OpColons0(in1,in2,in3)		in1
#define s0c0s0OpColons0(in1,in2,in3)		in1
#define s0c0c0OpColons0(in1,in2,in3)		in1

#define d0d0d0OpColond0(in1,in2,in3)		in1
#define d0d0z0OpColond0(in1,in2,in3)		in1
#define d0z0d0OpColond0(in1,in2,in3)		in1
#define d0z0z0OpColond0(in1,in2,in3)		in1

#define c0c0c0OpColons0(in1,in2,in3)		in1
#define c0c0s0OpColons0(in1,in2,in3)		in1
#define c0s0c0OpColons0(in1,in2,in3)		in1
#define c0s0s0OpColons0(in1,in2,in3)		in1

#define z0z0z0OpColond0(in1,in2,in3)		in1
#define z0z0d0OpColond0(in1,in2,in3)		in1
#define z0d0z0OpColond0(in1,in2,in3)		in1
#define z0d0d0OpColond0(in1,in2,in3)		in1


#define s0s0s0OpColons2(in1,in2,in3,out)		simplicitLists(in1,in2,in3,out)
#define s0s0c0OpColons2(in1,in2,in3,out)		cimplicitLists(FloatComplex(in1,0.0f),FloatComplex(in2,0.0f),in3,out)
#define s0c0s0OpColons2(in1,in2,in3,out)		cimplicitLists(FloatComplex(in1,0.0f),in2,FloatComplex(in3,0.0f),out)
#define s0c0c0OpColons2(in1,in2,in3,out)		cimplicitLists(FloatComplex(in1,0.0f),in2,in3,out)

#define d0d0d0OpColond2(in1,in2,in3,out)		dimplicitLists(in1,in2,in3,out)
#define d0d0z0OpColond2(in1,in2,in3,out)		zimplicitLists(DoubleComplex(in1,0.0),DoubleComplex(in2,0.0),in3,out)
#define d0z0d0OpColond2(in1,in2,in3,out)		zimplicitLists(DoubleComplex(in1,0.0),in2,DoubleComplex(in3,0.0),out)
#define d0z0z0OpColond2(in1,in2,in3,out)		zimplicitLists(DoubleComplex(in1,0.0),in2,in3,out)

#define c0c0c0OpColons2(in1,in2,in3,out)		cimplicitLists(in1,in2,in3,out)
#define c0c0s0OpColons2(in1,in2,in3,out)		cimplicitLists(in1,in2,FloatComplex(in3,0.0f),out)
#define c0s0c0OpColons2(in1,in2,in3,out)		cimplicitLists(in1,FloatComplex(in2,0.0f),in3,out)
#define c0s0s0OpColons2(in1,in2,in3,out)		cimplicitLists(in1,FloatComplex(in2,0.0f),FloatComplex(in3,0.0f),out)

#define z0z0z0OpColond2(in1,in2,in3,out)		zimplicitLists(in1,in2,in3,out)
#define z0z0d0OpColond2(in1,in2,in3,out)		zimplicitLists(in1,in2,DoubleComplex(in3,0.0),out)
#define z0d0z0OpColond2(in1,in2,in3,out)		zimplicitLists(in1,DoubleComplex(in2,0.0),in3,out)
#define z0d0d0OpColond2(in1,in2,in3,out)		zimplicitLists(in1,DoubleComplex(in2,0.0),DoubleComplex(in3,0.0),out)



#endif /* !__INT_OPCOLON_H__ */

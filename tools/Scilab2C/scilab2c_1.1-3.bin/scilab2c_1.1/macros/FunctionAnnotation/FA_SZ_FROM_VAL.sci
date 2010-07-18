function opout = FA_SZ_FROM_VAL(in1,in2)
// function  opout = FA_SZ_FROM_VAL(in1,in2)
// -----------------------------------------------------------------
// Return a size according to the floored value of the first argument
//
// Input data:
// in1: string specifying a number .//
//
// Output data:
// opout: string containing the computed result.
//
// -----------------------------------------------------------------

SCI2CNInArgCheck(argn(2),2,2);


if (SCI2Cisnum(in1))
   in1num = eval(in1) ;
   if isnan(in1num)
      opout = '__SCI2CNANSIZE';
   elseif ( in1num < 0 )
	opout= '0' ;
   else
	opout = string ( floor (abs(in1num))) ;
   
   end


else
  opout  = in2+"0floor"+in2+"0"+"("+in1+")";
  opout  = in2+"0abs"+in2+"0"+"("+opout+")";

end 


endfunction:

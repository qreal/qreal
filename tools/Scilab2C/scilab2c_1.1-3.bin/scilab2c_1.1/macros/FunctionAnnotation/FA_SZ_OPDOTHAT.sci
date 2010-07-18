function opoutsize = FA_SZ_OPDOTHAT(in1size,in2size)
// function opoutsize = FA_SZ_OPDOTHAT(in1size,in2size)
// -----------------------------------------------------------------
// Returns the size of the output computed by OPDOTHAT operator.
// 
// Assuming:
// size(in1) = [in1r,in1c]
// size(in2) = [in2r,in2c]
// size(out) = [outr,outc]
//
// we have the following combinations:
// in1   in2   outr   outc
// -----------------------
//  S     S    in2r   in2c    
//  S     M    in2r   in2c
//  M     S    in1r   in1c
//  M     M    in1r   in1c
//
// Where S means that the input is a scalar
// and M means that the input is a matrix.
//
// Input data:
// in1size: size of input number 1. It is an array of 2 strings. 
//          The first string specifies the number of rows.
//          The second string specifies the number of columns.
// 
// in2size: size of input number 2. It is an array of 2 strings. 
//          The first string specifies the number of rows.
//          The second string specifies the number of columns.
//
// Output data:
// opoutsize: size of output. It is an array of 2 strings. 
//            The first string specifies the number of rows.
//            The second string specifies the number of columns. 
//
// Status:
// 10-Dec-2007 -- Raffaele Nutricato: Author.
// 10-Dec-2007 -- Alberto Morea: Test Ok.
// -----------------------------------------------------------------

opoutsize = FA_SZ_OPDOTSTAR(in1size,in2size);

endfunction

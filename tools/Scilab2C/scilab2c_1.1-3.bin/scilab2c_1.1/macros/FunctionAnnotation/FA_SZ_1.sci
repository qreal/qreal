function outsize = FA_SZ_1(insize)
// function outsize = FA_SZ_1(insize)
// -----------------------------------------------------------------
// Returns the first element of the size array.
// 
// Input data:
// insize: size of input argument. It is an array of 2 strings. 
//          The first string specifies the number of rows.
//          The second string specifies the number of columns.
// 
// Output data:
// outsize: first element of the insize array.
//
// Status:
// 08-Dec-2007 -- Raffaele Nutricato: Author.
// 08-Dec-2007 -- Alberto Morea: Test Ok.
// -----------------------------------------------------------------

outsize = insize(1);
endfunction 

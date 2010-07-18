//SCI2C: DEFAULT_PRECISION= FLOAT
function test_float_log1p()
  d0 = %pi
  d2 = [1 2 3 4 ; 4 5 1  8 ; 9 1 11 12 ; 13 14 15 16]
  z0 = d0 + %i * d0
  z2 = d2 + %i * d2
  
  disp(log1p(d0));
  disp(log1p(d2));
  //disp(log1p(z0)); // ! Not Implemented in Scilab !
  //disp(log1p(z2)); // ! Not Implemented in Scilab !

endfunction
//SCI2C: DEFAULT_PRECISION= FLOAT
function test_float_sqrt()
  d0 = %pi
  d2 = [2 2 3 4 ; 4 5 6 8 ; 9 10 11 12 ; 13 14 15 16]
  z0 = d0 + %i * d0
  z2 = d2 + %i * d2
  
  disp(sqrt(d0));
  disp(sqrt(d2));
  disp(sqrt(z0));
  disp(sqrt(z2));
endfunction
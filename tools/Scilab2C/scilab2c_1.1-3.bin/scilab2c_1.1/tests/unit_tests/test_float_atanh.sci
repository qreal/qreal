//SCI2C: DEFAULT_PRECISION= FLOAT
function test_float_atanh()
  d0 = %pi
  d2 = [2 2 3 4 ; 4 5 6 8 ; 9 10 11 12 ; 13 14 15 16]
  z0 = d0 + %i * d0
  z2 = d2 + %i * d2
  
  disp(atanh(d0));
  disp(atanh(d2));
  disp(atanh(z0));
  disp(atanh(z2));
endfunction

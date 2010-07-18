//SCI2C: DEFAULT_PRECISION= FLOAT
function test_float_conj()
  d0 = %pi
  d2 = [1 2 3 4 ; 4 5 1  8 ; 9 1 11 12 ; 13 14 15 16]
  z0 = d0 + %i * d0
  z2 = d2 + %i * d2
  
 disp(conj(d0));
 disp(conj(d2));
 disp(conj(z0));
 disp(conj(z2));
endfunction

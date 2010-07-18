function test_double_sign()
  d0 = %pi
  d2 = [1 2 3 4 ; 4 5 6 8 ; 9 10 11 12 ; 13 14 15 16]
  z0 = d0 + %i * d0
  z2 = d2 + %i * d2
  
  sign(d0);
  sign(d2);
  sign(z0);
  sign(z2);
endfunction
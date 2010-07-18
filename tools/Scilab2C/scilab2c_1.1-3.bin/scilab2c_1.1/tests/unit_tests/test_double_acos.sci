function test_double_acos()
  d0 = %pi
  d2 = [1 2 3 4 ; 4 5 6 8 ; 9 10 11 12 ; 13 14 15 16]
  z0 = d0 + %i * d0
  z2 = d2 + %i * d2
  
  acos(d0);
  acos(d2);
  acos(z0);
  acos(z2);
endfunction

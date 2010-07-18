function test_double_asinh()
  d0 = %pi
  d2 = [1 2 3 4 ; 4 5 6 8 ; 9 10 11 12 ; 13 14 15 16]
  z0 = d0 + %i * d0
  z2 = d2 + %i * d2
  
  asinh(d0);
  asinh(d2);
  asinh(z0);
  asinh(z2);
endfunction

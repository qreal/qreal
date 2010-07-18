function test_double_acosh()
  d0 = %pi
  d2 = [1 2 3 4 ; 4 5 6 8 ; 9 10 11 12 ; 13 14 15 16]
  z0 = d0 + %i * d0
  z2 = d2 + %i * d2
  
  disp(acosh(d0));
  disp(acosh(d2));
  disp(acosh(z0));
  disp(acosh(z2));
endfunction

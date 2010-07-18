function test_double_chol()
  d0 = %pi
  d2 = [1 2 3 4 ; 4 5 6 8 ; 9 10 11 12 ; 13 14 15 16]
  z0 = d0 + %i * d0
  z2 = d2 + %i * d2
  
  disp(chol(d0));
  disp(chol(d2*d2'));
  disp(chol(z0));
  disp(chol(z2*z2')); 
endfunction

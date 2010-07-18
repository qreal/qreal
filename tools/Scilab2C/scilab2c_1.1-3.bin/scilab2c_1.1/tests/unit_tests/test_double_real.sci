function test_double_real()
  d0 = %pi
  d2 = [1 2 3 4 ; 5 6 7 8 ; 9 10 11 12 ; 13 14 15 16]
  z0 = d0 + %i * d0
  z2 = d2 + %i * d2
  
  disp(real(d0))
  disp(real(z0))
  disp(real(d2))
  disp(real(z2))
endfunction

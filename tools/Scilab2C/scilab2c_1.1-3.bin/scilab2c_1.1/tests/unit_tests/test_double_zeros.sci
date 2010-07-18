function test_double_zeros()
  d0 = %pi
  d2 = [1 2 3 4 ; 4 5 6 8 ; 9 10 11 12 ; 13 14 15 16]
  z0 = d0 + %i * d0
  z2 = d2 + %i * d2
  
  disp(zeros(d0))
  disp(zeros(z0))
  disp(zeros(d2))
  disp(zeros(z2))
  //disp(zeros(0,0)) // ! sci2c does not manage [] !
  disp(zeros(1,1))
  disp(zeros(d0,d0))
endfunction
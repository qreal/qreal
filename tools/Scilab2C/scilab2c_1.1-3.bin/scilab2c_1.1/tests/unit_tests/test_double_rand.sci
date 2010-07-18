function test_double_rand()
  d0 = %pi
  d2 = [1 2 3 4 ; 4 5 6 8 ; 9 10 11 12 ; 13 14 15 16]
  z0 = d0 + %i * d0
  z2 = d2 + %i * d2
  
  disp(rand(d0))
  disp(rand(z0))
  disp(rand(d2))
  disp(rand(z2))
  //disp(rand(0,0)) // ! sci2c does not manage [] !
  disp(rand(1,1))
  disp(rand(d0,d0))
endfunction
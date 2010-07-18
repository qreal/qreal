//SCI2C: DEFAULT_PRECISION= FLOAT
function test_float_zeros()
  d0 = %pi
  d2 = [1 2 3 4 ; 4 5 1  8 ; 9 1 11 12 ; 13 14 15 16]
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
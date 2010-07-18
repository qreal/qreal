//SCI2C: DEFAULT_PRECISION= FLOAT
function test_float_ones()
  d0 = %pi
  d2 = [1 2 3 4 ; 4 5 1  8 ; 9 1 11 12 ; 13 14 15 16]
  z0 = d0 + %i * d0
  z2 = d2 + %i * d2
  
  disp(ones(d0))
  disp(ones(z0))
  disp(ones(d2))
  disp(ones(z2))
  //disp(ones(0,0)) // ! sci2c does not manage [] !
  disp(ones(1,1))
  disp(ones(d0,d0))
endfunction
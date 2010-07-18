//SCI2C: DEFAULT_PRECISION= FLOAT
function test_float_imag()
  d0 = %pi
  d2 = [1 2 3 4 ; 5 6 7 8 ; 9 10 11 12 ; 13 14 15 16]
  z0 = d0 + %i * d0
  z2 = d2 + %i * d2
  
  disp(imag(d0))
  disp(imag(z0))
  disp(imag(d2))
  disp(imag(z2))
endfunction

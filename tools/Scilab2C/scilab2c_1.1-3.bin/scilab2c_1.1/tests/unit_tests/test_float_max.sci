//SCI2C: DEFAULT_PRECISION= FLOAT
function test_float_max()
  d0 = %pi
  d2 = [1 2 3 4 ; 5 6 7 8 ; 9 10 11 12 ; 13 14 15 16]
  d3 = [1 2 9 4 ; 5 6 7 8 ; 9 10 17 12 ; 14 14 15 16]
  
  disp(max(d0))
  disp(max(d2))
  
  disp(max(d0, 8))
  disp(max(d0, d2))
  disp(max(d2, 8))
  disp(max(d3, d2))

  

endfunction

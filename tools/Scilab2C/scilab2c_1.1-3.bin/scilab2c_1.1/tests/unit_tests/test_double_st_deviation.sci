function test_double_st_deviation()
  d0 = %pi
  d2 = [1 2 3 4 ; 4 5 6 8 ; 9 10 11 12 ; 13 14 15 16]
  z0 = d0 + %i * d0
  z2 = d2 + %i * d2
  
  disp(st_deviation(d0))
  disp(st_deviation(z0))
  disp(st_deviation(d2))
  disp(st_deviation(z2))

  disp(st_deviation(d0, 1))
  disp(st_deviation(z0, 1))
  disp(st_deviation(d2, 1))
  disp(st_deviation(z2, 1))

  disp(st_deviation(d0, 2))
  disp(st_deviation(z0, 2))
  disp(st_deviation(d2, 2))
  disp(st_deviation(z2, 2))
  
//   disp(st_deviation(d0, 'r'))
//   disp(st_deviation(z0, 'r'))
//   disp(st_deviation(d2, 'r'))
//   disp(st_deviation(z2, 'r'))

//   disp(st_deviation(d0, 'c'))
//   disp(st_deviation(z0, 'c'))
//   disp(st_deviation(d2, 'c'))
//   disp(st_deviation(z2, 'c'))
endfunction

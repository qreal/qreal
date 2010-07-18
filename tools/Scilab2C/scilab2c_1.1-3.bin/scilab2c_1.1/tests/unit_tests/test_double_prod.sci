function test_double_prod()
  d0 = %pi
  d2 = [1 2 3 4 ; 4 5 6 8 ; 9 10 11 12 ; 13 14 15 16]
  z0 = d0 + %i * d0
  z2 = d2 + %i * d2
  
  disp(prod(d0))
  disp(prod(z0))
  disp(prod(d2))
  disp(prod(z2))

  disp(prod(d0, 1))
  disp(prod(z0, 1))
  disp(prod(d2, 1))
  disp(prod(z2, 1))

  disp(prod(d0, 2))
  disp(prod(z0, 2))
  disp(prod(d2, 2))
  disp(prod(z2, 2))
  
//   disp(prod(d0, 'r'))
//   disp(prod(z0, 'r'))
//   disp(prod(d2, 'r'))
//   disp(prod(z2, 'r'))

//   disp(prod(d0, 'c'))
//   disp(prod(z0, 'c'))
//   disp(prod(d2, 'c'))
//   disp(prod(z2, 'c'))
endfunction
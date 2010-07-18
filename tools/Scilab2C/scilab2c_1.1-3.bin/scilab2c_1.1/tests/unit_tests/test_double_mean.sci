function test_double_mean()
  d0 = %pi
  d2 = [1 2 3 4 ; 4 5 6 8 ; 9 10 11 12 ; 13 14 15 16]
  z0 = d0 + %i * d0
  z2 = d2 + %i * d2
  
  disp(mean(d0))
  disp(mean(z0))
  disp(mean(d2))
  disp(mean(z2))

  disp(mean(d0, 1))
  disp(mean(z0, 1))
  disp(mean(d2, 1))
  disp(mean(z2, 1))

  disp(mean(d0, 2))
  disp(mean(z0, 2))
  disp(mean(d2, 2))
  disp(mean(z2, 2))
  
//   disp(mean(d0, 'r'))
//   disp(mean(z0, 'r'))
//   disp(mean(d2, 'r'))
//   disp(mean(z2, 'r'))

//   disp(mean(d0, 'c'))
//   disp(mean(z0, 'c'))
//   disp(mean(d2, 'c'))
//   disp(mean(z2, 'c'))
endfunction
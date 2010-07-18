function test_double_sum()
  d0 = %pi
  d2 = [1 2 3 4 ; 4 5 6 8 ; 9 10 11 12 ; 13 14 15 16]
  z0 = d0 + %i * d0
  z2 = d2 + %i * d2
  
  disp(sum(d0))
  disp(sum(z0))
  disp(sum(d2))
  disp(sum(z2))

  disp(sum(d0, 1))
  disp(sum(z0, 1))
  disp(sum(d2, 1))
  disp(sum(z2, 1))

  disp(sum(d0, 2))
  disp(sum(z0, 2))
  disp(sum(d2, 2))
  disp(sum(z2, 2))
  
//   disp(sum(d0, 'r'))
//   disp(sum(z0, 'r'))
//   disp(sum(d2, 'r'))
//   disp(sum(z2, 'r'))

//   disp(sum(d0, 'c'))
//   disp(sum(z0, 'c'))
//   disp(sum(d2, 'c'))
//   disp(sum(z2, 'c'))
endfunction
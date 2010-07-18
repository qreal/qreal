function test_double_min()
  d0 = %pi
  d2 = [1 2 3 4 ; 5 6 7 8 ; 9 10 11 12 ; 13 14 15 16]
  z0 = d0 + %i * d0
  z2 = d2 + %i * d2
  
  disp(min(d0))
  //disp(min(z0))
  disp(min(d2))
  //disp(min(z2))
  
  disp(min(d0, 1))
  //disp(min(z0, 1))
  disp(min(d2, 1))
  //disp(min(z2, 1))

  disp(min(d0, 2))
  //disp(min(z0, 2))
  disp(min(d2, 2))
  //disp(min(z2, 2))
  
//   disp(min(d0, 'r'))
//   disp(min(z0, 'r'))
//   disp(min(d2, 'r'))
//   disp(min(z2, 'r'))

//   disp(min(d0, 'c'))
//   disp(min(z0, 'c'))
//   disp(min(d2, 'c'))
//   disp(min(z2, 'c'))

endfunction

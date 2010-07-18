function test_double_max()
  d0 = %pi
  d2 = [1 2 3 4 ; 5 6 7 8 ; 9 10 11 12 ; 13 14 15 16]
  z0 = d0 + %i * d0
  z2 = d2 + %i * d2
  
  disp(max(d0))
  //disp(max(z0))
  disp(max(d2))
  //disp(max(z2))
  
  disp(max(d0, 1))
  //disp(max(z0, 1))
  disp(max(d2, 1))
  //disp(max(z2, 1))

  disp(max(d0, 2))
  //disp(max(z0, 2))
  disp(max(d2, 2))
  //disp(max(z2, 2))
  
//   disp(max(d0, 'r'))
//   disp(max(z0, 'r'))
//   disp(max(d2, 'r'))
//   disp(max(z2, 'r'))

//   disp(max(d0, 'c'))
//   disp(max(z0, 'c'))
//   disp(max(d2, 'c'))
//   disp(max(z2, 'c'))

endfunction

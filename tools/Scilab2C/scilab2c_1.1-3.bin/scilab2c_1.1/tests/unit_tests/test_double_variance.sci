function test_double_variance()
  d0 = 0.2113249
  d2 = [0.2113249 0.0002211 0.6653811;0.7560439 0.4453586 0.6283918]
  z0 = d0 + %i * d0
  z2 = d2 + %i * d2
  
  //disp(variance(d0))
  //disp(variance(z0))
  disp(variance(d2))
  disp(variance(z2))

  //disp(variance(d0, 1))
  //disp(variance(z0, 1))
  disp(variance(d2, 1))
  disp(variance(z2, 1))

  //disp(variance(d0, 2))
  //disp(variance(z0, 2))
  disp(variance(d2, 2))
  disp(variance(z2, 2))
  
//   disp(variance(d0, 'r'))
//   disp(variance(z0, 'r'))
//   disp(variance(d2, 'r'))
//   disp(variance(z2, 'r'))

//   disp(variance(d0, 'c'))
//   disp(variance(z0, 'c'))
//   disp(variance(d2, 'c'))
//   disp(variance(z2, 'c'))
endfunction

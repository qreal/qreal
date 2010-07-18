function test_double_OpMinus()
  a = 1;
  b = 2;
  c = a - b; // d0d0OpMinusd0
  disp(c);
  
  d = 1 - %i; // d0z0OpMinusz0
  e = %i - 3; // z0d0OpMinusz0
  f = d - e; // z0z0OpMinusz0

  disp(f);
  
  g = [1 2 3; 4 5 6];
  h = 10;
  w = g - h; // d2d0OpMinusd2
  j = h - g; // d0d2OpMinusd2
    
  disp(j);

  k = [9 8 7; 6 5 4; 3 2 1]
  l = [1 2 3; 4 5 6; 7 8 9]
  m = k - l // d2d2OpMinusd2
  
  disp(m);
  n = k - 2 * %i; //d2z0OpMinusz2
  o = 3 * %i - l; //z0d2OpMinusz2
  p = n - o; // z2z2OpMinusz2
  
  disp(p);
  q = [1 2 3 ; 4 5 6 ; 7 8 9]
  r = %pi * %i;
  s = q - r; // d2z0OpMinusz2
  t = r - q; // z0d2OpMinusz2
  
  disp(s);
  disp(t);
  u = [1 2 3] - %i * [4 5 6]
  v = [7 8 9]
  x = u - v; // z2d2OpMinusz2
  y = v - u; // d2z2OpMinusz2

  disp(x);

  disp(y);
endfunction

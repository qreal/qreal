//SCI2C: DEFAULT_PRECISION= FLOAT
function test_float_OpPlus()
  a = 1;
  b = 2;
  c = a + b; // d0d0OpPlusd0
  
  d = 1 + %i; // d0z0OpPlusz0
  e = %i + 3; // z0d0OpPlusz0
  f = d + e; // z0z0OpPlusz0
  
  g = [1 2 3; 4 5 6];
  h = 10;
  w = g + h; // d2d0OpPlusd2
  j = h + g; // d0d2OpPlusd2
  
  k = [9 8 7; 6 5 4; 3 2 1]
  l = [1 2 3; 4 5 6; 7 8 9]
  m = k + l // d2d2OpPlusd2
  
  n = k + 2 * %i; //d2z0OpPlusz2
  o = 3 * %i + l; //z0d2OpPlusz2
  p = n + o; // z2z2OpPlusz2
  
  q = [1 2 3 ; 4 5 6 ; 7 8 9]
  r = 3 * %i;
  s = q + r; // d2z0OpPlusz2
  t = r + q; // z0d2OpPlusz2
  
  u = [1 2 3] + %i * [4 5 6]
  v = [7 8 9]
  x = u + v; // z2d2OpPlusz2
  y = v + u; // d2z2OpPlusz2


  disp(c); 
  disp(d); 
  disp(e); 
  disp(f);
  disp("");
  disp(w); 
  disp(j); 
  disp(m); 
  disp(p); 
  disp(s); 
  disp(t); 
  disp(x); 
  disp(y); 

endfunction

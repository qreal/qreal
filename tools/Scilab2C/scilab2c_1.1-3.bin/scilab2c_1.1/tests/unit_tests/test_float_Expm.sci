//SCI2C: DEFAULT_PRECISION= FLOAT
function test_float_Expm()
 b = [1 2 3 4 ; 5 6 7 8 ; 9 10 11 12 ; 13 14 15 16];
 c = b*%i ;

 disp(expm(b));
 disp(expm(c));


endfunction

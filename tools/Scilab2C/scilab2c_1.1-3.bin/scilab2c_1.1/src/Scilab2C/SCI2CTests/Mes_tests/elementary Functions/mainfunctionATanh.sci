//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

//test atanh


disp (atanh(7/10));
disp (atanh((2+11*%i)/10))

c=[4 2 3 ;8 8 4; 3 4 5]/10
d=[4 2 3+3*%i ;8 8-1*%i 4; 3+4*%i 4 5]/10

disp (atanh(c))
disp (atanh(d))

endfunction

//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

//test sqrt

disp (sqrt(7/10));
disp (sqrt(2+11*%i))

c=[1 2 3 ;8 8 4; 3 4 5]
d=[1 2 3+3*%i ;8 8-1*%i 4; 3+4*%i 4 5]

disp (sqrt(c))
disp (sqrt(d))

endfunction

//SCI2C: DEFAULT_PRECISION= FLOAT

function mainfunction()

//test asin


disp (asin(.7));
disp (asin(.2+.11*%i))

c=[4 2 3 ;8 8 4; 3 4 5]/10
d=[4 2 3 ;8 8-1*%i 4; 3+4*%i 4 5]/10

disp (asin(c))
disp (asin(d))

endfunction

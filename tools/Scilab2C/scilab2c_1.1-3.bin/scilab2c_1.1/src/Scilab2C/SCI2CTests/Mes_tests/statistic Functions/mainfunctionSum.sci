//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

//test sum


c=[4 2 3 ;8 8 4; 3 4 5]
d=[4 2 3 ;8 8-1*%i 4; 3+4*%i 4 5]



disp (sum(c))
disp (sum(c,1))
disp (sum(c,2))
disp (sum(d))
disp (sum(d,1))
disp (sum(d,2))

endfunction

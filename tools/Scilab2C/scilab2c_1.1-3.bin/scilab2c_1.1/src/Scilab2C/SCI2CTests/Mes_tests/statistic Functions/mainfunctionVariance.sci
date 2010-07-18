//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

//test variance


c=[4 2 3 ;8 8 4; 3 4 5]
d=[4 2 3 ;8 8-1*%i 4; 3+4*%i 4 5]



disp (variance(c))
disp (variance(c,1))
disp (variance(c,2))
disp (variance(d))
disp (variance(d,1))
disp (variance(d,2))

endfunction

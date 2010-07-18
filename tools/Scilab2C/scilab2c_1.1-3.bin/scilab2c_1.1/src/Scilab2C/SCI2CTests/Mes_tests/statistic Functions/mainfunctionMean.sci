//SCI2C: DEFAULT_PRECISION= DOUBLE


function mainfunction()

//test mean


c=[4 2 3 ;8 8 4; 3 4 5]
d=[4 2 3 ;8 8-1*%i 4; 3+4*%i 4 5]



disp (mean(c))
disp (mean(c,1))
disp (mean(c,2))
disp (mean(d))
disp (mean(d,1))
disp (mean(d,2))

endfunction

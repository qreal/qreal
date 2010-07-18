//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

//test asinh


disp (asinh(7));
disp (asinh(2+11*%i))

c=[4 2 3 ;8 8 4; 3 4 5]
d=[4 2 3 ;8 8-1*%i 4; 3+4*%i 4 5]

disp (asinh(c))
disp (asinh(d))

endfunction

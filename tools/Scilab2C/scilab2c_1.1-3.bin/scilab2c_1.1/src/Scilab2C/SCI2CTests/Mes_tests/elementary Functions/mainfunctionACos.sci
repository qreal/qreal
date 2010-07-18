//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

//test acos


disp (acos(0.5));
disp (acos(0.2+0.1*%i))

c=[4 2 3 ;8 8 4; 3 4 5]/10
d=[4 2 3 ;8 8-1*%i 4; 3+4*%i 4 5]/10

disp (acos(c))
disp (acos(d))

endfunction

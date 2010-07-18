//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

//test acos


disp (acosh(7));
disp (acosh(2+11*%i))

c=[4 2 3 ;8 8 4; 3 4 5];
d=[4 2 3 ;8 8-1*%i 4; 3+4*%i 4 5];

disp (acosh(c))
disp (acosh(d))

endfunction

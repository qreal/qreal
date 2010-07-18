//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

//test inversion


disp (inv(3))


disp (inv(3+2*%i))

c=[4 2 3 ;8 8 4; 3 4 5]
d=[4 2 3 ;8 8-1*%i 4; 3+4*%i 4 5]


disp (inv(c))

disp (inv(d))

endfunction

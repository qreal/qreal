//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()


// test OpLogNot

disp(~(-2))
disp(~(0))
disp(~(3))

disp(~(0-2*%i))
disp(~(0+0*%i))
disp(~(0+3*%i))
disp(~(3-5*%i))
disp(~(3+0*%i))
disp(~(3+6*%i))

a=[-1 2 3; 0 -1 4; 2 -0 5];

disp(~(a))

b=[0+3*%i 2-%i 1 ; 0-2*%i 2+2*%i 1-%i; 0+0*%i 4+0*%i 5-0*%i]

disp(~(b))



endfunction

//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

disp(abs(7))
disp(abs(-3))


disp(abs(7-%i))
disp(abs(7+%i))
disp(abs(-3-2*%i))
disp(abs(-3+2*%i))

a=[-1 -5 4 -7; -8 3 -74 -5; 141 -3 4 45]
b=[1+%i*7 2 5+%i*4; 4+%i*21 8+%i 6; 3-%i*7 4-%i*2 78 ]
disp(abs(a))
disp(abs(b))


endfunction

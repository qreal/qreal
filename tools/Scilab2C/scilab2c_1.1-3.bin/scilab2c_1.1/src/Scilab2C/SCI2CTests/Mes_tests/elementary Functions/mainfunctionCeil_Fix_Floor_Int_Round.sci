//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()


// test ceil/fix/floor/int/round

disp(ceil(4.2))
disp(fix(4.2))
disp(floor(4.2))
disp(int(4.2))
disp(round(4.2))

disp(ceil(-4.2))
disp(fix(-4.2))
disp(floor(-4.2))
disp(int(-4.2))
disp(round(-4.2))


disp(ceil(4.2 - 2.5*%i))
disp(fix(4.2 - 2.5*%i))
disp(floor(4.2 - 2.5*%i))
disp(int(4.2 - 2.5*%i))
disp(round(4.2 - 2.5*%i))

disp(ceil(-4.2 + 2.5*%i))
disp(fix(-4.2 + 2.5*%i))
disp(floor(-4.2 + 2.5*%i))
disp(int(-4.2 + 2.5*%i))
disp(round(-4.2 + 2.5*%i))

a=[12.5 1.3 1.74 ; -2.4 -3.8 1.000001 ; 1.23 1.99999 0.035; 0 2 -4.5];

disp(ceil(a))
disp(fix(a))
disp(floor(a))
disp(int(a))
disp(round(a))

b=a+[1.2 3.8 -3.9; 7 2.7 1.5; 0 -9.5 2.0001; 2.35 -1. 5.49]*%i;
disp(ceil(b))
disp(fix(b))
disp(floor(b))
disp(int(b))
disp(round(b))

endfunction

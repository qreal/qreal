//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

//test OpApex(transpose&conj) & OpDotApex(transpose)

//reals

disp(3')

disp(3.')

a=[4 2 4 8;3 4 5 1;1 4 5 6; 7 4 5 3]

disp(a')
disp(a.')

//complexes

disp((3+%i)')

disp((3+%i).')

b=2.5*a+a*%i;

disp(b')
disp(b.')

endfunction

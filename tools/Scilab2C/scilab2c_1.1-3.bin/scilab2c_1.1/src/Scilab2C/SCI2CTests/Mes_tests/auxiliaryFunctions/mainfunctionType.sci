//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

//test type

disp (type(4))
disp (type(2+3*%i))

a=[1 2 4 ; 2 5 3]
disp (type(a))

b=[1-%i 2+3*%i 4 ; 2-2*%i 5 3+2*%i]
disp (type(b))

endfunction

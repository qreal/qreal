//SCI2C: DEFAULT_PRECISION= DOUBLE
function mainfunction()

//test addition

//d0d0
disp (1+1)
//d0z0
disp (1+%i)
//z0d0
disp (%i+1)
//z0z0
disp (%i+%i)

a=[1 2 3; 4 5 6]
b=[1+%i,2 3; 4 5 6]


//d2d2
disp(a+a)
//d2z2
disp(a+b)
//z2d2
disp(b+a)
//z2z2
disp(b+b)

//d2d0
disp(a+1)
//d0d2
disp(1+a)
//d2z0
disp(a+%i)
//z0d2
disp(%i+a)
//z2z2

//z2d0
disp(b+1)
//d0z2
disp(1+b)
//z2z0
disp(b+%i)
//z0z2
disp(%i+b)

endfunction

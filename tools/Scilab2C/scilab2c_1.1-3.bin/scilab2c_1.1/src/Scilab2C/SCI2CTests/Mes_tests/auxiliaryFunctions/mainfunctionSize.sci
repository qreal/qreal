//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

//test sign

//real
disp (size(4))
disp (size(4,1))
disp (size(4,2))

a=[1 0 5 4; -2 4 0 5 ; -4 -2 -1 4]
disp (size(a))
disp (size(a,1))
disp (size(a,2))

//complex
disp (size(4-2*%i))
disp (size(4-2*%i,1))
disp (size(4-2*%i,2))

b=[1+%i 0+0*%i 5-2*%i 4+0*%i; -2+%i 4 0+2*%i 5 ]
disp (size(b))
disp (size(b,1))
disp (size(b,2))

endfunction

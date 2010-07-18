//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

//test chol

//real
disp (chol(4))

a=[1 0 5; -2 4 0 ; -4 -2 -1]
a2=a*a'
disp (chol(a2))


//complex
disp (chol(4-2*%i))


b=[1+%i 0+0*%i; 5-2*%i 4+0*%i]
b2=b*b'
disp (chol(b2))


endfunction

//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

//test sign

//real
disp (sign(4))
disp (sign(0))
disp (sign(-2))

a=[1 0 5 4; -2 4 0 5 ; -4 -2 -1 4]
disp (sign(a))


//complex
disp (sign(4-2*%i))
disp (sign(4+2*%i))
disp (sign(0+%i*0))
disp (sign(3+%i*0))
disp (sign(0+%i*3))
disp (sign(-2 - 2*%i))
disp (sign(-2 + 2*%i))

b=[1+%i 0+0*%i 5-2*%i 4+0*%i; -2+%i 4 0+2*%i 5 ; -4-3*%i -2 -1-3*%i 4+2*%i]
disp (sign(b))

endfunction

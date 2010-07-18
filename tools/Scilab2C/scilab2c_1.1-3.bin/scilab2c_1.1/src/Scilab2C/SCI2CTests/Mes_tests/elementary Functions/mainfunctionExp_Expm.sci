//SCI2C: DEFAULT_PRECISION= DOUBLE
function mainfunction()

//test exp and expm

//d0
disp (exp(3))

//z0
disp (exp(3+%i*4))


a=[1 2 3; 5 5 6]
c=[4 2 3 ;8 8 4; 3 4 5]
b=[1+%i,2+4*%i 3+1*%i; 4 5 6+2*%i]
d=[4 2 3 ;8 8-1*%i 4; 3+4*%i 4 5]

//d2
disp (exp(a))
disp (exp(c))
disp (expm(c))

//z2
disp (exp(b))
disp (exp(d))
disp (expm(d))



endfunction

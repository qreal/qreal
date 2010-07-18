//SCI2C: DEFAULT_PRECISION= DOUBLE
function mainfunction()

//test det

//d0
disp (det(3))

//z0
disp (det(3+%i*4))


c=[4 2 3 ;8 8 4; 3 4 5]
e=[4 2 3 1;8 8 4 4; 3 4 5 7; 12 2 3 4]
d=[4 2 3 ;8 8-1*%i 4; 3+4*%i 4 5]
f=[4+%i 2 3 1*%i ;3+4*%i 8-2*%i 8+4*%i 4+2*%i ; 3 4+2*%i 5 7; 12+2*%i 2 3 4]

//d2
disp (det(c))
disp (det(e))

//z2
disp (det(d))
disp (det(f))



endfunction

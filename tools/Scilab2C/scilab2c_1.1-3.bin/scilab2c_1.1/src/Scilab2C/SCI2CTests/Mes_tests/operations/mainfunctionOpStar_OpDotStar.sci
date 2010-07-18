//SCI2C: DEFAULT_PRECISION= DOUBLE
function mainfunction()

//test multiplication

//d0d0
disp (5/3)
//d0z0
disp (1/%i)
//z0d0
disp (%i/5)
//z0z0
disp (%i/(3*%i))

a=[1 2 3; 4 5 6]
c=[4 2 3 ;8 8 4; 3 4 5]
b=[1+%i,2+4*%i 3+1*%i; 4 5 6+2*%i]
d=[4 2 3 ;8 8-1*%i 4; 3+4*%i 4 5]
e=[4 2 ;3 8 ;8-1*%i 4; 3+4*%i 4]

//d2d2
disp(a*c)
disp(a.*a)
//d2z2
disp(a*d)
disp(a.*b)
//z2d2
disp(e*a)
disp(b.*a)
//z2z2
disp(b*d)
disp(b.*b)




//d2d0
disp(a*1)
//d0d2
disp(1*a)
//d2z0
disp(a*%i)
//z0d2
disp(%i*a)


//z2d0
disp(b*1)
//d0z2
disp(1*b)
//z2z0
disp(b*%i)
//z0z2
disp(%i*b)

endfunction



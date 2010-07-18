//SCI2C: DEFAULT_PRECISION= FLOAT
function mainfunction()

//test division

//d0d0
disp (5/3)
//d0z0
disp (1/%i)
//z0d0
disp (%i/5)
//z0z0
disp (%i/(3*%i))

a=[1 2 3; 5 5 6]
c=[4 2 3 ;8 8 4; 3 4 5]
b=[1+%i,2+4*%i 3+1*%i; 4 5 6+2*%i]
d=[4 2 3 ;8 8-1*%i 4; 3+4*%i 4 5]

//d2d2
disp("d2d2")
disp(a/c)
disp(a./a)
//d2z2
disp("d2z2")
disp(a/d)
disp(a./b)
//z2d2
disp("z2d2")
disp(d/c)
disp(b./a)
//z2z2
disp("z2z2")
disp(b/d)
disp(b./b)




//d2d0
disp("d2d0")
disp(a/2)
//d0d2
disp("d0d2")
disp(1/c)
//d2z0
disp("d2z0")
disp(a/%i)
//z0d2
disp("z0d2")
disp(%i/c)


//z2d0
disp("z2d0")
disp(b/5)
//d0z2
disp("d0z2")
disp(1/d)
//z2z0
disp("z2z0")
disp(b/%i)
//z0z2
disp("z0z2")
disp(%i/d)

endfunction

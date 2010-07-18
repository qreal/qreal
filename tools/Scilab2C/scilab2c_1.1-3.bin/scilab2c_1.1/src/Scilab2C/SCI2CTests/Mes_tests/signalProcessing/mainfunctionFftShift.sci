//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()


// test fftshift

disp(fftshift(4))
disp(fftshift(5,1))
disp(fftshift(6,2))

disp(fftshift(4-%i))
disp(fftshift(4-%i*2,1))
disp(fftshift(4-%i*3,2))

a=[2 3 4 8; 8 4 5 6;1 2 3 4; 14 5 8 7; 1 2 6 7]
b=[2 3 4 8;1 2 6+3*%i 7]


disp(fftshift(a))
disp(fftshift(a,1))
disp(fftshift(a,2))


disp(fftshift(b))
disp(fftshift(b,1))
disp(fftshift(b,2))


endfunction

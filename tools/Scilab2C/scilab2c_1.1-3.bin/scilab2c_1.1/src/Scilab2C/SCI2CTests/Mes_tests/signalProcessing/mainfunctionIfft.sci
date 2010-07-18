//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

//test ifft

//real
disp (ifft(4))

a=[1 0 5; -2 4 0 ; -4 -2 -1]

disp (ifft(a))


//complex
disp (ifft(4-2*%i))


b=[1+%i 0+0*%i; 5-2*%i 4+0*%i]

disp (ifft(b))


endfunction

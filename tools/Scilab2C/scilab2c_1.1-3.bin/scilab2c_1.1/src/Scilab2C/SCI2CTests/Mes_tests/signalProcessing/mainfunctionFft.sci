//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()


// test fftshift

//1 input argument
disp(fft(4))

disp(fft([3 4 2 1; 1 4 5 2]));

disp(fft(4-%i))


disp(fft([3+%i*2 4 2-%i 1; 1-%i*2 4 5 2]));

//2 input arguments

disp(fft(4,1))

disp(fft([3 4 2 1; 1 4 5 2],1));

disp(fft(4-%i,1))


disp(fft([3+%i*2 4 2-%i 1; 1-%i*2 4 5 2],1));

disp(fft(4,-1))

disp(fft([3 4 2 1; 1 4 5 2],-1));

disp(fft(4-%i,-1))


disp(fft([3+%i*2 4 2-%i 1; 1-%i*2 4 5 2],-1));

endfunction

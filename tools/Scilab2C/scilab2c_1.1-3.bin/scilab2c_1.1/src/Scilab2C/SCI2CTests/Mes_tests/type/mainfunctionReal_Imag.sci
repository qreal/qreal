//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

disp(real(4));
disp(imag(4));

disp(real(1+2*%i));
disp(imag(1+2*%i));

a=[4 2 7 5;4 8 6 1];

disp(real(a));
disp(imag(a));

b=a+%i*2*a
disp(real(b));
disp(imag(b));

endfunction

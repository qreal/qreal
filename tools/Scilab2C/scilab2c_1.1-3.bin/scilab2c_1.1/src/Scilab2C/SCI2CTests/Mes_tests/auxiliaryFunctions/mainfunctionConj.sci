//SCI2C: DEFAULT_PRECISION= FLOAT

function mainfunction()

s1=[1+%i*3 1-%i*7;-1+5*%i 1-%i*7;-1+5*%i -1-6*%i]
s2=conj(s1);
disp(s2);

s3=[1 1;-1+5*%i 1-%i*7;-1+5*%i -1-6*%i]
s4=conj(s3);
disp(s4);

s5=1+%i*3
s6=conj(s5);
disp(s6);


s7=1-%i*3
s8=conj(s7);
disp(s8);

s9=[1+%i*3 1-%i*7;-1 7;-1+5*%i -1-6*%i]
s10=conj(s9);
disp(s10);

s11=[1+%i*3 1;-1+5*%i 1;-1 -1-6*%i]
s12=conj(s11);
disp(s12);


endfunction

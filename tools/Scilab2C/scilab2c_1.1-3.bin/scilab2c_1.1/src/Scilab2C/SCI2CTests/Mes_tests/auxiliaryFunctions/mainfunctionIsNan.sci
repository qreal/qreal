//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

//test isnan


disp(isnan(4));
disp(isnan(3+%i*4));

c=[1 5 4 8; 2 9 4 5; 3 6 4 7]
d=[1 5+%i*4 4 8+%i; 2-%i*3 9+%i*7 4+%i*2 5; 3 6 4 7-%i]
e=[0,0]
//disp(isnan([1 0.01 -%nan %inf-%inf]));
disp(isnan([1 0.01  %inf-%inf]));
disp(isnan(c));
disp(isnan(d));
disp(isnan(e));
disp(isnan(e+%i*0));
endfunction

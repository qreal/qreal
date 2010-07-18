//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

//test Hat_DotHat

disp(4.1^3.5);
disp(4.1.^3.5);

disp(4.4^(3.2+%i*1.1));
disp(4.4.^(3.2+%i*1.1));

disp((3.2+%i*1.1)^4.4);
disp((3.2+%i*1.1).^4.4);

disp((3.2+%i*1.1)^(4.4+2.6*%i));
disp((3.2+%i*1.1).^(4.4+2.6*%i));

a=[1.2 4.1 3.9; 3.7 2.6 8.1];
b=a+[2.5 3.9 8.7; 2.5 0.2 3.3]*%i;

disp(1.2^a)
disp(1.2.^a)

disp(1.2^b)
disp(1.2.^b)

disp((1.2+%i*1.1)^a)
disp((1.2+%i*1.1).^a)

disp((1.2+%i*1.1)^b)
disp((1.2+%i*1.1).^b)

disp(a.^a)
disp(a.^b)

disp(b.^a)
disp(b.^b)

c=[1.2 1.3; 1.4 5.1];

disp(c^2.1);
disp(c^(2.1+%i*4.6));

d=c+[4.1 2.1; 4.6 5.1]*%i;

disp(d^2.1);
disp(d^(2.1+%i*4.6));

endfunction

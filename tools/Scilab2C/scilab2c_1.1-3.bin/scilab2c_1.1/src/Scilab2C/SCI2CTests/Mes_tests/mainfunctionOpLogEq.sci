//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()


// test OpEqual
disp(1==1)
disp(1==2)
disp(1==2+%i)
disp(1+%i==2)
disp(1+%i==2+%i)

a=[1 2 ; 3 4]
b=[5 2 ; 6+%i 1-%i]

disp(a==a)
disp(a==b)
disp(b==a)
disp(b==b)


disp(3==a)
disp(5==b)
disp(a==4)
disp(b==2)

disp(1+%i==a)
disp(1-%i==b)
disp(a==6+%i)
disp(b==6+%i)

endfunction

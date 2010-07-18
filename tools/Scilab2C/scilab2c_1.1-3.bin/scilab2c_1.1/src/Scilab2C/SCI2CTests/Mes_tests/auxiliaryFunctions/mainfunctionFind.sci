//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

a = 1
disp(find(a==0))
disp(find(a==1))

b = 1+3*%i
disp(find(b==2-%i))
disp(find(b==1+3*%i))

c=[2 5 4 7; 36 4 1 5; 1 2 54 7; 9 6 3 1]
disp(find(c==1))


d=[1 2+%i 5 9; 7+%i 5+%i 4+%i 1+%i; 1+%i 5+%i 6+%i 3+%i; 2+%i 2+%i 2+%i 2+%i*3]
disp(find(d==2+%i))

endfunction

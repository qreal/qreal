//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

//test eye

//reals


disp(eye())

disp(eye(1))

disp(eye(4))

disp(eye(1,1))

disp(eye(1,4))

disp(eye(4,4))

disp(eye(4,1))

a=[1 2 3; 4 5 6; 7 8 9]

disp(eye(a))

//complexe

disp(eye(2+%i))

b=a+%i

disp(eye(b))


endfunction

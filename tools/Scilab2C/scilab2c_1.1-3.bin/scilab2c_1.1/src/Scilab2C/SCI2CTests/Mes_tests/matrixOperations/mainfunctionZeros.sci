//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

//test zeros

//reals


//disp(zeros())

disp(zeros(1))

disp(zeros(4))

disp(zeros(1,1))

disp(zeros(1,4))

disp(zeros(4,4))

disp(zeros(4,1))

a=[1 2 3; 4 5 6; 7 8 9]

disp(zeros(a))

//complexe

disp(zeros(2+%i))

b=a+%i

disp(zeros(b))


endfunction

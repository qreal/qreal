//SCI2C: DEFAULT_PRECISION= DOUBLE
function mainfunction()

N = 2;
B = myinit(N+4-2,2);
disp('B')
disp(B)

N = 4;
C = myinit(N,3);
disp('C')
disp(C)

D=B+2.*C;
disp('D')
disp(D)
endfunction


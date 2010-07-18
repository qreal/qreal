//SCi0C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

s1 = [1 -3.56 4 -50.20 -7 -8 9 -3 4];
disp('s1');
disp(s1);

i0 = find(s1<0.5);
disp('i0');
disp(i0);

disp('s1(i0)');
disp(s1(i0));

endfunction


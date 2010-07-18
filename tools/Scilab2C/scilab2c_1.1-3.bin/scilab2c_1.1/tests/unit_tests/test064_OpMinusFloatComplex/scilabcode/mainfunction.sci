//SCI2C: DEFAULT_PRECISION= FLOAT

function mainfunction()

s1 = -5.4;
disp('s1');
disp(s1);

s2 = -2.2-4*%i;
disp('s2');
disp(s2);

A = ((1.2:0.99:4.9)')*(-2.3:1.1:3.9)
disp('A = ((1.2:0.99:4.9)'')*(-2.3:1.1:3.9)');
disp(A);

B = -2.11*A-%i*A;
disp('B');

disp('o1 = -s1 - s2;');
o1 = -s1 - s2;
disp(o1);

disp('o1 = -3.4 - 4.5;');
o1 = (-3.4-%i) - (4.5-%i);
disp(o1);

disp('o2 = -3.4 - B - A;');
o2 = -3.4 - B - A;
disp(o2);

disp('o3 = A - (-3.4);');
o3 = A - (-3.4);
disp(o3);

disp('o4 = A - A;');
o4 = A - B;
disp(o4);

disp('o5 = -A;');
o5 = -B;
disp(o5);
endfunction

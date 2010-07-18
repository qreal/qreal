//SCI2C: DEFAULT_PRECISION= FLOAT

function mainfunction()

s1 = -5.4;
disp('s1');
disp(s1);

s2 = 2.2;
disp('s2');
disp(s2);

A = ((1.2:0.99:4.9)')*(-2.3:1.1:3.9)
disp('A = ((1.2:0.99:4.9)'')*(-2.3:1.1:3.9)');
disp(A);

disp('o1 = -s1 + s2;');
o1 = -s1 + s2;
disp(o1);

disp('o1 = -3.4 + 4.5;');
o1 = -3.4 + 4.5;
disp(o1);

disp('o2 = -3.4 + A;');
o2 = -3.4 + A;
disp(o2);

disp('o3 = A + (-3.4);');
o3 = A + (-3.4);
disp(o3);

disp('o4 = A + A;');
o4 = A + A;
disp(o4);

disp('o5 = +A;');
o5 = +A;
disp(o5);

st0 = '#';
disp('st0')
disp(st0)

st1 = 'ciao';
disp('st1')
disp(st1)

st2 = 'bye';
disp('st2')
disp(st2)

st3 = st0+st1+st2;
disp('st3 = st0+st1+st2;')
disp(st3)
endfunction


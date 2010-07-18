//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

a = 1
[u1,v1]=find(a==0);
[u2,v2]=find(a==1);

disp(u1);disp(v1);
disp(u2);disp(v2);

b = 1+3*%i
[u3,v3]=find(b==0);
[u4,v4]=find(b==1+3*%i);

disp(u3);disp(v3);
disp(u4);disp(v4);

c=[2 5 4 7; 36 4 1 5; 1 2 54 7; 9 6 3 1]
[u5,v5]=find(c==0);
[u6,v6]=find(c==1);

disp(u5);disp(v5);
disp(u6);disp(v6);

d=[1 2+%i 5 9; 7+%i 5+%i 4+%i 1+%i; 1+%i 5+%i 6+%i 3+%i; 2+%i 2+%i 2-%i 2+%i*3]
[u7,v7]=find(d==0);
[u8,v8]=find(d==2+%i);

disp(u7);disp(v7);
disp(u8);disp(v8);
endfunction

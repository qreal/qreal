//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()


// test OpIns

//reals
a=[1 2 3; 4 5 6]

a(3)=-2
disp(a);

a(3)=-4+%i
disp(a);

a(1,3)=-12;
disp(a);

a(1,3)=-5+%i;
disp(a);

a([1,2],1)=-12;
disp(a);

a([1,2],1)=-5+%i;
disp(a);

a(2,[2,3])=-12;
disp(a);

a(2,[2,3])=-5+%i;
disp(a);

a([1 2],[1,2,3])=4;
disp(a);

a([1 2],[1,2,3])=3+%i;
disp(a);

a([1,3])=7;
disp(a);

a([1,3])=2+%i;
disp(a);

a([1,3])=[1,0];
disp(a);

a([1,3])=[3-%i,1+%i];
disp(a);

a(2,[1,3])=[-2,-7];
disp(a);

a(2,[1,3])=[-4+%i,-5+%i];
disp(a);

a([1,2],3)=[-9;-1];
disp(a);

a([1,2],3)=[-4+%i;-5+%i];
disp(a);

a([1,2],[2,3])=[-31 -45;-29 -51];
disp(a);

a([1,2],[2,3])=[-4+%i,-5+%i;-4+%i,-5+%i];
disp(a);


//complexes

b=[1-3*%i 2+%i 3+2*%i; 4 5+4*%i 6]

b(3)=-2
disp(b);

b(3)=-4+%i
disp(b);

b(1,3)=-12;
disp(b);

b(1,3)=-5+%i;
disp(b);

b([1,2],1)=-12;
disp(b);

b([1,2],1)=-5+%i;
disp(b);

b(2,[2,3])=-12;
disp(b);

b(2,[2,3])=-5+%i;
disp(b);

b([1 2],[1,2,3])=4;
disp(b);

b([1 2],[1,2,3])=3+%i;
disp(b);

b([1,3])=7;
disp(b);

b([1,3])=2+%i;
disp(b);

b([1,3])=[1,0];
disp(b);

b([1,3])=[3-%i,1+%i];
disp(b);

b(2,[1,3])=[-2,-7];
disp(b);

b(2,[1,3])=[-4+%i,-5+%i];
disp(b);

b([1,2],3)=[-9;-1];
disp(b);

b([1,2],3)=[-4+%i;-5+%i];
disp(b);

b([1,2],[2,3])=[-31 -45;-29 -51];
disp(b);

b([1,2],[2,3])=[-4+%i,-5+%i;-4+%i,-5+%i];
disp(b);

endfunction

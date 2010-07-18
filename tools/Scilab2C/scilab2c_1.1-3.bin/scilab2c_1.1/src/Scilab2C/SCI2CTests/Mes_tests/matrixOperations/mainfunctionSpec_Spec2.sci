//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

//test spec

//reals

disp(spec(4))

[u,v]=spec(4)
disp(u)
disp(v)

a=[4 2 4 8;3 4 5 1;1 4 5 6; 7 4 5 3]

disp(spec(a))

[x,y]=spec(a)
disp(x)
disp(y)

b=[1 1;1 3]
disp(spec(b))

[s,t]=spec(b)
disp(s)
disp(t)

c=[1 -2;1 3]
disp(spec(c))

[m,n]=spec(c)
disp(m)
disp(n)


//complexes

disp(spec(3-%i))

[i,j]=spec(3-%i)
disp(i)
disp(j)

d=[1 4 2 5; 2 5 4 7; 3 4 2 4;1 4 7 5]+[1 4 5 7;2 3 6 9;2 4 1 5; 7 8 5 4]*%i;

disp(spec(d))

[k,l]=spec(d)
disp(k)
disp(l)

e=[1 2; 2 7]+[0 4;-4 0]*%i
disp(spec(e))

[o,p]=spec(e)
disp(o)
disp(p)

disp(spec(a+0*%i))
[q,qq]=spec(a+0*%i)
disp(q)
disp(qq)

endfunction

//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

//test lev

//reals

b=[1 2 3 4]

disp(lev(b))

[u,v]=lev(b)
disp(u)
disp(v)

[x,y,z]=lev(b)
disp(x)
disp(y)
disp(z)

//complexes

c=[1+2*%i 2 3-3*%i 4+%i]

disp(lev(c))

[uc,vc]=lev(c)
disp(uc)
disp(vc)

[xc,yc,zc]=lev(c)
disp(xc)
disp(yc)
disp(zc)


endfunction

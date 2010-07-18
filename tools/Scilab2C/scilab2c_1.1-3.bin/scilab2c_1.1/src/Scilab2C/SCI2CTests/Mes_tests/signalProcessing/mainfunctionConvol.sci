//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

//test convol

disp (convol(3,5))
disp (convol(3+2*%i,5-%i))

disp (convol(3,5+3*%i))
disp (convol(3+2*%i,2))

c=[1 2 3]
d=[4 5 6 7 8]
e=[2+3*%i 4+4*%i 7-5*%i 8+%i]
f=[1+5*%i 3-2*%i 1-%i]

disp (convol(3,d))
disp (convol(3,f))
disp (convol(c,2))
disp (convol(e,5))

disp (convol(3-%i,d))
disp (convol(3+2*%i,f))
disp (convol(c,2+2*%i))
disp (convol(e,5+%i))

disp (convol(c,d))
disp (convol(c,f))
disp (convol(e,c))
disp (convol(e,f))

//the following lines don't exist in Scilab but are in Sci2C

//g=[1 2; 3 4]
//h=[4 5; 6 7]
//i=[2+3*%i 4+4*%i; 7-5*%i 8+%i]
//j=[1+5*%i 4+4*%i; 3-2*%i 1-%i]

//disp (convol(g,h))
//disp (convol(g,j))
//disp (convol(i,h))
//disp (convol(i,j))



endfunction

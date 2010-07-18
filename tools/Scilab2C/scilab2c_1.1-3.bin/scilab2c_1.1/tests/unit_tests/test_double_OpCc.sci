

function test_double_OpCc()

	a = [1 ; 2 ; 3 ; 4]; // d0d0OpCcs2
	b = [7 ; a];// d0d2OpCcs2
	c = [a ; 7];// d2d0OpCcs2
	d = [a ; b];// d2d2OpCcs2
	disp(a);	
	disp(b);	
	disp(c);	
	disp(d);	

	e = [1*%i ; 2*%i ; 3*%i ; 4*%i ];// z0z0OpCcz2
	f = [7 ; e];// z0z2OpCcz2
	g = [e ; 7];// z2z0OpCcz2
	h = [e ; f];// z2z2OpCcz2

	disp(e);	
	disp(f);	
	disp(g);	
	disp(h);	

	
	i = [%i  ; 4.5]; // z0d0OpCcz2
	j = [4.5 ; %i ]; // d0z0OpCcz2
	k = [4.5 ; i  ]; // d0z2OpCcz2
	l = [i   ; 4.5]; // z2d0OpCcz2

	disp(i);	
	disp(j);	
	disp(k);	
	disp(l);	

	m = [1   ; 4.5]; 
	n = [%i  ; m  ]; // z0d2OpCcz2
	o = [m   ; %i ]; // d0z2OpCcz2
	p = [m   ; n  ]; // d2z2OpCcz2
	q = [n   ; m  ]; // z2d2OpCcz2

	disp(n);	
	disp(o);	
	disp(p);	
	disp(q);	

endfunction

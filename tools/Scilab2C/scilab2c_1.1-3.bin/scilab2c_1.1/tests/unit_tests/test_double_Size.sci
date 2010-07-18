function test_double_Size()
	disp(size(4));
	disp(size([1 2]))
	disp(size(2+3*%i));
	disp(size([1 2 3 ; 4 5 6]));
	disp(size([1 2 3 ; 4 5 6]*%i));
	e = %pi ;
	disp(size(e)); // d0Sized0

	f = e * %i;
	disp(size(f));// z0Sized0

	a = [1 2 3 ; 4 5 6];
	disp(size(a));// d2Sized0
	
	b = [1 2 3 ; 4 5 6] * %i;
	disp(size(b));// z2Sized0

// with a second argument
	o=1;
	
	disp(size(4,1));
	disp(size(2+3*%i,1));
	disp(size([1 2 3 ; 4 5 6],o));
	disp(size([1 2 3 ; 4 5 6]*%i,1));
	e = %pi ;
	disp(size(e,1)); // d0Sized0

	f = e * %i;
	disp(size(f,1));// z0Sized0

	a = [1 2 3 ; 4 5 6];
	disp(size(a,o));// d2Sized0
	
	b = [1 2 3 ; 4 5 6] * %i;
	disp(size(b,1));// z2Sized0
	



endfunction 

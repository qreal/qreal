//SCI2C: DEFAULT_PRECISION= FLOAT
function test_float_Length()
	disp(length(4));
	disp(length(2+3*%i));
	disp(length([1 2 3 ; 4 5 6]));
	disp(length([1 2 3 ; 4 5 6]*%i));
	e = %pi ;
	disp(length(e)); // d0Lengthd0

	f = e * %i;
	disp(length(f));// z0Lengthd0

	a = [1 2 3 ; 4 5 6];
	disp(length(a));// d2Lengthd0
	
	b = [1 2 3 ; 4 5 6] * %i;
	disp(length(b));// z2Lengthd0


	g = "pouet" ;
	h = "a" ;
	
	disp(length(g)); // g0Lengthd0
	disp(length(h)); // g2Lengthd0

	disp(length("pouet"));
	disp(length("a"));

	



endfunction 

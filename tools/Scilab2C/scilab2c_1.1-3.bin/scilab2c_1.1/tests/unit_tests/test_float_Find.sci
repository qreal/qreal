//SCI2C: DEFAULT_PRECISION= FLOAT
function test_float_Find ()

	disp (find( 1 == 3 )); // d0Findd0
	
	a = [1 2 3; 4 5 6];
	b = [1 8 3; 9 5 6];
	disp (find( a == b )); // d2Findd2

	[ z , j ] = find (1 == 3); // d0Findd0d0
	[ k , l ] = find (a == b); // d2Findd2d2
	
	disp(z);
	disp(j);
	disp(k);
	disp(l);

	disp ( find( 1 == 3 , 2 )); // d0d0Findd0
	
	c = [1 2 3; 4 5 6];
	d = [1 8 3; 9 5 6];
	disp (find( c == d , 2 )); // d2d0Findd2

	[ o , p ] = find (1 == 3 , 2); // d0d0Findd0d0
	[ q , s ] = find (c == d , 2); // d2d0Findd2d2

	disp(o);
	disp(p);
	disp(q);
	disp(s);

endfunction

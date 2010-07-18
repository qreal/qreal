//SCI2C: DEFAULT_PRECISION= FLOAT
function test_float_Det()

	a = [1 2 3; 4 %pi 5; 4.5 -3 2];
	disp(det(a));

	b = %i * [1 2 3; 4 %pi 5; 4.5 -3 2];
	disp(det(b));


	disp(det(5));

	disp(det(%i));
endfunction



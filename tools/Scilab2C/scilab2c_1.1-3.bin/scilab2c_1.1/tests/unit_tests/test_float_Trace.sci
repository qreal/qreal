//SCI2C: DEFAULT_PRECISION= FLOAT
function test_float_Trace()

	a = [1 2 3; 4 %pi 5; 4.5 -3 2];
	disp(trace(a));

	b = %i * [1 2 3; 4 %pi 5; 4.5 -3 2];
	disp(trace(b));


	disp(trace(5));

	disp(trace(%i));
endfunction

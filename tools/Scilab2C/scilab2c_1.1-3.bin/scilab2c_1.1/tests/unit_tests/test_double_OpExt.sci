function test_double_OpExt ()

	a = [1 2 3 ; 4 5 6];


	disp(a(2));
	disp(a([3 5]));
	disp(a(2,3));
	disp(a(2,[2 3]));
	disp(a ([1 2],1));
	disp(a ([1 2],[2 3]));


	b = [1 2 3 ; 4 5 6] * %i;


	disp(b(2));
	disp(b([3 5]));
	disp(b(2,3));
	disp(b(2,[2 3]));
	disp(b ([1 2],1));
	disp(b ([1 2],[2 3]));


endfunction

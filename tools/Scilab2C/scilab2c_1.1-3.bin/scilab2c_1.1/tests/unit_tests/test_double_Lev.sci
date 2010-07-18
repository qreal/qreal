function test_double_Lev()

	a = [1 2 3 4 5 6 7 8 9 10] ; 

	disp(lev(a)) ;

	[aa aaa] = lev(a);
	disp(aa);
	disp(aaa)

	[a1 aa1 aaa1] = lev (a);
	disp(a1);
	disp(aa1);
	disp(aaa1);
	
// complex
	b =%i * [1 2 3 4 5 6 7 8 9 10] ; 

	disp(lev(b)) ;

	[bb bbb] = lev(b);
	disp(bb);
	disp(bbb)

	[b1 bb1 bbb1] = lev (b);
	disp(b1);
	disp(bb1);
	disp(bbb1);


endfunction

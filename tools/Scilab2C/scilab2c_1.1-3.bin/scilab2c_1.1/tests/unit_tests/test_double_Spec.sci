function test_double_Spec()


	disp(spec(45)) ; // d0Spec
	disp(spec(4+3*%i)); // z0Spec
	
	a = [1 2 3; -4 5 98 ; 7 8 5] ;
	b = [1 2 3; -4 5 98 ; 7 -100000 5] ;


	disp(spec(a));
	disp(spec(b));

	disp(spec(a*%i));
 // 2 outputs

	[aa bb] = spec(45) ; // d0Spec
	disp(aa);
	disp(bb);
	
 	[cc dd] = spec(4+3*%i); // z0Spec
	
	disp(cc);
	disp(dd);

	a = [1 2 3; -4 5 98 ; 7 8 5] ;
	b = [1 2 3; -4 5 98 ; 7 -100000 5] ;


	[ee ff] = spec(a);
	disp(ee);
	disp(ff);
	[gg hh] = spec(b);
	disp(gg);
	disp(hh);

	[ii jj] = spec(a*%i);
	disp(ii);
	disp(jj);

endfunction

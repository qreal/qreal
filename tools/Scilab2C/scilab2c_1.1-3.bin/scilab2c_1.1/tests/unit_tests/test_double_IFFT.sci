function test_double_IFFT()


disp(ifft(2)); // d0IFFTd0
disp(ifft(%i + 4)) // z0IFFTz0
	
disp(ifft([1 2])); // d2IFFTd2
disp(ifft([1 -2 3] + zeros(1 , 3)*%i )); // d2IFFTz2
disp(ifft([1 2]*%i)); // z2IFFTz2

disp(ifft(4.4))
disp(ifft(%i + 3.4))


// one input
	a = [10 1 15 3.5 ]; //d2Fftz2
	Abis = a + zeros(a)*%i ;
	disp(ifft(Abis));
	disp("********1*******");
	b = [25  3.6]; // d2Fftd2
	disp(ifft(b));
	disp("********2*******");
	c= [10 2 5 6 3 8 9 445 1 2 3 0 125 5 41 4 5 552 2.36  45656 72.3 ]
	cbis = c + zeros(c)*%i ;
	disp(ifft(cbis));
	disp("********3*********");
	// pure imaginary
	d = [10 1 15 3.5 ]*%i; //z2Fftz2
	disp(ifft(d));
	disp("********4*********");
	e = [25  3.6]*%i; // z2Fftd2
	disp(ifft(e));
	disp("********5***********");
	f= %i*[10 2 5 6 3 8 9 445 1 2 3 0 125 5 41 4 5 552 2.36  45656 72.3 ]
	disp(ifft(f));
	disp("********6***********");
	// full complex

	g = a + [10 1 15 3.5 ]*%i; //z2Fftz2
	disp(ifft(g));
	disp("********7***********");
	h = b + [25  3.6]*%i; // z2Fftd2
	disp(ifft(h));
	disp("********8***********");
	j= c + %i*[10 2 5 6 3 8 9 445 1 2 3 0 125 5 41 4 5 552 2.36  45656 72.3 ]
	disp(ifft(j));
	disp("********************");
	disp("*******************");
	disp("*******************");
	disp("********************");


// ********************
//  MATRIX
// ********************
// one input
	aa = [10 1 15 3.5 ; 5 3 9 6  ]; //d2Fftz2
	disp(ifft(aa));
	disp("********9***********");
	bb = [25  3.6 ; 4 3.2]; // d2Fftd2
	disp(ifft(bb));
	disp("********10**********");
	cc= [10 2 5 6 3 8 9 445 1 2 3 0 125 5 41 4 5 552 2.36  45656 72.3 ; 21 8 2 5 6 3 4 123 5 6 0 5 548 7 65 4 2 166 4.12  65546 1.78 ]
	disp(ifft(cc));
	disp("********11**********");
	// pure imaginary
	dd = [10 1 15 3.5 ; 5 8 6 3 ]*%i; //z2Fftz2
	disp(ifft(dd));
	disp("********12**********");
	ee = [25  3.6 ; 4 5.9 ]*%i; // z2Fftd2
	disp(ifft(ee));
	disp("********13**********");
	ff = %i*[10 2 5 6 3 8 9 445 1 2 3 0 125 5 41 4 5 552 2.36  45656 72.3;  21 8 2 5 6 3 4 123 5 6 0 5 548 7 65 4 2 166 4.12  65546 1.78 ]

	disp(ifft(ff));
	disp("********14**********");
	// full complex

	gg = aa + dd; //z2Fftz2
	disp(ifft(gg));
	disp("********15**********");
	hh = bb + ee; // z2Fftd2
	disp(ifft(hh));
	disp("********16**********");
	jj= cc + ff
	disp(ifft(jj));


endfunction

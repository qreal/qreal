//SCI2C: DEFAULT_PRECISION= FLOAT
function test_float_Fft()

disp(fft(4.4))
disp(fft(%i + 3.4))


// one input
	a = [10 1 15 3.5 ]; //d2Fftz2
	disp(fft(a));
	disp("");
	b = [25  3.6]; // d2Fftd2
	disp(fft(b));
	disp("");
	c= [10 2 5 6 3 8 9 445 1 2 3 0 125 5 41 4 5 552 2.36  45656 72.3 ]
	disp(fft(c));
	disp("");
	// pure imaginary
	d = [10 1 15 3.5 ]*%i; //z2Fftz2
	disp(fft(d));
	disp("");
	e = [25  3.6]*%i; // z2Fftd2
	disp(fft(e));
	disp("");
	f= %i*[10 2 5 6 3 8 9 445 1 2 3 0 125 5 41 4 5 552 2.36  45656 72.3 ]
	disp(fft(f));
	disp("");
	// full complex

	g = a + [10 1 15 3.5 ]*%i; //z2Fftz2
	disp(fft(g));
	disp("");
	h = b + [25  3.6]*%i; // z2Fftd2
	disp(fft(h));
	disp("");
	j= c + %i*[10 2 5 6 3 8 9 445 1 2 3 0 125 5 41 4 5 552 2.36  45656 72.3 ]
	disp(fft(j));
	disp("");
	disp("*******************");
	disp("*******************");
	disp("");
// two output
// call ifft
	disp(fft(a,1));
	disp("");
	disp(fft(b,1));
	disp("");
	disp(fft(c,1));
	disp("");
	disp(fft(d,1));
	disp("");
	disp(fft(e,1));
	disp("");
	disp(fft(f,1));
	disp("");
	// full complex
	disp(fft(g,1));
	disp("");
	disp(fft(h,1));
	disp("");
	disp(fft(j,1));
	disp("");
	disp("");
	disp("*******************");
	disp("*******************");
	disp("");
//
	disp(fft(a,-1));
	disp("");
	disp(fft(b,-1));
	disp("");
	disp(fft(c,-1));
	disp("");
	disp(fft(d,-1));
	disp("");
	disp(fft(e,-1));
	disp("");
	disp(fft(f,-1));
	disp("");
	// full complex
	disp(fft(g,-1));
	disp("");
	disp(fft(h,-1));
	disp("");
	disp(fft(j,-1));
	disp("");


// ********************
//  MATRIX
// ********************
// one input
	aa = [10 1 15 3.5 ; 5 3 9 6  ]; //d2Fftz2
	disp(fft(aa));
	disp("");
	bb = [25  3.6 ; 4 3.2]; // d2Fftd2
	disp(fft(bb));
	disp("");
	cc= [10 2 5 6 3 8 9 445 1 2 3 0 125 5 41 4 5 552 2.36  45656 72.3 ; 21 8 2 5 6 3 4 123 5 6 0 5 548 7 65 4 2 166 4.12  65546 1.78 ]
	disp(fft(cc));
	disp("");
	// pure imaginary
	dd = [10 1 15 3.5 ; 5 8 6 3 ]*%i; //z2Fftz2
	disp(fft(dd));
	disp("");
	ee = [25  3.6 ; 4 5.9 ]*%i; // z2Fftd2
	disp(fft(ee));
	disp("");
	ff = %i*[10 2 5 6 3 8 9 445 1 2 3 0 125 5 41 4 5 552 2.36  45656 72.3;  21 8 2 5 6 3 4 123 5 6 0 5 548 7 65 4 2 166 4.12  65546 1.78 ]

	disp(fft(ff));
	disp("");
	// full complex

	gg = aa + dd; //z2Fftz2
	disp(fft(gg));
	disp("");
	hh = bb + ee; // z2Fftd2
	disp(fft(hh));
	disp("");
	jj= cc + ff
	disp(fft(jj));
	disp("");
	disp("*******************");
	disp("*******************");
	disp("");
// two output
// call ifft
	disp(fft(aa,1));
	disp("");
	disp(fft(bb,1));
	disp("");
	disp(fft(cc,1));
	disp("");
	disp(fft(dd,1));
	disp("");
	disp(fft(ee,1));
	disp("");
	disp(fft(ff,1));
	disp("");
	// full complex
	disp(fft(gg,1));
	disp("");
	disp(fft(hh,1));
	disp("");
	disp(fft(jj,1));
	disp("");
	disp("");
	disp("*******************");
	disp("*******************");
	disp("");
//
	disp(fft(aa,-1));
	disp("");
	disp(fft(bb,-1));
	disp("");
	disp(fft(cc,-1));
	disp("");
	disp(fft(dd,-1));
	disp("");
	disp(fft(ee,-1));
	disp("");
	disp(fft(ff,-1));
	disp("");
	// full complex
	disp(fft(gg,-1));
	disp("");
	disp(fft(hh,-1));
	disp("");
	disp(fft(jj,-1));
	disp("");

endfunction

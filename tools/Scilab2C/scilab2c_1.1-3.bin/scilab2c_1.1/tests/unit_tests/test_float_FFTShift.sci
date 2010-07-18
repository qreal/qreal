//SCI2C: DEFAULT_PRECISION= FLOAT
function test_float_FFTShift()
	
	disp(fftshift(5.3));// d0*d0
	disp(fftshift(5.3*%i));// z0*z0
	disp(fftshift(5.3  + 5.6*%i ));// z0*z0


	a = fft ([10 1 15 3.5 ]);
	disp(fftshift(a)); 
	
	b = [ 14 15 2 6 ; 1 2 6 4 ; 48.5 3 9.3 1 ];
	disp(fftshift(b)); 
	disp(fftshift(b , 1)); 
	disp(fftshift(b , 2)); 
	//disp(fftshift(b , "all")); 


	c = [10 1 15 3.5 ]*%i;
	disp(fftshift(c)); 
	
	d = [ 14 15 2 6 ; 1 2 6 4 ; 48.5 3 9.3 1 ]*%i;
	disp(fftshift(b)); 
	disp(fftshift(b , 1)); 
	disp(fftshift(b , 2)); 
	//disp(fftshift(b , "all")); 


	e = c +  [10 1 15 3.5 ]*%i;
	disp(fftshift(e)); 
	
	f = [ 18 15 7 6 ; 1 5 6 4 ; 43.5 1 9.3 1 ] + [ 14 15 2 6 ; 1 2 6 4 ; 48.5 3 9.3 1 ]*%i;
	disp(fftshift(f)); 
	disp(fftshift(f , 1)); 
	disp(fftshift(f , 2)); 
	//disp(fftshift(f , "all")); 

endfunction 

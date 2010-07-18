//SCI2C: DEFAULT_PRECISION= FLOAT

function test_float_Stdevf()
	disp (stdevf (4 , 5 )) ;	
	disp ("*******1*******");
	disp (stdevf ([4 5 6], [1 2 3]));
	disp ("*******2*******");
	disp (stdevf (4 , 5*%i )) ;	
	disp ("*******3*******");
	disp (stdevf ([4 5 6], [1 2 3]*%i));
	disp ("*******4*******");



	disp (stdevf ([4 5 6 ; 7 8 9], [1 2 3 ; 4 5 6],1));
	disp ("*******5*******");
	disp (stdevf ([4 7; 5 8; 6 9], [1 4; 2 5; 3 6],2));
// complex
	disp ("*******6*******");
	disp (stdevf (4 +4*%i , 5 )) ;	
	disp ("*******7*******");
	disp (stdevf ([4*%i, 5+6*%i, 6], [1 2 3]));
        disp ("*******8*******");
	disp (stdevf (4 +4*%i , 5*%i )) ;	
	disp ("*******9*******");
	disp (stdevf ([4*%i, 5+6*%i, 6], [1 2 3]*%i));
	disp ("******10*******");


	disp (stdevf ([4 5 6 ; 7 8 9]*%i, [1 2 3 ; 4 5 6],1));
	disp ("******11*******");
	disp (stdevf ([4 7; 5 8; 6 9], [1 4; 2 5; 3 6]*%i,2));
	disp (stdevf ([4 5 6 ; 7 8 9],[1 2 3 ; 4 8 9]  + [1 2 3 ; 4 5 6]*%i,1));
	disp ("******12*******");
	disp (stdevf ([4 5 6 ; 7 8 9], [1 2 3 ; 4 5 6]*%i,1));
	disp ("******13*******");
	disp (stdevf ([4 7; 5 8; 6 9]*%i, [1 4; 2 5; 3 6],2));
	disp ("******14*******");

	disp (stdevf ([4 5 6 ; 7 8 9]*%i, [1 2 3 ; 4 5 6]*%i,1));
	disp ("******15*******");
	disp (stdevf ([4 7; 5 8; 6 9]*%i, [1 4; 2 5; 3 6]*%i,2));
	disp ("******16*******");
//

endfunction 

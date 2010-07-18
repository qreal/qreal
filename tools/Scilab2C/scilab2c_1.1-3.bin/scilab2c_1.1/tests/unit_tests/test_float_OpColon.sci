//SCI2C: DEFAULT_PRECISION= FLOAT
function test_float_OpColon()


//	A = [1 2 3 4 ; 5 6 7 8 ; 9 10 11 12];
//	B = A * %i ;
d  = %i ;
a = %pi ;
b = %e  ;
c = [7 8 5 ];
disp(zeros ( a , %e ));

disp(zeros ( %pi , %e));
disp(zeros(c));
disp(zeros ([4 5 6 ; 4 5 6]) );

	disp(0:1:3);
	disp(1:1);
	disp(4+53*%i:0);
//

	disp(2:5);
	disp(4+3*%i:8+%i);
//
		
	disp(1:5:1);
	disp(%i:%i+1:2);
//

	disp(2:7:5);
	disp(4+3*%i:4:8+%i);
//a =4 + 53 *%i : 0 ;
endfunction
//OUT(1).SZ(2)= FA_INT(
//		     FA_ADD(
//			     FA_DIV(
//			            FA_SUB(FA_REAL(IN(3).VAL,IN(3).TP),FA_REAL(IN(1).VAL, IN(3).TP))
//                                   ,FA_REAL(IN(2).VAL,IN(2).TP)
//                                   )
//                            ,''1''
//			   )
//		    )

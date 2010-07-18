//SCI2C: DEFAULT_PRECISION= FLOAT
function test_float_Convol()


 disp(convol(4,4.5)); //d0d0Convold0
 disp(convol(%i + 3 ,2*%i + 4)); // z0z0Convolz0


 a = [1 2 3 4 5 %e %pi];
 b = [1 7 8 5 1 6 ];

 disp(convol(a,b));
 disp(convol(b,b)); // d2d2Convold2


 disp(convol(4,a)); //d0d2Convold2
 disp(convol(a,4)); // d2d0Convold2


 c = [1 2 3 4 5 %e %pi] * %i;
 d = [1 7 8 5 1 6 ] * %i;

 disp(convol(c,d));
 disp(convol(d,d)); // z2z2Convolz2


 disp(convol(%i + 3 ,c)); // z0z2Convolz2
 disp(convol(d ,2*%i + 4)); // z2z0Convolz2
	
// with 2 outputs


// [zz yy] = convol(4,4.5); //d0d0Convold0d0
// disp( zz );
 //disp( yy );

// [xx ww ] = convol(%i + 3 ,2*%i + 4); // z0z0Convolz0z0
// disp( xx);
// disp( ww );

// [vv uu]  =convol(a,b);
// disp( vv); // d2d2Convold2d2
// disp( uu );


// [tt ss] = convol(4,a); //d0d2Convold2d2
// disp( tt);
// disp( ss );



// [rr qq] = convol(a,4); // d2d0Convold2d2
// disp( rr );
// disp( qq );



// with 2 outputs

// [pp oo]  =convol(c,d);
// disp( pp); // z2z2Convolz2z2
// disp( oo );


// [mm nn] = convol(4,c); //z0z2Convolz2z2
// disp( mm);
// disp( nn );



// [ll kk] = convol(c,4); // z2z0Convolz2z2
// disp( ll );
 //disp( kk);
 

// three input i

// with 2 outputs


 //[zzz yyy] = convol(4,4.5,18); //d0d0d0Convold0d0
 //disp( zzz );
 //disp( yyy );

 //[xxx www ] = convol(%i + 3 ,2*%i + 4,%i); // z0z0z0Convolz0z0
 //disp( xxx);
 //disp( www );

// bbis = [4 7 9 10];

 //[vvv uuu]  =convol(a,b,bbis);
// disp( vvv); // d2d2d2Convold2d2
// disp( uuu );
//

 //[ttt sss] = convol(4,a, 8); //d0d2d0Convold2d0
 //disp( ttt);
 //disp( sss );

// with 2 outputs
//dbis = [10 13 14] *%i;
// [ppp ooo]  =convol(c,d, dbis);
// disp( ppp); // z2z2z2Convolz2z2
// disp( ooo );


// [mmm nnn] = convol(4 * %i ,c , %i); //z0z2z0Convolz2z0
// disp( mmm);
// disp( nnn);

//mixed up

// disp(convol(a,c)); //d2z2Convolz2
// disp(convol(c,a)); // z2d2Convolz2
// disp(convol(4,c)); //d0z2Convolz2
// disp(convol(%i + 3 ,a)); // z0d2Convolz2

// disp(convol(a, %i)); //d2z0Convolz2
// disp(convol(c , 4)); // z2d0Convolz2





endfunction 

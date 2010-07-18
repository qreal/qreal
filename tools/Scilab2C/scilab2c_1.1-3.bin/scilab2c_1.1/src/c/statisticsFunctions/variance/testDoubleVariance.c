/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2006-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <assert.h>
#include <stdio.h>

#include "variance.h"

#define SINGLE  1.323312455
#define LINES   7
#define COLUMNS 10
#define MATRIX  {0.102326436434,0.939044147730,0.638607255649,0.059883427806,0.311199211050,\
 0.377510951832,0.909709410276,0.928138985299,0.855155082885,0.308869514614,\
0.729239261709,0.127006734256,0.242402311414,0.807912770193,0.621117659844,\
 0.081452373881,0.088844371960,0.587072817143,0.068080495112,0.995457543526,\
0.835322400089,0.656235548668,0.767905956600,0.706818781793,0.187699332368,\
 0.568810169585,0.401039466728,0.397196868435,0.483212330844,0.161224133335,\
0.932864250615,0.719208867755,0.262409000657,0.931377100293,0.596724558622,\
 0.310327019077,0.350161732174,0.104841318447,0.830732351169,0.804621329065,\
0.050784428138,0.551920620725,0.125952405389,0.083156193607,0.420623748098,\
 0.168616071343,0.707545555662,0.104633864947,0.465604968835,0.489758284763,\
0.167727907188,0.760689914692,0.454900974408,0.063926273491,0.87152507063,0.978668818716,\
 0.135898929089,0.484713065904,0.785042577423,0.049412418623,\
0.947593610268,0.884073690511,0.212796610314,0.654832502827,0.007690255996,\
 0.031808692031,0.706323316786,0.008731670678,0.270014822017,0.153652121313}


#define MATRIXROW {0.1688249423502767021521  ,  0.0729081225272336097554  ,  0.0579879770921138057438  ,  0.1499989757031960635469  ,  0.0850863194040760717485 ,    0.1085086384507867873772  ,  0.0969379859569881041637 ,   0.1072816602642319844252 ,   0.0913638718167188862429 ,   0.1285778184208021535095  }
#define MATRIXCOL {    0.1233078274276283442523,  \
    0.1232135941992091865416 , \
    0.0540377259589001762707 , \
    0.0925512303008872161403 , \
    0.0555055644879708723338 , \
    0.1276299219577190779962 , \
    0.1385167433745558840386  }


#define IMATRIX {0.05826747464016080,0.99946373142302036,0.85600351681932807,0.62329693790525198,\
 0.49792320514097810,0.44210509583353996,0.55833499459549785,0.09633230511099100,\
 0.80100952507928014,0.34466254524886608,\
0.48831309471279383,0.95087061496451497,0.01332767866551876,0.76261807000264525,\
 0.74346329551190138,0.65157829830422997,0.57006288319826126,0.70580983115360141,\
 0.51323400903493166,0.64977857517078519,\
0.80352442665025592,0.69910932797938585,0.69258948462083936,0.91169391758739948,\
 0.92454590043053031,0.27637310232967138,0.31692579621449113,0.86305770650506020,\
 0.20109100220724940,0.01225362811237574,\
0.38199013099074364,0.96006405679509044,0.16440964583307505,0.59608811559155583,\
 0.72101737372577190,0.58661046391353011,0.99326277803629637,0.00761850038543344,\
 0.78608208894729614,0.89965870184823871,\
0.43115562805905938,0.81857266277074814,0.83410377753898501,0.55516970623284578,\
 0.51345925079658628,0.04770902730524540,0.80747798969969153,0.80489510949701071,\
 0.78650354826822877,0.30791273340582848,\
0.75731822755187750,0.53423820668831468,0.42618893459439278,0.85448804078623652,\
 0.92601215932518244,0.94169309409335256,0.85547966323792934,0.59637623047456145,\
 0.69515300076454878,0.18359116325154901,\
0.01200280850753188,0.2255702270194888,0.34853330114856362,0.55313225090503693,\
 0.63695094687864184,0.96132039744406939,0.50314606027677655,0.11768362112343311,\
 0.42481321236118674,0.52963322307914495}

#define RMATRIX {0.53868199465796351,0.65326874051243067,0.37497402401641011,0.60422550700604916,\
 0.33622304117307067,0.03531436901539564,0.33213760564103723,0.39493087679147720,\
 0.04170337272807956,0.87363853026181459,\
0.65950810909271240,0.31221040291711688,0.15333442110568285,0.66478573577478528,\
 0.44319023378193378,0.40884594758972526,0.94536898937076330,0.67733758920803666,\
 0.18673646822571754,0.92331133363768458,\
0.55921846115961671,0.75012728199362755,0.23257926432415843,0.57102064136415720,\
 0.60969385923817754,0.67336730472743511,0.07624059682711959,0.10437540244311094,\
 0.95672677317634225,0.19771346449851990,\
0.05278092902153730,0.75808868417516351,0.17633479088544846,0.01146994484588504,\
 0.36732212174683809,0.19948644982650876,0.52714426256716251,0.21445603063330054,\
 0.94751045759767294,0.04272260749712586,\
0.03257346292957664,0.53217577841132879,0.87635089689865708,0.15471007302403450,\
 0.25448470888659358,0.91815057490020990,0.72509902389720082,0.87545845471322536,\
 0.05635281419381499,0.01432112138718367,\
0.57406943850219250,0.80984140699729323,0.01664119493216276,0.93888836959376931,\
 0.11264799535274506,0.66036546928808093,0.76042845565825701,0.25908330874517560,\
 0.88014078326523304,0.70332178613170981,\
0.94614937948063016,0.78529163636267185,0.20241560926660895,0.10981105919927359,\
 0.75415370846167207,0.15320260450243950,0.59521253732964396,0.71318271104246378,\
 0.51107599260285497,0.52125945501029491}






#define IMATRIXROW {-0.0419293790250184,-0.05416405376783551,+0.11775191182077104,+0.08015791690354153,+0.00011032748454003,\
-0.10124210045965254,0.06720614014223115,+0.02470873836336094,-0.07082037296251543,-0.00621987869853545}

#define RMATRIXROW {0.01439621349781126,-0.04697393442028135,-0.03018604392150223,0.09765990436118642,0.01606884471097247,\
-0.00604250542333471,0.02870576510687836,-0.04985742926427023,0.12356001472881707,0.05909180726174274}

#define IMATRIXCOL {-0.01904810719805961,+0.04121453132770223,+0.00378553967089328,\
+0.10307670507342410,+0.03497427750676810,+0.01761849804019567,-0.10182321471060848}

#define RMATRIXCOL {-0.02739893277693160,0.01792852626358409,-0.02205084134302381,\
-0.01012610864032573,0.07317682977907164,0.04762062457313784,0.00849011627117621}


static void  dvariancesTest (void ) {
    
    double toTest =  dvariances (SINGLE) ;
     
    assert ( fabs ( toTest - SINGLE ) / fabs( toTest) < 1e-16 ) ;
}    


static void  drowvariancesTest (void ) {
    
    double toTest = drowvariances (SINGLE) ;
     
    assert ( fabs ( toTest - SINGLE ) / fabs( toTest) < 1e-16 ) ;
} 

static void  dcolumnvariancesTest (void) {
    
    double toTest =  dcolumnvariances (SINGLE) ;
     
    assert ( fabs ( toTest - SINGLE ) / fabs( toTest) < 1e-16 ) ;
} 


static void  zvariancesTest (void ) {
    
    doubleComplex toTest = zvariances ( DoubleComplex ( SINGLE , SINGLE ) );
     
    assert ( fabs ( zreals( toTest) - SINGLE ) / fabs( zreals( toTest)) < 1e-16 ) ;
    assert ( fabs ( zimags( toTest) - SINGLE ) / fabs( zimags( toTest)) < 1e-16 ) ;
}   

static void  zrowvariancesTest (void ) {
    
    doubleComplex toTest = zrowvariances ( DoubleComplex ( SINGLE , SINGLE ) );
     
    assert ( fabs ( zreals( toTest) - SINGLE ) / fabs( zreals( toTest)) < 1e-16 ) ;
    assert ( fabs ( zimags( toTest) - SINGLE ) / fabs( zimags( toTest)) < 1e-16 ) ;
}

static void  zcolumnvariancesTest (void ) {
    
    doubleComplex toTest = zrowvariances ( DoubleComplex ( SINGLE , SINGLE ) );
     
    assert ( fabs ( zreals( toTest) - SINGLE ) / fabs( zreals( toTest)) < 1e-16 ) ;
    assert ( fabs ( zimags( toTest) - SINGLE ) / fabs( zimags( toTest)) < 1e-16 ) ;
}   






static void  dvarianceaTest ( void){
    
    double mtoTest[] = MATRIX ;
    double result = 0.1007532974338590653352;
    double out = dvariancea ( mtoTest , LINES*COLUMNS );  ;
    

    
    printf ( "out : %1.20f\t result : %1.20f\t\n" , out , result ) ; 
    

    
    assert ( fabs ( out - result ) / fabs( out ) < 3e-15 ) ;
    
}


static void drowvarianceaTest ( void )
{
    int i = 0 ;
    
    double mtoTest[] = MATRIX ; 
    double result[LINES] =  MATRIXCOL ;
    double out[LINES]  ;
    
    drowvariancea ( mtoTest , COLUMNS , LINES , out ) ;
    
    for ( i = 0 ; i < LINES ; i++)
    {
            printf ( "ROWout : %1.20f\t result : %1.20f\t\n" , out[i] , result [i] ) ; 
           assert ( fabs ( out[i] - result[i] ) / fabs( out[i] ) < 3e-16 ) ;
    }
    
}

static void dcolumnvarianceaTest ( void )
{
    int i = 0 ;
    
    double mtoTest[] = MATRIX ; 
    double result[COLUMNS] =  MATRIXROW ;
    double out[ COLUMNS]  ;
  
    
    
    dcolumnvariancea ( mtoTest ,  COLUMNS, LINES , out ) ;
    
    for ( i = 0 ; i <  COLUMNS; i++)
    {
            printf ( "%dCOLout : %1.20f\t result : %1.20f\t\n" ,i, out[i] , result [i] ) ;  } 
     for ( i = 0 ; i <  COLUMNS; i++)
    {
          assert ( fabs ( out[i] - result[i] ) / fabs( out[i] ) < 3e-16 ) ;
    }
    
}




static void  zvarianceaTest ( void){
    
    double  rmtoTest [] = RMATRIX ;
    double  imtoTest [] = IMATRIX ;
    doubleComplex Result = DoubleComplex ( 0.01225832057910708 , 0.01071188208687752 );
    
    doubleComplex* mtoTest = DoubleComplexMatrix ( rmtoTest , imtoTest , LINES*COLUMNS ) ;
    doubleComplex out = zvariancea ( mtoTest ,  LINES*COLUMNS );  ;
    
            assert ( fabs(  zreals(out) -  zreals (Result) ) / fabs (zreals (out)) < 3e-16 );
            assert(  fabs(  zimags(out) -  zimags (Result )) / fabs (zimags (out)) < 3e-15 );

    
}

static void zrowvarianceaTest (void ) {
   
    int i = 0 ;
    
    double  rmtoTest [] = RMATRIX ;
    double  imtoTest [] = IMATRIX ;
    double  rResult  [] = RMATRIXROW;
    double  iResult  [] = IMATRIXROW ;
    
    doubleComplex out[COLUMNS];
    doubleComplex* mtoTest = DoubleComplexMatrix ( rmtoTest , imtoTest , LINES*COLUMNS  ) ;
    doubleComplex* Result  = DoubleComplexMatrix ( rResult  , iResult  , COLUMNS );
    
    zrowvariancea ( mtoTest , LINES , COLUMNS , out ) ; 
    
    /*FIXME test failed under 1-e04 precision */
    for ( i = 0 ; i < COLUMNS ; i++)
    {
        printf ( "%d out : %e  %e \t result %e %e \n" , i , zreals(out[i])  , zimags(out[i]), zreals(Result[i]) ,zimags( Result[i]));
        assert ( fabs(  zreals(out[i]) -  zreals (Result[i]) ) / fabs (zreals (out[i])) < 3e-8 );
        assert(  fabs(  zimags(out[i]) -  zimags (Result[i])) / fabs (zimags (out[i])) < 3e-15);
      
    }
}



static void zcolumnvarianceaTest ( void) {
        int i = 0 ;
    
    double  rmtoTest [] = RMATRIX ;
    double  imtoTest [] = IMATRIX ;
    double  rResult  [] = RMATRIXCOL;
    double  iResult  [] = IMATRIXCOL ;
    
    doubleComplex out[LINES];
    doubleComplex* mtoTest = DoubleComplexMatrix ( rmtoTest , imtoTest , LINES*COLUMNS  ) ;
    doubleComplex* Result  = DoubleComplexMatrix ( rResult  , iResult  , LINES );
    
    zcolumnvariancea ( mtoTest , LINES , COLUMNS , out ) ; 
    
   
    for ( i = 0 ; i < LINES ; i++)
    {
        printf ( "%d out : %e  %e \t result %e %e \n" , i , zreals(out[i])  , zimags(out[i]), zreals(Result[i]) ,zimags( Result[i]));
        assert ( fabs(  zreals(out[i]) -  zreals (Result[i]) ) / fabs (zreals (out[i])) < 3e-16 );
        assert(  fabs(  zimags(out[i]) -  zimags (Result[i])) / fabs (zimags (out[i])) < 3e-16);
      
    }
    
    
    
}





static int testDoubleVariance   (void) {
  printf("\n\n\n\n*********************\n");
  printf("***** Double  Tests ****\n");
  printf("*********************\n");

  printf("\n\t>>>>singleton real  Tests\n"); 
   dcolumnvariancesTest () ;   
  dvariancesTest () ;    
  drowvariancesTest () ;


  printf("\n\t>>>>singleton complex  Tests\n");     
    
  zrowvariancesTest () ;
  zcolumnvariancesTest () ;
  zvariancesTest () ;
    
    
    
  printf("\n\t>>>>array real  Tests\n"); 
    
  dvarianceaTest () ;    
  
  dcolumnvarianceaTest () ;   
  drowvarianceaTest ( );
    
    
  printf("\n\t>>>>array  complex  Tests\n"); 
    
  zvarianceaTest () ;  
  zrowvarianceaTest ( );  
  zcolumnvarianceaTest () ;


  return 0;
}

int main(void) {
  assert(testDoubleVariance () == 0);
  return 0;
}


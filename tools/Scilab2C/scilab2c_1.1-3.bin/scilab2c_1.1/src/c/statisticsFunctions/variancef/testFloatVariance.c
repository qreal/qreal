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

#define SINGLE  1.323312455f
#define LINES   7
#define COLUMNS 10
#define MATRIX  {0.102326436434f,0.939044147730f,0.638607255649f,0.059883427806f,0.311199211050f,\
 0.377510951832f,0.909709410276f,0.928138985299f,0.855155082885f,0.308869514614f,\
0.729239261709f,0.127006734256f,0.242402311414f,0.807912770193f,0.621117659844f,\
 0.081452373881f,0.088844371960f,0.587072817143f,0.068080495112f,0.995457543526f,\
0.835322400089f,0.656235548668f,0.767905956600f,0.706818781793f,0.187699332368f,\
 0.568810169585f,0.401039466728f,0.397196868435f,0.483212330844f,0.161224133335f,\
0.932864250615f,0.719208867755f,0.262409000657f,0.931377100293f,0.596724558622f,\
 0.310327019077f,0.350161732174f,0.104841318447f,0.830732351169f,0.804621329065f,\
0.050784428138f,0.551920620725f,0.125952405389f,0.083156193607f,0.420623748098f,\
 0.168616071343f,0.707545555662f,0.104633864947f,0.465604968835f,0.489758284763f,\
0.167727907188f,0.760689914692f,0.454900974408f,0.063926273491f,0.87152507063f,0.978668818716f,\
 0.135898929089f,0.484713065904f,0.785042577423f,0.049412418623f,\
0.947593610268f,0.884073690511f,0.212796610314f,0.654832502827f,0.007690255996f,\
 0.031808692031f,0.706323316786f,0.008731670678f,0.270014822017f,0.153652121313f}


#define MATRIXROW {0.168824942350f,0.072908122527f,0.057987977092f,0.149998975703f,0.085086319404f,\
0.108508638451f,0.096937985957f,0.107281660264f,0.091363871817f,0.128577818421f}

#define MATRIXCOL { 0.123307827428f,0.123213594199f,0.054037725959f,0.092551230301f,0.055505564488f,\
0.127629921958f,0.138516743375f }


#define IMATRIX {0.05826747464016080f,0.99946373142302036f,0.85600351681932807f,0.62329693790525198f,\
 0.49792320514097810f,0.44210509583353996f,0.55833499459549785f,0.09633230511099100f,\
 0.80100952507928014f,0.34466254524886608f,\
0.48831309471279383f,0.95087061496451497f,0.01332767866551876f,0.76261807000264525f,\
 0.74346329551190138f,0.65157829830422997f,0.57006288319826126f,0.70580983115360141f,\
 0.51323400903493166f,0.64977857517078519f,\
0.80352442665025592f,0.69910932797938585f,0.69258948462083936f,0.91169391758739948f,\
 0.92454590043053031f,0.27637310232967138f,0.31692579621449113f,0.86305770650506020f,\
 0.20109100220724940f,0.01225362811237574f,\
0.38199013099074364f,0.96006405679509044f,0.16440964583307505f,0.59608811559155583f,\
 0.72101737372577190f,0.58661046391353011f,0.99326277803629637f,0.00761850038543344f,\
 0.78608208894729614f,0.89965870184823871f,\
0.43115562805905938f,0.81857266277074814f,0.83410377753898501f,0.55516970623284578f,\
 0.51345925079658628f,0.04770902730524540f,0.80747798969969153f,0.80489510949701071f,\
 0.78650354826822877f,0.30791273340582848f,\
0.75731822755187750f,0.53423820668831468f,0.42618893459439278f,0.85448804078623652f,\
 0.92601215932518244f,0.94169309409335256f,0.85547966323792934f,0.59637623047456145f,\
 0.69515300076454878f,0.18359116325154901f,\
0.01200280850753188f,0.2255702270194888f,0.34853330114856362f,0.55313225090503693f,\
 0.63695094687864184f,0.96132039744406939f,0.50314606027677655f,0.11768362112343311f,\
 0.42481321236118674f,0.52963322307914495f}

#define RMATRIX {0.53868199465796351f,0.65326874051243067f,0.37497402401641011f,0.60422550700604916f,\
 0.33622304117307067f,0.03531436901539564f,0.33213760564103723f,0.39493087679147720f,\
 0.04170337272807956f,0.87363853026181459f,\
0.65950810909271240f,0.31221040291711688f,0.15333442110568285f,0.66478573577478528f,\
 0.44319023378193378f,0.40884594758972526f,0.94536898937076330f,0.67733758920803666f,\
 0.18673646822571754f,0.92331133363768458f,\
0.55921846115961671f,0.75012728199362755f,0.23257926432415843f,0.57102064136415720f,\
 0.60969385923817754f,0.67336730472743511f,0.07624059682711959f,0.10437540244311094f,\
 0.95672677317634225f,0.19771346449851990f,\
0.05278092902153730f,0.75808868417516351f,0.17633479088544846f,0.01146994484588504f,\
 0.36732212174683809f,0.19948644982650876f,0.52714426256716251f,0.21445603063330054f,\
 0.94751045759767294f,0.04272260749712586f,\
0.03257346292957664f,0.53217577841132879f,0.87635089689865708f,0.15471007302403450f,\
 0.25448470888659358f,0.91815057490020990f,0.72509902389720082f,0.87545845471322536f,\
 0.05635281419381499f,0.01432112138718367f,\
0.57406943850219250f,0.80984140699729323f,0.01664119493216276f,0.93888836959376931f,\
 0.11264799535274506f,0.66036546928808093f,0.76042845565825701f,0.25908330874517560f,\
 0.88014078326523304f,0.70332178613170981f,\
0.94614937948063016f,0.78529163636267185f,0.20241560926660895f,0.10981105919927359f,\
 0.75415370846167207f,0.15320260450243950f,0.59521253732964396f,0.71318271104246378f,\
 0.51107599260285497f,0.52125945501029491f}






#define IMATRIXROW {-0.0419293790250184f,-0.05416405376783551f,+0.11775191182077104f,+0.08015791690354153f,+0.00011032748454003f,\
-0.10124210045965254f,0.06720614014223115f,+0.02470873836336094f,-0.07082037296251543f,-0.00621987869853545f}

#define RMATRIXROW {0.01439621349781126f,-0.04697393442028135f,-0.03018604392150223f,0.09765990436118642f,0.01606884471097247f,\
-0.00604250542333471f,0.02870576510687836f,-0.04985742926427023f,0.12356001472881707f,0.05909180726174274f}

#define IMATRIXCOL {-0.01904810719805961f,+0.04121453132770223f,+0.00378553967089328f,\
+0.10307670507342410f,+0.03497427750676810f,+0.01761849804019567f,-0.10182321471060848f}

#define RMATRIXCOL {-0.02739893277693160f,0.01792852626358409f,-0.02205084134302381f,\
-0.01012610864032573f,0.07317682977907164f,0.04762062457313784f,0.00849011627117621f}


static void  svariancesTest (void ) {
    
    float toTest =  svariances (SINGLE) ;
     
    assert ( fabs ( toTest - SINGLE ) / fabs( toTest) < 1e-06 ) ;
}    


static void  srowvariancesTest (void ) {
    
    float toTest = srowvariances (SINGLE) ;
     
    assert ( fabs ( toTest - SINGLE ) / fabs( toTest) < 1e-06 ) ;
} 

static void  scolumnvariancesTest (void) {
    
    float toTest =  scolumnvariances (SINGLE) ;
     
    assert ( fabs ( toTest - SINGLE ) / fabs( toTest) < 1e-06 ) ;
} 


static void  cvariancesTest (void ) {
    
    floatComplex toTest = cvariances ( FloatComplex ( SINGLE , SINGLE ) );
     
    assert ( fabs ( creals( toTest) - SINGLE ) / fabs( creals( toTest)) < 1e-06 ) ;
    assert ( fabs ( cimags( toTest) - SINGLE ) / fabs( cimags( toTest)) < 1e-06 ) ;
}   

static void  crowvariancesTest (void ) {
    
    floatComplex toTest = crowvariances ( FloatComplex ( SINGLE , SINGLE ) );
     
    assert ( fabs ( creals( toTest) - SINGLE ) / fabs( creals( toTest)) < 1e-06 ) ;
    assert ( fabs ( cimags( toTest) - SINGLE ) / fabs( cimags( toTest)) < 1e-06 ) ;
}

static void  ccolumnvariancesTest (void ) {
    
    floatComplex toTest = crowvariances ( FloatComplex ( SINGLE , SINGLE ) );
     
    assert ( fabs ( creals( toTest) - SINGLE ) / fabs( creals( toTest)) < 1e-06 ) ;
    assert ( fabs ( cimags( toTest) - SINGLE ) / fabs( cimags( toTest)) < 1e-06 ) ;
}   






static void  svarianceaTest ( void){
    
    float mtoTest[] = MATRIX ;
    float result = 0.100753297434f;
    float out = svariancea ( mtoTest , LINES*COLUMNS );  ;
    

    
    printf ( "out : %e\t result : %e\t\n" , out , result ) ; 
    

    
    assert ( fabs ( out - result ) / fabs( out ) < 1e-06 ) ;
    
}


static void srowvarianceaTest ( void )
{
    int i = 0 ;
    
    float mtoTest[] = MATRIX ; 
    float result[COLUMNS] =  MATRIXROW ;
    float out[COLUMNS]  ;
    
    srowvariancea ( mtoTest , LINES , COLUMNS , out ) ;
    
    for ( i = 0 ; i < COLUMNS ; i++)
    {
            printf ( "ROWout : %e\t result : %e\t\n" , out[i] , result [i] ) ; 
           assert ( fabs ( out[i] - result[i] ) / fabs( out[i] ) < 1e-06 ) ;
    }
    
}

static void scolumnvarianceaTest ( void )
{
    int i = 0 ;
    
    float mtoTest[] = MATRIX ; 
    float result[LINES] =  MATRIXCOL ;
    float out[ LINES]  ;
  
    
    
    scolumnvariancea ( mtoTest , LINES , COLUMNS , out ) ;
    
    for ( i = 0 ; i <  LINES ; i++)
    {
            printf ( "%dCOLout : %e\t result : %e\t\n" ,i, out[i] , result [i] ) ; 
          assert ( fabs ( out[i] - result[i] ) / fabs( out[i] ) < 1e-06 ) ;
    }
    
}




static void  cvarianceaTest ( void){
    
    float  rmtoTest [] = RMATRIX ;
    float  imtoTest [] = IMATRIX ;
    floatComplex Result = FloatComplex ( 0.01225832057910708f , 0.01071188208687752f );
    
    floatComplex* mtoTest = FloatComplexMatrix ( rmtoTest , imtoTest , LINES*COLUMNS ) ;
    floatComplex out = cvariancea ( mtoTest ,  LINES*COLUMNS );  ;
    
            assert ( fabs(  creals(out) -  creals (Result) ) / fabs (creals (out)) < 1e-06 );
            assert(  fabs(  cimags(out) -  cimags (Result )) / fabs (cimags (out)) < 1e-06 );

    
}

static void crowvarianceaTest (void ) {
   
    int i = 0 ;
    
    float  rmtoTest [] = RMATRIX ;
    float  imtoTest [] = IMATRIX ;
    float  rResult  [] = RMATRIXROW;
    float  iResult  [] = IMATRIXROW ;
    
    floatComplex out[COLUMNS];
    floatComplex* mtoTest = FloatComplexMatrix ( rmtoTest , imtoTest , LINES*COLUMNS  ) ;
    floatComplex* Result  = FloatComplexMatrix ( rResult  , iResult  , COLUMNS );
    
    crowvariancea ( mtoTest , LINES , COLUMNS , out ) ; 
    
    /*FIXME test failed under 1-e04 precision */
    for ( i = 0 ; i < COLUMNS ; i++)
    {
        printf ( "%d out : %e  %e \t result %e %e \n" , i , creals(out[i])  , cimags(out[i]), creals(Result[i]) ,cimags( Result[i]));
        assert ( fabs(  creals(out[i]) -  creals (Result[i]) ) / fabs (creals (out[i])) < 3e-6 );
        assert(  fabs(  cimags(out[i]) -  cimags (Result[i])) / fabs (cimags (out[i])) < 3e-6);
      
    }
}



static void ccolumnvarianceaTest ( void) {
        int i = 0 ;
    
    float  rmtoTest [] = RMATRIX ;
    float  imtoTest [] = IMATRIX ;
    float  rResult  [] = RMATRIXCOL;
    float  iResult  [] = IMATRIXCOL ;
    
    floatComplex out[LINES];
    floatComplex* mtoTest = FloatComplexMatrix ( rmtoTest , imtoTest , LINES*COLUMNS  ) ;
    floatComplex* Result  = FloatComplexMatrix ( rResult  , iResult  , LINES );
    
    ccolumnvariancea ( mtoTest , LINES , COLUMNS , out ) ; 
    
   
    for ( i = 0 ; i < LINES ; i++)
    {
        printf ( "%d out : %e  %e \t result %e %e \n" , i , creals(out[i])  , cimags(out[i]), creals(Result[i]) ,cimags( Result[i]));
        assert ( fabs(  creals(out[i]) -  creals (Result[i]) ) / fabs (creals (out[i])) < 1e-06 );
        assert(  fabs(  cimags(out[i]) -  cimags (Result[i])) / fabs (cimags (out[i])) < 1e-06);
      
    }
    
    
    
}





static int testFloatVariance   (void) {
  printf("\n\n\n\n*********************\n");
  printf("***** Float Tests ****\n");
  printf("*********************\n");

  printf("\n\t>>>>singleton real  Tests\n"); 
   scolumnvariancesTest () ;   
  svariancesTest () ;    
  srowvariancesTest () ;


  printf("\n\t>>>>singleton complex  Tests\n");     
    
  crowvariancesTest () ;
  ccolumnvariancesTest () ;
  cvariancesTest () ;
    
    
    
  printf("\n\t>>>>array real  Tests\n"); 
    
  svarianceaTest () ;    
  
  scolumnvarianceaTest () ;   
  srowvarianceaTest ( );
    
    
  printf("\n\t>>>>array  complex  Tests\n"); 
    
  cvarianceaTest () ;  
  crowvarianceaTest ( );  
  ccolumnvarianceaTest () ;


  return 0;
}

int main(void) {
  assert(testFloatVariance () == 0);
  return 0;
}

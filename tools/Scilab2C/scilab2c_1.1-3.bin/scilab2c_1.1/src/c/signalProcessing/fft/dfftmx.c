/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Allan SIMON
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <stdlib.h>
#include <math.h>
#include "max.h"
#include "min.h"
#include "fft_internal.h"


/*c'est moche je sais */

static double* a ;
static double* b ;
static int     ntot  ;
static int     n     ;
static int     nspan ;
static int     isn   ;
static int     m     ;
static int     kt    ;
static double*    wt    ;
static double*    ck    ;
static double*    bt    ;
static double*    sk    ;
static int*    np    ;
static int*    nfac  ;
static int     inc   ;
static int     nt    ;
static int     ks    ;

static double  rad   ;
static double  c72   ;
static double  s72   ;
static double  s120  ;

static double  aa ;
static double  ak ;
static double  akm ;
static double  akp ;
static double  aj ;
static double  ajp ;
static double  ajm ;

static double  bb ;
static double  bk ;
static double  bkm ;
static double  bkp ;
static double  bj ;
static double  bjp ;
static double  bjm ;
static double  dr ;

static double  cd ;
static double  c1 ;
static double  c2 ;
static double  c3 ;

static double  sd ;
static double  s1 ;
static double  s2 ;
static double  s3 ;

static int      kspan ;
static int     nn ;
static int     jc ;
static int     klim ;
static int     jf ;
static int     maxf ;
static int     kk ;
static int     k ;
static int     k1 ;
static int     k2 ;
static int     k3 ;
static int     k4 ;
static int     mm ;
static int     kspnn ;
static int      i ;
static int      j ;
static int      jj;

/* Prototypes */

static   void preliminaryWork (void);
static    void permute_stage1 (void);
static    void permute_stage2 (void);
static    void f4t_150 (void);
static    void factorOf3Transform (void) ;
static    void factorOf5Transform (void) ;
static    void preFOtherTransform (void);
static    void factorOfOtherTransform (void);
static    void pre_sqFactor2NormlOrder (void);
static    void nonSqFactor2NormOrder (void) ;
static    void detPermutCycles (void);
static    void reorderMatrix (void ) ;

static    int f4t_170 (void);
static    int factorTransform (void);
static    int pre_fOf2Trans (void);
static    int factorOf2Transform (void);
static    int factorOf4Transform (void);
static    int  mulByRotationFactor (void );
static    int  post_sqFactor2NormlOrder (void);
static    void  single_sqFactor2NormlOrder (void);
static    int  multi_sqFactor2NormlOrder (void);

/* End Prototypes */

/*note on this code all numbers alone in comment is
 a reference to the corresponding goto in the original fotran code */

int dfftmx ( double* _pdblA , double* _pdblB , int _iNtot, int _iN, int _iNspan,
             int _iIsn, int _iM, int _iKt, double* _pdblWt, double* _pdblCk,
             double* _pdblBt, double* _pdblSk, int* _piNp, int* _piNfac)
{
   int retVal = 0 ;
   a = _pdblA ;
   b = _pdblB ;

   ntot = _iNtot ;
   n    = _iN ;
   nspan= _iNspan ;
   isn  = _iIsn;
   m    = _iM ;
   kt   = _iKt ;
   wt   = _pdblWt ;
   ck   = _pdblCk;
   bt   = _pdblBt;
   sk   = _pdblSk;
   np   = _piNp;
   nfac = _piNfac;

   inc = abs ( isn ) ;
   nt = inc*ntot ;
   ks = inc*nspan;
   rad = atan((double)1);
   c72 = cos (rad/0.6250);
   s72 = sin (rad/0.6250);
   s120= sqrt(0.750);



   preliminaryWork() ;


   while ( retVal == 0 ) retVal = factorTransform ( ) ;

   np[0] = ks ;

   if ( kt != 0) permute_stage1 ( ) ;


   if ( 2*kt + 1 < m ) permute_stage2 ( ) ;

   _pdblA = a ;
   _pdblB = b ;

   return 0 ;
}

/** **************************************
Sous-Fonctions
******************************************/



/* this function only set the value of variable */
static void preliminaryWork (void)
{
   s72 = -s72 ;
   s120= -s120;
   rad = -rad ;

   kspan = ks ;
   nn = nt -inc ;
   jc = ks/n ;

   /* sin , cos values are re-initialized each lim steps */

   klim = 32*jc ;
   i = 0;
   jf = 0 ;

   if ( kt > 0 )  maxf = max ( nfac[kt-1] , nfac[m-kt-1] );
   else 	  maxf = nfac[m-kt-1] ;

}



/*40*/
/* this function is call as many time as dfftbi has determined factor for the size of the input vector
   each time we call a transform function for each kind of factor , we begin by the smallest
   factor are stored in nfac
 */

static int  factorTransform (void)
{

   int retVal = 42;

   dr = 8 * (double)jc/(double)kspan ;
   cd = 2 * sin(0.5*dr*rad)*sin(0.5*dr*rad);
   sd = sin(dr*rad) ;
   kk = 1 ;
   i++ ;




switch ( nfac[i-1] )
   {
      case 2 :
         /*transform for factor of 2 (including rotation factor)*/

         retVal = pre_fOf2Trans() ;
         if ( retVal == 0 )  factorOf2Transform () ;

         break ;

      case 4 :

         kspnn = kspan ;
         kspan = kspan >> 2 ; /*kspan /= 4 */

         retVal = factorOf4Transform () ;
         break ;

      case 3 :

         k = nfac[i-1] ;
         kspnn = kspan ;
         kspan = kspan / k ;

         factorOf3Transform ( ) ;
         break ;

      case 5 :

         k = nfac[i-1] ;
         kspnn = kspan ;
         kspan = kspan / k ;

         factorOf5Transform ( ) ;
         break ;

      default :

         k = nfac[i-1] ;
         kspnn = kspan ;
         kspan = kspan / k ;

         if ( nfac[i-1] != jf) preFOtherTransform ( ) ;

         factorOfOtherTransform ( ) ;
         break ;
    }



   if ( retVal == 42 )
    {
      if ( i !=  m)  retVal = mulByRotationFactor ( ) ;
      else           retVal = 1 ;
    }

   if ( retVal == 1 ) return 1 ; /*goto permute */
   else 	      return 0 ; /*goto factor_transform => once again*/

}

/* permutation for square factor of n */
static void permute_stage1 (void)
{

 int retVal = 1 ;

   pre_sqFactor2NormlOrder () ;

   if ( n == ntot )
      /*permutation for single-variate transform (optional code)*/
      while ( retVal == 1)
        {
         single_sqFactor2NormlOrder () ;
         retVal = post_sqFactor2NormlOrder () ;
        }
   else
      /*permutation for multivariate transform*/
      while ( retVal == 1) retVal = multi_sqFactor2NormlOrder ();

}

static void permute_stage2 (void)
{
	kspnn = np[kt] ;

	/*permutation for square-free facotrs of n */
	nonSqFactor2NormOrder () ;

	/*determine the permutation cycles of length greater than 1*/
	detPermutCycles ();

	j = k3 + 1;
	nt -= kspnn ;
	i = nt - inc + 1 ;
	while ( nt >= 0 )
	{
		reorderMatrix ( ) ;

		j = k3 + 1 ;
		nt -= kspnn ;
		i = nt - inc + 1 ;
         }
}

/*****************************************
Sous-Sous-Fonctions
******************************************/





static int pre_fOf2Trans (void)
{
	kspan /= 2;
	k1 = kspan + 2 ;
	/*50*/
	do{
		do{
			k2 = kk + kspan ;
			ak = a[k2-1] ;
			bk = b[k2-1] ;

			a[k2-1] = a[kk-1] - ak;
			b[k2-1] = b[kk-1] - bk;

			a[kk-1] = a[kk-1] + ak;
			b[kk-1] = b[kk-1] + bk;

			kk = k2 + kspan ;
		}while (kk <= nn);

		kk -= nn ;
	}while (kk <= jc);


	if ( kk > kspan )  return 1 ; /*goto350*/
	else               return 0 ; /*goto60*/


}



static int factorOf2Transform (void)
{
	do /*60*/ {/*while ( kk <= jc*2 )*/
		c1 = 1 - cd ;
		s1 = sd ;
		mm = min( k1/2 , klim);

		do/* do 80 */  {/*while ( kk <= mm || ( kk > mm && kk < k2 ))*/
			do {/*while(kk > k2) */
				do { /*while (  kk < nt )*/
					k2 = kk + kspan;

					ak = a[kk-1] - a[k2-1];
					bk = b[kk-1] - b[k2-1];

					a[kk-1] = a[kk-1] + a[k2-1];
					b[kk-1] = b[kk-1] + b[k2-1];

					a[k2-1] = c1*ak - s1*bk;
					b[k2-1] = s1*ak + c1*bk;

					kk = k2 + kspan;
				}while (  kk < nt );

				k2 = kk - nt;
				c1 = -c1;
				kk = k1 - k2;


			}while (kk > k2);

			kk += jc;

			if ( kk <= mm ) /* 70 */
			{
				ak =  c1 - ( cd*c1+sd*s1) ;
				s1 += (sd*c1-cd*s1) ;
				/*c the following three statements compensate for truncation
				c error.  if rounded arithmetic is used, substitute
				c c1=ak*/
				c1 = 0.5/(ak*ak+s1*s1) + 0.5 ;
				s1 *= c1 ;
				c1 *= ak ;
			}
			else {
				if ( kk < k2 ) /*90*/ {
					s1 = dr*rad*((double)(kk-1)/(double)jc);
					c1 = cos(s1) ;
					s1 = sin(s1) ;
					mm = min(k1/2,mm+klim);
				}
			}

		} while ( kk <= mm || ( kk > mm && kk < k2 ));

		k1 += (inc+inc) ;
		kk = (k1-kspan)/2 + jc;

	} while ( kk <= jc*2 );


	return 0 ; /*goto40*/
}


/* this one is just an optimisation of the factor of 2 transform , we compute more things each turn */

static int factorOf4Transform (void)
{
   int return_value = 0 ;

   /*120*/
   do
      {
         c1 = 1 ;
         s1 = 0 ;

         mm = min ( kspan , klim ) ;

         do
            {
            f4t_150 () ;
            return_value = f4t_170 () ;
            } while (  return_value == 0 );

         kk += ( inc - kspan ) ;
      } while ( kk <= jc ) ;



   if ( kspan == jc )
        return 1 ; /*goto350*/
   else
         return 0 ;/*goto40*/


}

/*this function and the following are just here for conveniance , they just do fourier transformation  for factor of 4
  but as the code was a bit long in factorof4transform , we've created two sub-functions */

static void f4t_150 (void)
{

   do{
      k1 = kk + kspan ;
      k2 = k1 + kspan ;
      k3 = k2 + kspan ;

      akp = a[kk-1] + a[k2-1] ;
      akm = a[kk-1] - a[k2-1] ;

      ajp = a[k1-1] + a[k3-1] ;
      ajm = a[k1-1] - a[k3-1] ;

      a[kk-1] = akp + ajp ;
      ajp = akp - ajp ;

      bkp = b[kk-1] + b[k2-1] ;
      bkm = b[kk-1] - b[k2-1] ;

      bjp = b[k1-1] + b[k3-1] ;
      bjm = b[k1-1] - b[k3-1] ;

      b[kk-1] = bkp + bjp ;
      bjp = bkp - bjp ;

      akp = akm + bjm ;
      akm = akm - bjm ;

      bkp = bkm - ajm ;
      bkm = bkm + ajm ;

      if ( s1 == 0 )/*190*/
         {
         a[k1-1] = akp ;
         a[k2-1] = ajp ;
         a[k3-1] = akm ;

         b[k1-1] = bkp ;
         b[k2-1] = bjp ;
         b[k3-1] = bkm ;


         }
      else /*160*/
         {

         a[k1-1] = akp*c1 - bkp*s1 ;
         a[k2-1] = ajp*c2 - bjp*s2 ;
         a[k3-1] = akm*c3 - bkm*s3 ;

         a[k1-1] = bkp*c1 + akp*s1 ;
         a[k2-1] = bjp*c2 + ajp*s2 ;
         a[k3-1] = bkm*c3 + akm*s3 ;
         }
       kk=k3+kspan;
    }while  ( kk <= nt ) ;


}

static int  f4t_170 (void)
{
   kk += ( jc - nt ) ;

   if ( kk <= mm )
      {
       c2 = c1 - (cd*c1 + sd*s1);
       s1 = s1 + (sd*c1 - cd*s1);

      /*
      the following three statements compensate for truncation
      error.  if rounded arithmetic is used, substitute
      c1=c2
      */

      c1 = 0.5/(c2*c2+s1*s1) + 0.5 ;
      s1 *= c1 ;
      c1 *= c2 ;

      /*140*/

      c2 = c1*c1 - s1*s1 ;
      s2 = c1*s1*2 ;
      c3 = c2*c1 - s2*s1 ;
      s3 = c2*s1 + s2*c1 ;


      return 0 ;

      }
   else
     {
      if ( kk <= kspan )
         {
          s1 = dr*rad * (kk-1)/jc ;
          c1 = cos (s1) ;
          s1 = sin (s1) ;
          mm = min ( kspan , mm  + klim );

         /*140*/

         c2 = c1*c1 - s1*s1 ;
         s2 = c1*s1*2 ;
      	 c3 = c2*c1 - s2*s1 ;
         s3 = c2*s1 + s2*c1 ;

         return 0 ;
         }
      }

   return 1 ;
}




static void factorOf3Transform (void)
{
	do{
		do{
			k1 = kk + kspan ;
			k2 = k1 + kspan ;

			ak = a[kk-1] ;
			bk = b[kk-1] ;

			aj = a[k1-1] + a[k2-1] ;
			bj = b[k1-1] + b[k2-1] ;

			a[kk-1] = ak + aj ;
			b[kk-1] = bk + bj ;

			ak = -0.5*aj + ak ;
			bk = -0.5*bj + bk ;

			aj = (a[k1-1] - a[k2-1])*s120 ;
			bj = (b[k1-1] - b[k2-1])*s120 ;

			a[k1-1] = ak - bj ;
			b[k1-1] = bk + aj ;
			a[k2-1] = ak + bj ;
			b[k2-1] = bk - aj ;

			kk = k2 + kspan ;
		} while (kk < nn);

		kk -= nn ;
	}while (kk <= kspan);

}

static void factorOf5Transform (void)
{
	c2 = c72*c72 - s72 *s72 ;
	s2 = 2 * c72*s72;

	do{
		do{
			k1 = kk + kspan ;
			k2 = k1 + kspan ;
			k3 = k2 + kspan ;
			k4 = k3 + kspan ;



			akp = a[k1-1] + a[k4-1] ;
			akm = a[k1-1] - a[k4-1] ;

			bkp = b[k1-1] + b[k4-1] ;
			bkm = b[k1-1] - b[k4-1] ;

			ajp = a[k2-1] + a[k3-1] ;
			ajm = a[k2-1] - a[k3-1] ;

			bjp = b[k2-1] + b[k3-1] ;
			bjm = b[k2-1] - b[k3-1] ;

			aa = a[kk-1] ;
			bb = b[kk-1] ;

			a[kk-1] = aa + akp + ajp;
			b[kk-1] = bb + bkp + bjp;

			ak = akp*c72 + ajp*c2 + aa ;
			bk = bkp*c72 + bjp*c2 + bb ;

			aj = akm*s72 + ajm*s2 ;
			bj = bkm*s72 + bjm*s2 ;

			a[k1-1] = ak - bj ;
			a[k4-1] = ak + bj ;
			b[k1-1] = bk + aj ;
			b[k4-1] = bk - aj ;

			ak = akp*c2 + ajp*c72 + aa ;
			bk = bkp*c2 + bjp*c72 + bb ;

			aj = akm*s2 - ajm*s72 ;

			bj = bkm*s2 - bjm*s72 ;

			a[k2-1] = ak - bj ;
			a[k3-1] = ak + bj ;
			b[k2-1] = bk + aj ;
			b[k3-1] = bk - aj ;

			kk = k4 + kspan;
		}while (kk < nn);

		kk -= nn ;
	}while (kk <= kspan);
}

/* this function is the general case of non factor of 2 factor , the factorof3transform and factorof5trandform are just
special case of this one */


static void preFOtherTransform (void)
{

   jf = k ;
   s1 = (rad*8)/k ;
   c1 = cos (s1) ;
   s1 = sin (s1) ;
   ck[jf-1] = 1 ;
   sk[jf-1] = 0 ;
   j = 1 ;

   do
   {
      ck[j-1] = ck[k-1] * c1 +  sk[k-1]*s1 ;
      sk[j-1] = ck[k-1] * s1 -  sk[k-1]*c1 ;

      k -- ;

      ck[k-1] = ck[j-1] ;
      sk[k-1] = - sk[j-1] ;

      j++ ;

   }while ( j < k );

}

static void factorOfOtherTransform (void)
{
int ktemp = 0 ;

do
   {
   k1 = kk ;
   k2 = kk + kspnn ;

   aa = a[kk-1] ;
   bb = b[kk-1] ;

   ak = aa ;
   bk = bb ;
   j = 1 ;
   k1 += kspan ;

   do
   {
      k2 -= kspan ;
      j++ ;

      wt[j-1] = a[k1-1] + a[k2-1] ;
      ak = wt[j-1] + ak ;

      bt[j-1] = b[k1-1] + b[k2-1] ;
      bk = bt[j-1] + bk ;
      j++ ;

      wt[j-1] = a[k1-1] - a[k2-1] ;
      bt[j-1] = b[k1-1] - b[k2-1] ;

      k1 += kspan;
   }while ( k1 < k2 ) ;

   a[kk-1] = ak ;
   b[kk-1] = bk ;

   k1 = kk ;
   k2 = kk + kspnn ;
   j = 1 ;

   do
      {
      k1 += kspan ;
      k2 -= kspan ;
      jj = j  ;
      ak = aa ;
      bk  = bb ;
      aj = 0 ;
      bj = 0 ;
      k  = 1 ;

      do
         {
         k++ ;
         ak += ( wt[k-1] * ck[jj-1] ) ;
         bk += ( bt[k-1] * ck[jj-1] ) ;

         k++ ;
         aj += (wt[k-1] * sk[jj-1]) ;
         bj += (bt[k-1] * sk[jj-1]) ;
         jj += j ;

         if ( jj > jf )
            jj -= jf ;
         } while ( k < jf ) ;

      k = jf - j ;
      a[k1-1] = ak - bj ;
      b[k1-1] = bk + aj ;
      a[k2-1] = ak + bj ;
      b[k2-1] = bk - aj ;

      j++ ;

      }while ( j < k ) ;





   kk += kspnn ;
   ktemp = kk ;

   if ( kk > nn )
      {
         kk -= nn;
      }

}while (  ktemp <= nn || (kk <= kspan &&  ( ktemp > nn))  );
}




static int mulByRotationFactor (void )
{
 int ktemp = 0 ;

   if ( i != m )
      {
       kk = jc + 1 ;

       /*300*/
       do
         {
         c2 = 1 - cd ;
         s1 = sd ;

         mm = min ( kspan , klim ) ;

         /*320 */
         do
           {

            c1 = c2 ;
            s2 = s1 ;
            kk += kspan ;

            do
              {

                ak = a[kk-1] ;
                a[kk-1] = c2*ak - s2*b[kk-1] ;
                b[kk-1] = s2*ak + c2*b[kk-1] ;

                kk += kspnn ;
                ktemp = kk ;


                if ( kk > nt )
                  {
                   ak = s1*s2 ;
                   s2 = s1*c2 + s2*c1 ;
                   c2 = c1*c2 - ak ;
                   kk += (kspan - nt ) ;



                  }

               }while (ktemp <= nt || ( kk <= kspnn && ktemp > nt  )) ;

            kk += ( jc - kspnn );


            if ( kk <= mm )
               {

               /* 310*/
                c2 = c1 - ( cd*c1 + sd*s1 ) ;
                s1 += (sd*c1 - cd*s1 ) ;

               /*
               the following three statements compensate for truncation
               error.  if rounded arithmetic is used, substitute
               c1=c2
               */

                c1 = 0.5/(c2*c2+s1*s1) + 0.5 ;
                s1 *= c1 ;
                c2 *= c1 ;
               }
            else
               {
                  if ( kk <= kspan )
                     {

                      s1 = dr*rad * (kk-1)/jc ;
                      c2 = cos (s1) ;
                      s1 = sin (s1) ;
                      mm = min ( kspan , mm  + klim );
                     }
               }

            }while ( kk <= mm  ||( kk <= kspan && kk > mm ) ) ;

          kk += (jc + inc -kspan );


         }while ( kk <= jc+jc);


       return 0 ; /* goto40 */
      }

   return 1 ; /* goto350*/
}




static void pre_sqFactor2NormlOrder (void)
{

   k = kt + kt + 1 ;

   if ( m < k )
      k -- ;

   j = 1 ;
   np[k] = jc ;

   do
      {
        np[j] = np[j-1]/nfac[j-1] ;
        np[k-1] = np[k]*nfac[j-1] ;

        j++ ;
        k-- ;
      }while ( j <  k ) ;

   k3 = np[k] ;
   kspan = np[1] ;
   kk = jc + 1 ;
   k2 = kspan + 1 ;
   j = 1;


}

static int  post_sqFactor2NormlOrder (void)
{

    do
     {
      do
        {
          k2 -= np[j-1] ;
          j++ ;
          k2 += np[j]  ;

         } while ( k2 >  np[j-1]);

       j = 1 ;

/* 390 */
       do
         {
          if ( kk < k2 )
            {


             return 1 ;
            }
          else
            {
               kk += inc ;
               k2 += kspan ;
            }
         }while( k2 < ks  );

      }while ( kk < ks ) ;

   jc = k3 ;

   return 0;
}


/* appeler cetter fonction dans un do while valeur_retour != 1)*/
static void  single_sqFactor2NormlOrder (void)
{


      do
        {

         ak = a[kk-1] ;
         a[kk-1] = a[k2-1] ;
         a[k2-1] = ak ;

         bk = b[kk-1] ;
         b[kk-1] = b[k2-1] ;
         b[k2-1] = bk ;

         kk += inc ;
         k2 += kspan ;
        } while ( k2 < ks );

      /*380*/
}

/*idem que single_ */
static int multi_sqFactor2NormlOrder (void)
{



      k = kk + jc ;

      do /*410*/
        {
         ak = a[kk-1] ;
         a[kk-1] = a[k2-1] ;
         a[k2-1] = ak ;
         bk = b[kk-1] ;
         b[kk-1] = b[k2-1] ;
         b[k2-1] = bk ;
         kk += inc ;
         k2 += kspan ;
        } while ( kk < k );

      kk += (ks - jc ) ;
      k2 += (ks - jc ) ;

      if ( kk < nt )
         return 1 ;

      k2 += (  kspan - nt );
      kk += (  jc    - nt );

      if ( k2 < ks )
         {

         return 1 ;
         }
      if( post_sqFactor2NormlOrder ( ) == 1 )
         {

         return 1 ;
         }
   jc = k3 ;

   return 0;

}



static void nonSqFactor2NormOrder (void)
{

   j = m - kt ;
   nfac[j] = 1 ;



    do
      {
         nfac[j-1] *= nfac[j] ;

         j-- ;


      }while ( j != kt ) ;

   kt ++ ;
   nn = nfac[kt-1] - 1;

   jj = 0 ;
   j = 0;

   /*480*/

   k2 = nfac[kt-1] ;
   k = kt + 1 ;
   kk = nfac[k-1] ;
   j ++ ;

   while ( j <= nn )
         {
            jj += kk ;


            while ( jj >=  k2 )
               {
                jj -= k2 ;
                k2 = kk ;
                k++ ;
                kk = nfac[k-1] ;

                jj += kk ;


               }

            np[j-1] = jj ;
            k2 = nfac[kt-1] ;
            k = kt + 1 ;
            kk = nfac[k-1] ;
            j ++ ;

         }

   j = 0 ;

   return ;
}

/*  here we determine how many permutation cycles we need to do */
static void detPermutCycles (void)
{

  do
   {
    do
      {
       j++ ;

       kk = np[j-1] ;
      }while ( kk < 0 ) ;


    if ( kk != j )
      {
         do
            {

               k = kk ;
               kk = np[k-1] ;
               np[k-1] = -kk ;
            }while ( kk != j ) ;
         k3 = kk ;
      }
    else
       np[j-1] = -j ;
   }while ( j != nn );

   maxf *= inc ;

   return ;
}

static void  reorderMatrix (void)
{
do
  {
   do
      {
       j--  ;

      }while (np[j-1] < 0 ) ;

   jj = jc ;

   /*520*/
   do
      {
         kspan = jj ;

         if ( jj > maxf )
            kspan = maxf ;

         jj -= kspan ;
         k  = np [j-1];
         kk = jc*k + i + jj ;
         k1 = kk + kspan ;
         k2 = 0 ;

         do /*530*/
            {
               k2 ++ ;
               wt[k2-1] = a[k1-1] ;
               bt[k2-1] = b[k1-1] ;
               k1 -= inc ;

            }while ( k1 != kk );

         do
            {
            k1 = kk + kspan ;
            k2 = k1 - jc * (k + np[k-1]);
            k = -np[k-1];


            do
               {
                  a[k1-1] = a[k2-1] ;
                  b[k1-1] = b[k2-1] ;

                  k1 -= inc ;
                  k2 -= inc ;

                  }while ( k1 != kk ) ;

            kk = k2 ;

            }while ( k != j );

            k1 = kk +kspan ;
            k2 = 0 ;
         /*560*/

         do
            {
            k2 ++ ;
            a[k1-1] = wt[k2-1] ;
            b[k1-1] = bt[k2-1] ;
            k1 -= inc ;


            }while (  k1 != kk ) ;

   } while ( jj != 0 ) ;
}while ( j != 1 ) ;

   return ;
}



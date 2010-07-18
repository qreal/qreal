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
#include <stdio.h>
#include "max.h"
#include "ifft_internal.h"

void difftbi ( double* a , double* b , int nseg , int n    , int nspn  ,
              int isn   , int ierr  , int lout , int lnow , int lused ,
              int lmax  , int lbook , double* rstak , int* istak )
{

   int nfac[15] ;
   int i ;
   int in ;
   int j = 3 ;
   int j2 = 3 ;
   int j3 = 3 ;
   int jj = 9;
   int m = 0 ;
   int k ;
   int kt ;
   int kkk ;
   int nspan ;
   int nitems ;
   int ntot ;
   int maxp  = 0;
   int maxf ;
   int itype;
   int istkgt ;
   int isize[] = {1,1,1,2,2} ;

   int nf = abs ( n ) ;

   ierr = 0 ;
  printf ( "debut de dfftbi \n" );
   /*determine the factors of n */


   if ( nf == 1)
      return ;

   k = nf ;

   nspan = abs ( nf*nspn ) ;
   ntot  = abs ( nspan*nseg) ;

   if ( isn*ntot == 0 )
      {
      ierr = 1 ;
      return  ;
      }

printf ("un petit test  kplop %d\n" , k - (int)(k/16)*16 )  ;

   while ( (k- (int)(k/16)*16 ) == 0 )
      {
         m++;
         printf ("m %d ,k %d ,k2 %d\n" , m , k ,(int) (k/16)*16 );
         nfac[m-1] = 4 ;
         k = k >> 4 ;
      }

printf ("avant ploa k %d\n\n" , k );
   do
      {
      while ( k%jj == 0 )
         {
            m++;
            nfac[m-1] = j ;
            k /= jj ;
             printf ("\nm %d ,k %d j %f jj %d\n" , m , k ,j , jj);
         }

         j+=2;
         jj= j*j ;

      }while ( jj <= k);


printf ( "ploa\n" );


  if ( k <= 4)
     {
      kt = m;
      nfac[m+1] = k;
      if ( k != 1 )
         m++;
      }
   else
     {
       if ( (k & 7) != 0 )
         {
            m++;
            nfac[m-1] = 2 ;
            k = k >> 2 ;
          }

   /*all square factor out now but k >= 5 still */
       kt = m ;
       maxp = max ( (kt+1)*2 , k-1);
       j=2;
        printf ( "plob\n" );
      do
        {
         if ( k%j == 0 )
            {
               m++;
               nfac[m-1] = j ;
               k /= j ;
            }

          j = (j+1) | 1 ;

         }while ( j <= k );

      }

   if ( m <= ( kt+1) )
      maxp = m + kt + 1  ;

   if ( m + kt > 15)
    {
      ierr = 2 ;
      printf ( "argh return 5 \n" );
      return ;
    }
   if ( kt != 0 )
      {
         j = kt ;

         do{
            m++;
            nfac[m-1] = nfac[j-1];
            j--;
           }while ( j != 0) ;
      }

    maxf = nfac[m-kt-1] ;

   if ( kt > 0 )
      maxf = max ( nfac[kt-1] , maxf );

   for ( kkk = 1 ; kkk < m ; kkk++ )
      maxf = max ( maxf , nfac[kkk-1]);

 nitems = maxf * 4 ;
 itype = 4 ;

 istkgt = ( lnow*isize[1] -1)/isize[itype-1] + 2;

 i = ( (istkgt - 1 + nitems) * isize[itype-1] -1) / isize[1] + 3 ;
 printf ("i %d ,\n lmax %d\n istkgt %d\n lnow %d \n", i , lmax , istkgt , lnow ) ;



   if ( i > lmax )
      {
         ierr = -i ;
         printf ( "argh return 4 -i %d \n" , -i );
         return ;
      }

   istak[i-2] = itype ;
   istak[i-1] = lnow  ;
   lout ++ ;
   lnow = i ;
   lused = max ( lused , lnow );

   j = istkgt ;
   jj = j + maxf ;
   j2 = jj+ maxf ;
   j3 = j2+ maxf ;

   nitems = maxp ;
   itype  = 2 ;

 istkgt = ( lnow*isize[1] -1)/isize[itype-1] + 2;

 i = ( (istkgt - 1 + nitems) * isize[itype-1] -1) / isize[1] + 3 ;

   if ( i > lmax )
      {
         ierr = -i ;
         printf ( "argh return 4 -i %d \n" , -i );
         return ;
      }

   istak[i-2] = itype ;
   istak[i-1] = lnow  ;
   lout ++ ;
   lnow = i ;
   lused = max ( lused , lnow );

   k = istkgt ;

/*
c     la carte suivante est a supprimer si simple precision
c     next instruction commented by FD&MG (simulog residue?)
c    ********************************************
c      k=2*k-1
c    *********************************************
*/

   printf ( "dfftmx  me voilà tayoooooooo \n" );
   difftmx( a , b , ntot , nf , nspan , isn , m , kt , &rstak[j-1] , &rstak[jj-1] , &rstak[j2-1] , &rstak[j3-1] , &istak[k-1] , nfac);

   k =2 ;

   in = 2 ;

   if (!( lbook <= lnow &&  lnow <= lused && lused <=  lmax ))
      {
         ierr = 3 ;
         printf ( "argh return 6 \n" );
         return ;
      }

   while ( in > 0)
      {
         if ( lbook > istak[lnow-1] || istak[lnow-1] >=  lnow-1)
         {
            ierr = 4 ;
         }

         lout-- ;
         lnow = istak[lnow-1] ;
         in-- ;
      }
      printf ( "fin de dfftbi \n" );
   return ;
}

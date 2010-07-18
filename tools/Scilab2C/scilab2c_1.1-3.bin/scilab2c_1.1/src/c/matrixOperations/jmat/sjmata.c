#include <jmat.h>

void sjmata(float n, float m, float *out){
	int i,nn,mm,j;
	mm=(int) m;
	nn=(int) n;

	
	for (i=0;i<n;i++){/*We work by blocs*/
		for(j=0;j<m;j++){/*then by column*/
			/*
			 * i*mm*nn : advance to the good column
			 * mm*(nn-(i+1)) : take the first element of the column to be 1, when j=0
			 * j*(mm*nn+1) : advance to the next to be 1
			 * */
			out[i*mm*mm*nn+(mm*(nn-(i+1))+j*(mm*nn+1))]=1;
		}
	}
	
}



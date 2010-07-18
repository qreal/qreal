/*
**  -*- C -*-
**
** 
** Made by  Raffaele.Nutricato@tiscali.it
**
** Copyright Raffaele Nutricato
*/

/*
	Modify by Arnaud Torset : 20/02/09
*/

#ifndef __OPINS_H__
#define __OPINS_H__


/* a(3)=2 */
#define s2s0s0OpIns(in,size,indice,newVal)	in[(int) indice-1]=newVal;

#define d2d0d0OpIns(in,size,indice,newVal)	in[(int) indice-1]=newVal;

#define c2s0c0OpIns(in,size,indice,newVal)	in[(int) indice-1]=newVal;

#define z2d0z0OpIns(in,size,indice,newVal)	in[(int) indice-1]=newVal;

/* a([1 2])=2 */
#define s2s2s0OpIns(in,size1,indices,size2,newVal)	{int i;\
									for (i=0;i<size2[0]*size2[1];i++)	in[(int)indices[i]-1]=newVal;\
									}


#define d2d2d0OpIns(in,size1,indices,size2,newVal)	{int i;\
									for (i=0;i<size2[0]*size2[1];i++)	in[(int)indices[i]-1]=newVal;\
									}

#define c2s2c0OpIns(in,size1,indices,size2,newVal)	{int i;\
									for (i=0;i<size2[0]*size2[1];i++)	in[(int)indices[i]-1]=newVal;\
									}

#define z2d2z0OpIns(in,size1,indices,size2,newVal)	{int i;\
									for (i=0;i<size2[0]*size2[1];i++)	in[(int)indices[i]-1]=newVal;\
									}
/* a([1 2])=[3,1] */								
#define s2s2s2OpIns(in,size1,indices,size2,newVals,size3)	{int i;\
										for (i=0;i<size2[0]*size3[1];i++)	in[(int)indices[i]-1]=newVals[i];\
										}


#define d2d2d2OpIns(in,size1,indices,size2,newVals,size3)	{int i;\
										for (i=0;i<size2[0]*size3[1];i++)	in[(int)indices[i]-1]=newVals[i];\
										}

#define c2s2c2OpIns(in,size1,indices,size2,newVals,size3)	{int i;\
										for (i=0;i<size2[0]*size3[1];i++)	in[(int)indices[i]-1]=newVals[i];\
										}

#define z2d2z2OpIns(in,size1,indices,size2,newVals,size3)	{int i;\
										for (i=0;i<size2[0]*size3[1];i++)	in[(int)indices[i]-1]=newVals[i];\
										}
									

/* a(1,3)=2 */	
#define s2s0s0s0OpIns(in,size,row,col,newVal)	in[(int)(col-1)*size[0]+(int)row-1]=newVal;

#define d2d0d0d0OpIns(in,size,row,col,newVal)	in[(int)(col-1)*size[0]+(int)row-1]=newVal;

#define c2s0s0c0OpIns(in,size,row,col,newVal)	in[(int)(col-1)*size[0]+(int)row-1]=newVal;

#define z2d0d0z0OpIns(in,size,row,col,newVal)	in[(int)(col-1)*size[0]+(int)row-1]=newVal;

	
/* a(1,[3,1])=2 */	
#define s2s0s2s0OpIns(in,size1,row,cols,size2,newVal)	{int i;\
									for (i=0;i<size2[0]*size2[1];i++) 	in[(int)(cols[i]-1)*size1[0]+(int)row-1]=newVal;\
									}

#define d2d0d2d0OpIns(in,size1,row,cols,size2,newVal)	{int i;\
									for (i=0;i<size2[0]*size2[1];i++) 	in[(int)(cols[i]-1)*size1[0]+(int)row-1]=newVal;\
									}

#define c2s0s2c0OpIns(in,size1,row,cols,size2,newVal)	{int i;\
									for (i=0;i<size2[0]*size2[1];i++) 	in[(int)(cols[i]-1)*size1[0]+(int)row-1]=newVal;\
									}

#define z2d0d2z0OpIns(in,size1,row,cols,size2,newVal)	{int i;\
									for (i=0;i<size2[0]*size2[1];i++) 	in[(int)(cols[i]-1)*size1[0]+(int)row-1]=newVal;\
									}						

	
/* a([3,1],1)=2 */	
#define s2s2s0s0OpIns(in,size1,rows,size2,col,newVal)	{int i;\
									for (i=0;i<size2[0]*size2[1];i++) 	in[(int)(col-1)*size1[0]+(int)rows[i]-1]=newVal;\
									}

#define d2d2d0d0OpIns(in,size1,rows,size2,col,newVal)	{int i;\
									for (i=0;i<size2[0]*size2[1];i++) 	in[(int)(col-1)*size1[0]+(int)rows[i]-1]=newVal;\
									}

#define c2s2s0c0OpIns(in,size1,rows,size2,col,newVal)	{int i;\
									for (i=0;i<size2[0]*size2[1];i++) 	in[(int)(col-1)*size1[0]+(int)rows[i]-1]=newVal;\
									}

#define z2d2d0z0OpIns(in,size1,rows,size2,col,newVal)	{int i;\
									for (i=0;i<size2[0]*size2[1];i++) 	in[(int)(col-1)*size1[0]+(int)rows[i]-1]=newVal;\
									}
									
/* a([3,1],[1 2])=2 */
#define s2s2s2s0OpIns(in,size1,rows,size2,cols,size3,newVal)	{int i,j;\
											for (i=0;i<size3[0]*size3[1];i++)\
												for (j=0;j<size2[0]*size2[1];j++)\
													in[(int)(cols[i]-1)*size1[0]+(int)rows[j]-1]=newVal;\
											}

#define d2d2d2d0OpIns(in,size1,rows,size2,cols,size3,newVal)	{int i,j;\
											for (i=0;i<size3[0]*size3[1];i++)\
												for (j=0;j<size2[0]*size2[1];j++)\
													in[(int)(cols[i]-1)*size1[0]+(int)rows[j]-1]=newVal;\
											}

#define c2s2s2c0OpIns(in,size1,rows,size2,cols,size3,newVal)	{int i,j;\
											for (i=0;i<size3[0]*size3[1];i++)\
												for (j=0;j<size2[0]*size2[1];j++)\
													in[(int)(cols[i]-1)*size1[0]+(int)rows[j]-1]=newVal;\
											}

#define z2d2d2z0OpIns(in,size1,rows,size2,cols,size3,newVal)	{int i,j;\
											for (i=0;i<size3[0]*size3[1];i++)\
												for (j=0;j<size2[0]*size2[1];j++)\
													in[(int)(cols[i]-1)*size1[0]+(int)rows[j]-1]=newVal;\
											}
/* a(1,[3,1])=[2,5] */	
#define s2s0s2s2OpIns(in,size1,row,cols,size2,newVals,size3)	{int i;\
											for (i=0;i<size2[0]*size2[1];i++)\
											 	in[(int)(cols[i]-1)*size1[0]+(int)row-1]=newVals[i];\
											}

#define d2d0d2d2OpIns(in,size1,row,cols,size2,newVals,size3)	{int i;\
											for (i=0;i<size2[0]*size2[1];i++)\
											 	in[(int)(cols[i]-1)*size1[0]+(int)row-1]=newVals[i];\
											}

#define c2s0s2c2OpIns(in,size1,row,cols,size2,newVals,size3)	{int i;\
											for (i=0;i<size2[0]*size2[1];i++)\
											 	in[(int)(cols[i]-1)*size1[0]+(int)row-1]=newVals[i];\
											}

#define z2d0d2z2OpIns(in,size1,row,cols,size2,newVals,size3)	{int i;\
											for (i=0;i<size2[0]*size2[1];i++)\
											 	in[(int)(cols[i]-1)*size1[0]+(int)row-1]=newVals[i];\
											}	
/* a([3,1],1)=[2,5] */	
#define s2s2s0s2OpIns(in,size1,rows,size2,col,newVals,size3)	{int i;\
											for (i=0;i<size2[0]*size2[1];i++)\
												in[(int)(col-1)*size1[0]+(int)rows[i]-1]=newVals[i];\
											}

#define d2d2d0d2OpIns(in,size1,rows,size2,col,newVals,size3)	{int i;\
											for (i=0;i<size2[0]*size2[1];i++)\
												in[(int)(col-1)*size1[0]+(int)rows[i]-1]=newVals[i];\
											}

#define c2s2s0c2OpIns(in,size1,rows,size2,col,newVals,size3)	{int i;\
											for (i=0;i<size2[0]*size2[1];i++)\
												in[(int)(col-1)*size1[0]+(int)rows[i]-1]=newVals[i];\
											}

#define z2d2d0z2OpIns(in,size1,rows,size2,col,newVals,size3)	{int i;\
											for (i=0;i<size2[0]*size2[1];i++)\
												in[(int)(col-1)*size1[0]+(int)rows[i]-1]=newVals[i];\
											}

/* a([3,1],[1 2])=[2,1,3,5] */
#define s2s2s2s2OpIns(in,size1,rows,size2,cols,size3,newVals,size4)	{int i,j;\
												for (i=0;i<size3[0]*size3[1];i++)\
													for (j=0;j<size2[0]*size2[1];j++)\
														in[(int)(cols[i]-1)*size1[0]+(int)rows[j]-1]=newVals[i*size4[0]+j];\
												}

#define d2d2d2d2OpIns(in,size1,rows,size2,cols,size3,newVals,size4)	{int i,j;\
												for (i=0;i<size3[0]*size3[1];i++)\
													for (j=0;j<size2[0]*size2[1];j++)\
														in[(int)(cols[i]-1)*size1[0]+(int)rows[j]-1]=newVals[i*size4[0]+j];\
												}

#define c2s2s2c2OpIns(in,size1,rows,size2,cols,size3,newVals,size4)	{int i,j;\
												for (i=0;i<size3[0]*size3[1];i++)\
													for (j=0;j<size2[0]*size2[1];j++)\
														in[(int)(cols[i]-1)*size1[0]+(int)rows[j]-1]=newVals[i*size4[0]+j];\
												}

#define z2d2d2z2OpIns(in,size1,rows,size2,cols,size3,newVals,size4)	{int i,j;\
												for (i=0;i<size3[0]*size3[1];i++)\
													for (j=0;j<size2[0]*size2[1];j++)\
														in[(int)(cols[i]-1)*size1[0]+(int)rows[j]-1]=newVals[i*size4[0]+j];\
												}
												
												
/* Mixed types 
2 cases : we assign a real in a complex array => ok, we just have put the imaginary part to 0
	    we assign a complex in a real array => ko, we can't enlarge the input matrix. So we just replace by the real part of the complex
*/

/* a(3)=2 */
#define s2s0c0OpIns(in,size,indice,newVal)	s2s0s0OpIns(in,size,indice,creals(newVal))

#define d2d0z0OpIns(in,size,indice,newVal)	d2d0d0OpIns(in,size,indice,zreals(newVal))

#define c2s0s0OpIns(in,size,indice,newVal)	c2s0c0OpIns(in,size,indice,FloatComplex(newVal,0))

#define z2d0d0OpIns(in,size,indice,newVal)	z2d0z0OpIns(in,size,indice,DoubleComplex(newVal,0))


/* a([1 2])=2 */
#define s2s2c0OpIns(in,size1,indices,size2,newVal)	s2s2s0OpIns(in,size1,indices,size2,creals(newVal))

#define d2d2z0OpIns(in,size1,indices,size2,newVal)	d2d2d0OpIns(in,size1,indices,size2,zreals(newVal))

#define c2s2s0OpIns(in,size1,indices,size2,newVal)	c2s2c0OpIns(in,size1,indices,size2,FloatComplex(newVal,0))

#define z2d2d0OpIns(in,size1,indices,size2,newVal)	z2d2z0OpIns(in,size1,indices,size2,DoubleComplex(newVal,0))

	
/* a([1 2])=[3,1] */								
#define s2s2c2OpIns(in,size1,indices,size2,newVals,size3)	{int i;\
										for (i=0;i<size2[0]*size3[1];i++)	in[(int)indices[i]-1]=creals(newVals[i]);\
										}


#define d2d2z2OpIns(in,size1,indices,size2,newVals,size3)	{int i;\
										for (i=0;i<size2[0]*size3[1];i++)	in[(int)indices[i]-1]=zreals(newVals[i]);\
										}

#define c2s2s2OpIns(in,size1,indices,size2,newVals,size3)	{int i;\
										for (i=0;i<size2[0]*size3[1];i++)	in[(int)indices[i]-1]=FloatComplex(newVals[i],0);\
										}

#define z2d2d2OpIns(in,size1,indices,size2,newVals,size3)	{int i;\
										for (i=0;i<size2[0]*size3[1];i++)	in[(int)indices[i]-1]=DoubleComplex(newVals[i],0);\
										}
									

/* a(1,3)=2 */	
#define s2s0s0c0OpIns(in,size,row,col,newVal)	s2s0s0s0OpIns(in,size,row,col,creals(newVal))

#define d2d0d0z0OpIns(in,size,row,col,newVal)	d2d0d0d0OpIns(in,size,row,col,zreals(newVal))

#define c2s0s0s0OpIns(in,size,row,col,newVal)	c2s0s0c0OpIns(in,size,row,col,FloatComplex(newVal,0))

#define z2d0d0d0OpIns(in,size,row,col,newVal)	z2d0d0z0OpIns(in,size,row,col,DoubleComplex(newVal,0))

	
/* a(1,[3,1])=2 */	
#define s2s0s2c0OpIns(in,size1,row,cols,size2,newVal)	s2s0s2s0OpIns(in,size1,row,cols,size2,creals(newVal))

#define d2d0d2z0OpIns(in,size1,row,cols,size2,newVal)	d2d0d2d0OpIns(in,size1,row,cols,size2,zreals(newVal))

#define c2s0s2s0OpIns(in,size1,row,cols,size2,newVal)	c2s0s2c0OpIns(in,size1,row,cols,size2,FloatComplex(newVal,0))

#define z2d0d2d0OpIns(in,size1,row,cols,size2,newVal)	z2d0d2z0OpIns(in,size1,row,cols,size2,DoubleComplex(newVal,0))				

	
/* a([3,1],1)=2 */	
#define s2s2s0c0OpIns(in,size1,rows,size2,col,newVal)	s2s2s0s0OpIns(in,size1,rows,size2,col,creals(newVal))

#define d2d2d0z0OpIns(in,size1,rows,size2,col,newVal)	d2d2d0d0OpIns(in,size1,rows,size2,col,zreals(newVal))

#define c2s2s0s0OpIns(in,size1,rows,size2,col,newVal)	c2s2s0c0OpIns(in,size1,rows,size2,col,FloatComplex(newVal,0))

#define z2d2d0d0OpIns(in,size1,rows,size2,col,newVal)	z2d2d0z0OpIns(in,size1,rows,size2,col,DoubleComplex(newVal,0))	
									
/* a([3,1],[1 2])=2 */
#define s2s2s2c0OpIns(in,size1,rows,size2,cols,size3,newVal)	s2s2s2s0OpIns(in,size1,rows,size2,cols,size3,creals(newVal))

#define d2d2d2z0OpIns(in,size1,rows,size2,cols,size3,newVal)	d2d2d2d0OpIns(in,size1,rows,size2,cols,size3,zreals(newVal))

#define c2s2s2s0OpIns(in,size1,rows,size2,cols,size3,newVal)	c2s2s2c0OpIns(in,size1,rows,size2,cols,size3,FloatComplex(newVal,0))

#define z2d2d2d0OpIns(in,size1,rows,size2,cols,size3,newVal)	z2d2d2z0OpIns(in,size1,rows,size2,cols,size3,DoubleComplex(newVal,0))	


/* a(1,[3,1])=[2,5] */	
#define s2s0s2c2OpIns(in,size1,row,cols,size2,newVals,size3)	{int i;\
											for (i=0;i<size2[0]*size2[1];i++)\
											 	in[(int)(cols[i]-1)*size1[0]+(int)row-1]=creals(newVals[i]);\
											}

#define d2d0d2z2OpIns(in,size1,row,cols,size2,newVals,size3)	{int i;\
											for (i=0;i<size2[0]*size2[1];i++)\
											 	in[(int)(cols[i]-1)*size1[0]+(int)row-1]=zreals(newVals[i]);\
											}

#define c2s0s2s2OpIns(in,size1,row,cols,size2,newVals,size3)	{int i;\
											for (i=0;i<size2[0]*size2[1];i++)\
											 	in[(int)(cols[i]-1)*size1[0]+(int)row-1]=FloatComplex(newVals[i],0);\
											}

#define z2d0d2d2OpIns(in,size1,row,cols,size2,newVals,size3)	{int i;\
											for (i=0;i<size2[0]*size2[1];i++)\
											 	in[(int)(cols[i]-1)*size1[0]+(int)row-1]=DoubleComplex(newVals[i],0);\
											}	
/* a([3,1],1)=[2,5] */	
#define s2s2s0c2OpIns(in,size1,rows,size2,col,newVals,size3)	{int i;\
											for (i=0;i<size2[0]*size2[1];i++)\
												in[(int)(col-1)*size1[0]+(int)rows[i]-1]=creals(newVals[i]);\
											}

#define d2d2d0z2OpIns(in,size1,rows,size2,col,newVals,size3)	{int i;\
											for (i=0;i<size2[0]*size2[1];i++)\
												in[(int)(col-1)*size1[0]+(int)rows[i]-1]=zreals(newVals[i]);\
											}

#define c2s2s0s2OpIns(in,size1,rows,size2,col,newVals,size3)	{int i;\
											for (i=0;i<size2[0]*size2[1];i++)\
												in[(int)(col-1)*size1[0]+(int)rows[i]-1]=FloatComplex(newVals[i],0);\
											}

#define z2d2d0d2OpIns(in,size1,rows,size2,col,newVals,size3)	{int i;\
											for (i=0;i<size2[0]*size2[1];i++)\
												in[(int)(col-1)*size1[0]+(int)rows[i]-1]=DoubleComplex(newVals[i],0);\
											}

/* a([3,1],[1 2])=[2,1,3,5] */
#define s2s2s2c2OpIns(in,size1,rows,size2,cols,size3,newVals,size4)\
									{int i,j;\
									for (i=0;i<size3[0]*size3[1];i++)\
										for (j=0;j<size2[0]*size2[1];j++)\
											in[(int)(cols[i]-1)*size1[0]+(int)rows[j]-1]=creals(newVals[i*size4[0]+j]);\
									}

#define d2d2d2z2OpIns(in,size1,rows,size2,cols,size3,newVals,size4)\
									{int i,j;\
									for (i=0;i<size3[0]*size3[1];i++)\
										for (j=0;j<size2[0]*size2[1];j++)\
											in[(int)(cols[i]-1)*size1[0]+(int)rows[j]-1]=zreals(newVals[i*size4[0]+j]);\
									}

#define c2s2s2s2OpIns(in,size1,rows,size2,cols,size3,newVals,size4)\
									{int i,j;\
									for (i=0;i<size3[0]*size3[1];i++)\
										for (j=0;j<size2[0]*size2[1];j++)\
											in[(int)(cols[i]-1)*size1[0]+(int)rows[j]-1]=FloatComplex(newVals[i*size4[0]+j],0);\
									}

#define z2d2d2d2OpIns(in,size1,rows,size2,cols,size3,newVals,size4)\
									{int i,j;\
									for (i=0;i<size3[0]*size3[1];i++)\
										for (j=0;j<size2[0]*size2[1];j++)\
											in[(int)(cols[i]-1)*size1[0]+(int)rows[j]-1]=DoubleComplex(newVals[i*size4[0]+j],0);\
									}
																
#endif /* !__OPINS_H__ */

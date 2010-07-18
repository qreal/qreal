/*
** ----------------------
** --- Class OPEQUAL. ---
** ----------------------
*/
/* --- Equal. --- */
#define s0OpEquals0(in) \
sOpEquals1(in);

#define s2OpEquals2(inptr,insizeptr,outptr) \
sOpEquala1(inptr, insizeptr[0]*insizeptr[1], outptr);

#define d0OpEquald0(in) \
dOpEquals1(in);

#define d2OpEquald2(inptr,insizeptr,outptr) \
dOpEquala1(inptr, insizeptr[0]*insizeptr[1], outptr);

/*
// ---------------------
// --- Class OPSTAR. ---
// ---------------------
*/
/* --- OpStar. --- 
#define s0s0OpStars0(in1,in2) \
ssOpStarss1(in1,in2);

#define s0s2OpStars2(in1,inptr2,insizeptr2,outptr) \
ssOpStarsa1(in1,inptr2, insizeptr2[0]*insizeptr2[1], outptr);

#define s2s0OpStars2(inptr2,insizeptr2,in1,outptr) \
ssOpStarsa1(in1,inptr2, insizeptr2[0]*insizeptr2[1], outptr);*/
/*questa su e' una macro 

#define s2s2OpStars2(inptr1,insizeptr1,inptr2,insizeptr2,outptr) \
ssOpStaraa1(inptr1,insizeptr1[0],insizeptr1[1], inptr2, insizeptr2[1], outptr);

#define d0d0OpStard0(in1,in2) \
ddOpStarss1(in1,in2);

#define d0d2OpStard2(in1,inptr2,insizeptr2,outptr) \
ddOpStarsa1(in1,inptr2, (insizeptr2[0]) * (insizeptr2[1]), outptr)

#define d2d0OpStard2(inptr2,insizeptr2,in1,outptr) \
ddOpStarsa1(in1,inptr2, (insizeptr2[0]) * (insizeptr2[1]), outptr)*/
/*questa su e' una macro 

#define d2d2OpStard2(inptr1,insizeptr1,inptr2,insizeptr2,outptr) \
ddOpStaraa1(inptr1,insizeptr1[0],insizeptr1[1], inptr2, insizeptr2[1], outptr);*/

/*
// ---------------------
// --- Class OPPLUS. ---
// ---------------------
*/
/* --- OpPlus. ---*/
/*RN volendo puoi fare una sola macro del tipo sa1() che serve per tutte le operazioni
#define s0s0OpPluss0(in1,in2) \
ssOpPlusss1(in1,in2);

#define s0s2OpPluss2(in1,inptr2,insizeptr2,outptr) \
ssOpPlussa1(in1,inptr2, insizeptr2[0]*insizeptr2[1], outptr);

#define s2s2OpPluss2(inptr1,insizeptr1,inptr2,insizeptr2,outptr) \
ssOpPlusaa1(inptr1,inptr2, insizeptr1[0]*insizeptr1[1], outptr)

#define s2s0OpPluss2(inptr2,insizeptr2,in1,outptr) \
ssOpPlussa1(in1,inptr2, insizeptr2[0]*insizeptr2[1], outptr);*/
/*questa su e' una macro 

#define d0d0OpPlusd0(in1,in2) \
ddOpPlusss1(in1,in2);

#define d2d2OpPlusd2(inptr1,insizeptr1,inptr2,insizeptr2,outptr) \
ddOpPlusaa1(inptr1,inptr2, insizeptr1[0]*insizeptr1[1], outptr);

#define d0d2OpPlusd2(in1,inptr2,insizeptr2,outptr) \
ddOpPlussa1(in1,inptr2, (insizeptr2[0]) * (insizeptr2[1]), outptr)

#define d2d0OpPlusd2(inptr2,insizeptr2,in1,outptr) \
ddOpPlussa1(in1,inptr2, (insizeptr2[0]) * (insizeptr2[1]), outptr)*/
/*questa su e' una macro */


/* --- OpDotStar. ---*/
/*#define s0s0OpDotStars0(in1,in2) \
ssOpDotStarss1(in1,in2);

#define s0s2OpDotStars2(in1,inptr2,insizeptr2,outptr) \
ssOpDotStarsa1(in1,inptr2, insizeptr2[0]*insizeptr2[1], outptr);

#define s2s2OpDotStars2(inptr1,insizeptr1,inptr2,insizeptr2,outptr) \
ssOpDotStaraa1(inptr1,inptr2, insizeptr1[0]*insizeptr1[1], outptr)

#define s2s0OpDotStars2(inptr2,insizeptr2,in1,outptr) \
ssOpDotStarsa1(in1,inptr2, insizeptr2[0]*insizeptr2[1], outptr);
questa su e' una macro */

/*#define d0d0OpDotStard0(in1,in2) \
ddOpDotStarss1(in1,in2);

#define d2d2OpDotStard2(inptr1,insizeptr1,inptr2,insizeptr2,outptr) \
ddOpDotStaraa1(inptr1,inptr2, insizeptr1[0]*insizeptr1[1], outptr);

#define d0d2OpDotStard2(in1,inptr2,insizeptr2,outptr) \
ddOpDotStarsa1(in1,inptr2, (insizeptr2[0]) * (insizeptr2[1]), outptr)

#define d2d0OpDotStard2(inptr2,insizeptr2,in1,outptr) \
ddOpDotStarsa1(in1,inptr2, (insizeptr2[0]) * (insizeptr2[1]), outptr)
questa su e' una macro */

/*
// ---------------------
// --- Class ^. ---
// ---------------------
*/
/*RN DA FARE ANCORA forse la si puo' integrare dentro le operazioni OPPLUS*/

/*
// ---------------------
// --- Class OPAPEX. ---
// ---------------------
*/
/* --- OpApex. ---
#define s0OpApexs0(in) \
sOpApexs(in);

#define d0OpApexd0(in) \
dOpApexs(in);

#define s2OpApexs2(inptr,insizeptr,outptr) \
sOpApexa(inptr, insizeptr[0],insizeptr[1], outptr);

#define d2OpApexd2(inptr,insizeptr,outptr) \
dOpApexa(inptr, insizeptr[0],insizeptr[1], outptr);
*/
/*
// ---------------------
// --- Class SIN. ---
// ---------------------
*/
/* --- sin. ---*//*
#define s0sins0(inptr) \
ssins(inptr);

#define d0sind0(inptr) \
dsins(inptr);

#define c0sinc0(inptr) \
csins(inptr);

#define z0sinz0(inptr) \
zsins(inptr);

#define s2sins2(inptr,insizeptr,outptr) \
ssina(inptr, insizeptr[0]*insizeptr[1], outptr);

#define d2sind2(inptr,insizeptr,outptr) \
dsina(inptr, insizeptr[0]*insizeptr[1], outptr);

#define c2sinc2(inptr,insizeptr,outptr) \
csina(inptr, insizeptr[0]*insizeptr[1], outptr);

#define z2sinz2(inptr,insizeptr,outptr) \
zsina(inptr, insizeptr[0]*insizeptr[1], outptr);
*/
/* --- cos. ---*//*
#define s0coss0(inptr) \
scoss(inptr);

#define d0cosd0(inptr) \
dcoss(inptr);

#define c0cosc0(inptr) \
ccoss(inptr);

#define z0cosz0(inptr) \
zcoss(inptr);

#define s2coss2(inptr,insizeptr,outptr) \
scosa(inptr, insizeptr[0]*insizeptr[1], outptr);

#define d2cosd2(inptr,insizeptr,outptr) \
dcosa(inptr, insizeptr[0]*insizeptr[1], outptr);

#define c2cosc2(inptr,insizeptr,outptr) \
ccosa(inptr, insizeptr[0]*insizeptr[1], outptr);

#define z2cosz2(inptr,insizeptr,outptr) \
zcosa(inptr, insizeptr[0]*insizeptr[1], outptr);
*/
/* --- sinh. ---*//*
#define s0sinhs0(inptr) \
ssinhs(inptr);

#define d0sinhd0(inptr) \
dsinhs(inptr);

#define c0sinhc0(inptr) \
csinhs(inptr);

#define z0sinhz0(inptr) \
zsins(inptr);

#define s2sinhs2(inptr,insizeptr,outptr) \
ssinha(inptr, insizeptr[0]*insizeptr[1], outptr);

#define d2sinhd2(inptr,insizeptr,outptr) \
dsinha(inptr, insizeptr[0]*insizeptr[1], outptr);

#define c2sinhc2(inptr,insizeptr,outptr) \
csinha(inptr, insizeptr[0]*insizeptr[1], outptr);

#define z2sinhz2(inptr,insizeptr,outptr) \
zsinha(inptr, insizeptr[0]*insizeptr[1], outptr);

*/
/*
// ---------------------
// --- Class DISP. ---
// ---------------------
*//*
#define s0dispd0(invar) \
sdisps2 (invar, #invar );

#define d0dispd0(invar) \
ddisps2 (invar, #invar );

#define c0dispd0(invar) \
cdisps2 (invar, #invar );

#define z0dispd0(invar) \
zdisps2 (invar, #invar );

#define s2dispd0(invar,insize) \
sdispa2 (invar, insize, #invar );

#define d2dispd0(invar,insize) \
ddispa2 (invar, insize, #invar );

#define c2dispd0(invar,insize) \
cdispa2 (invar, insize, #invar );

#define z2dispd0(invar,insize) \
zdispa2 (invar, insize, #invar );

#define i2dispd0(invar,insize) \
idispa2 (invar, insize, #invar );*/
/*
// --------------------
// --- Class ZEROS. ---
// --------------------
*/
/* --- ones. ---*//*
#define d0d0onesd2(inptr1,inptr2,outptr) \
ddonesss1(inptr1, inptr2, outptr);

#define onesd0() \
1

#define d0onesd0(in1) \
1

#define d2onesd2(inptr,insizeptr,outptr) \
ddonesss1(insizeptr[0], insizeptr[1], outptr);

#define d0d0onesd2(inptr1,inptr2,outptr) \
ddonesss1(inptr1, inptr2, outptr);

#define s0s0oness2(inptr1,inptr2,outptr) \
ssonesss1(inptr1, inptr2, outptr);

#define oness0() \
1

#define s0oness0(in1) \
1

#define s2oness2(inptr,insizeptr,outptr) \
ssonesss1(insizeptr[0], insizeptr[1], outptr);
*/

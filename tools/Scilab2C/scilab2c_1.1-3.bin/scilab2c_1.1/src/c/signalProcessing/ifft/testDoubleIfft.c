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
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "ifft.h"


#define ROW    1
#define COLS2  2
#define COLS3  3
#define COLS4  4
#define COLS5  5
#define COLS6  6
#define COLS7  7
#define COLS8  8
#define COLS9  9
#define COLS10 10
#define COLS11 11
#define COLS12 12
#define COLS13 13
#define COLS14 14
#define COLS15 15
#define COLS16 16
#define COLS32 32

#define ZREAL_IN2 { 0.00022113462910056  ,  0.33032709173858166  }
#define ZIMAG_IN2 { 0.66538110421970487  ,  0.62839178834110498  }

#define ZREAL_IN3 { 2.48206677380949259,  0.43537130765616894, 0.97385666053742170}
#define ZIMAG_IN3 { 2.14807060454040766,- 0.78285905346274376, 0.42632796149700880}


#define ZREAL_IN4 { 0.84974523587152362,  0.68573101982474327, 0.87821648130193353, 0.06837403681129217}
#define ZIMAG_IN4 { 0.56084860628470778,  0.66235693730413914, 0.72635067673400044, 0.19851438421756029}

#define ZREAL_IN5 { 0.84974523587152362,  0.68573101982474327, 0.87821648130193353, 0.06837403681129217,\
                    0.65251349471509457}
#define ZIMAG_IN5 { 0.56084860628470778,  0.66235693730413914, 0.72635067673400044, 0.19851438421756029,\
                    0.56642488157376647}

#define ZREAL_IN6 {  0.54425731627270579, 0.23207478970289230, 0.23122371966019273, 0.21646326314657927,\
                     0.65251349471509457, 0.30760907428339124 }
#define ZIMAG_IN6 {  0.21460078610107303, 0.31264199689030647, 0.36163610080257058, 0.2922266637906432,\
                     0.56642488157376647, 0.59350947011262178 }

#define ZREAL_IN7 {  0.54425731627270579, 0.23207478970289230, 0.23122371966019273, 0.21646326314657927,\
                     0.65251349471509457, 0.88338878145441413, 0.30760907428339124 }
#define ZIMAG_IN7 {  0.21460078610107303, 0.31264199689030647, 0.36163610080257058, 0.2922266637906432,\
                     0.40948254754766822, 0.56642488157376647, 0.59350947011262178 }



#define ZREAL_IN8 {  0.54425731627270579, 0.23207478970289230, 0.23122371966019273, 0.21646326314657927,\
                     0.88338878145441413, 0.65251349471509457, 0.30760907428339124, 0.93296162132173777 }
#define ZIMAG_IN8 {  0.21460078610107303, 0.31264199689030647, 0.36163610080257058, 0.2922266637906432,\
                     0.56642488157376647, 0.48264719732105732, 0.33217189135029912, 0.59350947011262178}


#define ZREAL_IN9 { 0.23122371966019273, 0.21646326314657927, 0.88338878145441413, 0.65251349471509457,\
                    0.30760907428339124, 0.93296162132173777, 0.21460078610107303, 0.31264199689030647,\
                    0.43685875833034515}
#define ZIMAG_IN9 { 0.26931248093023896, 0.63257448654621840, 0.40519540151581168, 0.91847078315913677,\
                    0.28064980218186975, 0.12800584640353918, 0.77831285959109664, 0.21190304495394230,\
                    0.68568959552794695}


#define ZREAL_IN10 { 0.21646326314657927, 0.65251349471509457, 0.63257448654621840, 0.31264199689030647,\
                     0.93296162132173777, 0.31264199689030647, 0.48185089323669672, 0.48264719732105732,\
                     0.2922266637906432 , 0.48264719732105732 }
#define ZIMAG_IN10 { 0.23122371966019273, 0.21646326314657927, 0.88338878145441413, 0.65251349471509457,\
                     0.11383596854284406, 0.19983377400785685, 0.56186607433483005, 0.58961773291230202,\
                     0.23122371966019273, 0.21646326314657927}
/*
#define ZREAL_IN10 { 1,2,3,4,5,6,7,8,9,10 }


#define ZIMAG_IN10 { 1,2,3,4,5,6,7,8,9,10 }
*/




#define ZREAL_IN11 {0.49934938363730907,0.26385784195736051,0.52535630855709314,0.53762298030778766,\
                    0.11999255046248436,0.2256303490139544 ,0.62740930821746588,0.76084325974807143,\
                    0.04855662025511265,0.67239497276023030,0.20171726960688829 }

#define ZIMAG_IN11 {0.39115739194676280,0.83003165572881699,0.58787201577797532,0.48291792999953032,\
                    0.22328650346025825,0.84008856676518917,0.12059959070757031,0.28553641680628061,\
                    0.86075146449729800,0.84941016510128975,0.52570608118548989}


#define ZREAL_IN12 {0.75604385416954756,0.00022113462910056,0.33032709173858166,0.66538110421970487,\
                    0.62839178834110498,0.84974523587152362,0.68573101982474327,0.87821648130193353,\
                    0.06837403681129217,0.56084860628470778,0.66235693730413914,0.72635067673400044}

#define ZIMAG_IN12 {0.19851438421756029,0.54425731627270579,0.23207478970289230,0.23122371966019273,\
                    0.21646326314657927,0.88338878145441413,0.65251349471509457,0.30760907428339124,\
                    0.93296162132173777,0.21460078610107303,0.31264199689030647,0.36163610080257058}


#define ZREAL_IN13 {0.2922266637906432 ,0.56642488157376647,0.48264719732105732,0.33217189135029912,\
                    0.59350947011262178,0.50153415976092219,0.43685875833034515,0.26931248093023896,\
                    0.63257448654621840,0.40519540151581168,0.91847078315913677,0.04373343335464597,\
                    0.48185089323669672}



#define ZIMAG_IN13 {0.26395560009405017,0.41481037065386772,0.28064980218186975,0.12800584640353918,\
                    0.77831285959109664,0.21190304495394230,0.11213546665385365,0.68568959552794695,\
                    0.15312166837975383,0.69708506017923355,0.84155184263363481,0.40620247554033995,\
                    0.40948254754766822}



#define ZREAL_IN14 {0.87841258011758327,0.11383596854284406,0.19983377400785685,0.56186607433483005,\
                    0.58961773291230202,0.68539796629920602,0.89062247332185507,0.50422128057107329,\
                    0.34936154074966908,0.38737787725403905,0.92228986788541079,0.94881842611357570,\
                    0.34353372454643250,0.37601187312975526}

#define ZIMAG_IN14 {0.73409405630081892,0.26157614728435874,0.49934938363730907,0.26385784195736051,\
                    0.52535630855709314,0.53762298030778766,0.11999255046248436,0.2256303490139544 ,\
                    0.62740930821746588,0.76084325974807143,0.04855662025511265,0.67239497276023030,\
                    0.20171726960688829,0.39115739194676280}

#define ZREAL_IN15 {0.83003165572881699,0.58787201577797532,0.48291792999953032,0.22328650346025825,\
                    0.84008856676518917,0.12059959070757031,0.28553641680628061,0.86075146449729800,\
                    0.84941016510128975,0.52570608118548989,0.99312098976224661,0.64885628735646605,\
                    0.99231909401714802,0.05004197778180242,0.74855065811425447}

#define ZIMAG_IN15 {0.41040589986369014,0.60845263302326202,0.85442108893766999,0.06426467280834913,\
                    0.82790829380974174,0.92623437754809856,0.56672112690284848,0.57116389367729425,\
                    0.81601104838773608,0.05689279362559319,0.55959366867318749,0.12493403162807226,\
                    0.72792222863063216,0.26777664758265018,0.54653349192813039}



#define ZREAL_IN16 {0.23122371966019273, 0.21646326314657927, 0.88338878145441413, 0.65251349471509457,\
                    0.30760907428339124, 0.93296162132173777, 0.21460078610107303, 0.31264199689030647,\
                    0.36163610080257058, 0.2922266637906432 , 0.56642488157376647, 0.48264719732105732,\
                    0.33217189135029912, 0.59350947011262178, 0.50153415976092219, 0.43685875833034515}
#define ZIMAG_IN16 {0.26931248093023896, 0.63257448654621840, 0.40519540151581168, 0.91847078315913677,\
                    0.04373343335464597, 0.48185089323669672, 0.26395560009405017, 0.41481037065386772,\
                    0.28064980218186975, 0.12800584640353918, 0.77831285959109664, 0.21190304495394230,\
                    0.11213546665385365, 0.68568959552794695, 0.15312166837975383, 0.69708506017923355}


#define ZREAL_IN32 { 0.21132486546412110, 0.75604385416954756, 0.00022113462910056, 0.33032709173858166,\
                     0.66538110421970487, 0.62839178834110498, 0.84974523587152362, 0.68573101982474327,\
                     0.87821648130193353, 0.06837403681129217, 0.56084860628470778, 0.66235693730413914,\
                     0.72635067673400044, 0.19851438421756029, 0.54425731627270579, 0.23207478970289230,\
                     0.23122371966019273, 0.21646326314657927, 0.88338878145441413, 0.65251349471509457,\
                     0.30760907428339124, 0.93296162132173777, 0.21460078610107303, 0.31264199689030647,\
                     0.36163610080257058, 0.2922266637906432 , 0.56642488157376647, 0.48264719732105732,\
                     0.33217189135029912, 0.59350947011262178, 0.50153415976092219, 0.43685875833034515}

#define ZIMAG_IN32 { 0.26931248093023896, 0.63257448654621840, 0.40519540151581168, 0.91847078315913677,\
                     0.04373343335464597, 0.48185089323669672, 0.26395560009405017, 0.41481037065386772,\
                     0.28064980218186975, 0.12800584640353918, 0.77831285959109664, 0.21190304495394230,\
                     0.11213546665385365, 0.68568959552794695, 0.15312166837975383, 0.69708506017923355,\
                     0.84155184263363481, 0.40620247554033995, 0.40948254754766822, 0.87841258011758327,\
                     0.11383596854284406, 0.19983377400785685, 0.56186607433483005, 0.58961773291230202,\
                     0.68539796629920602, 0.89062247332185507, 0.50422128057107329, 0.34936154074966908,\
                     0.38737787725403905, 0.92228986788541079, 0.94881842611357570, 0.34353372454643250}




#define ZREAL_RESULT2 {   0.16527411318384111 , - 0.16505297855474055   }


#define ZIMAG_RESULT2 {+ 0.64688644628040493, + 0.01849465793929994}



#define ZREAL_RESULT3 {  1.29709824733436108, 0.94154648753137238, 0.2434220389437592}

#define ZIMAG_RESULT3 { +0.59717983752489090,+0.61999805178742728,+0.93089271522808925}





#define ZREAL_RESULT4 {  0.62051669345237315,-0.12307844962924719, 0.24346416513435543, 0.10884282691404223}

#define ZIMAG_RESULT4 { +0.53701765113510191,+0.11296372814103961,+0.10658199037425220,-0.19571476336568594}



#define ZREAL_RESULT5 {  0.62691605370491743, 0.01919730450714778, 0.10104198714906780,-0.07720389203270116,\
 0.17979378254309186}

#define ZIMAG_RESULT5 { +0.54289909722283480,+0.13998734162926388,-0.17962778177138733,+0.12064468327985833,\
-0.06305473407586182}



#define ZREAL_RESULT6 {  0.36402360963014263, 0.09605967189946550, 0.01914935343131208, 0.11197456725252172,\
-0.00281267335181221,-0.04413721258892397}

#define ZIMAG_RESULT6 { +0.39017331654516357,-0.08647381537197443,-0.01847428942485585,-0.00928606038602692,\
-0.11828530217444963,+0.05694693691321624}





#define ZREAL_RESULT7 {  0.43821863417646711, 0.04570283341490348, 0.00786143114706625, 0.09081035877037001,\
 0.06907620238256847,-0.01879476197071369,-0.08861738164795580}

#define ZIMAG_RESULT7 { +0.39293177811694990,-0.13474546775513907,+0.02350839028687007,-0.01818990984988529,\
-0.03304492053033681,-0.13370336175502612,+0.11784427758764042}




#define ZREAL_RESULT8 {  0.50006150756962597, 0.02175013240250377, 0.12240753043442965,-0.02322026233124427,\
-0.00844178465195000,-0.11389905106099872, 0.09979579551145434,-0.05419655160111495}

#define ZIMAG_RESULT8 { +0.39448237349279225,-0.14241475898929634,-0.02220236556604505,-0.14652524166940226,\
-0.02577395853586495,+0.03536239646532335,+0.04400678444653749,+0.07766555645702852}




#define ZREAL_RESULT9 {  0.4653623884336816 ,-0.10996977634919125,-0.00630644375731721,-0.04060246287025721,\
 0.05408191850705556, 0.01600258566680582,-0.05864725873797094,-0.05201672319102815,\
-0.03668050804158550}

#define ZIMAG_RESULT9 { +0.47890158897886670,+0.08218728720967677,-0.06845717881236174,-0.04807038546781070,\
-0.08014536882440566,-0.2030002369914648,+0.22453417104910142,-0.06865385510029860, \
-0.04798354111106445}




#define ZREAL_RESULT10 {  0.47991688111796976,-0.06991181823164128,-0.08469577602979275, 0.07318652166556244,\
-0.07052463321312863, 0.03129850449040533,-0.12994121698152189,-0.09609430997689952,\
 0.06979737512491652, 0.01343173518070941}

#define ZIMAG_RESULT10 { +0.38964297915808854,+0.03225815718776807,-0.11005045339360417,+0.07000358882571070,\
-0.03882538926347270,+0.01466467357240619,+0.09131179012889759,-0.02808828423670948,\
-0.11655017979588442,-0.07314316252300754}





#define ZREAL_RESULT11 {  0.40752098586579621, 0.01832270994676578, 0.00727102452352803,-0.13808643998727593,\
 0.03761367031409660, 0.01913753006534984, 0.14829738766302858,-0.03282455871021355,\
 0.08531354288243319,-0.01850240581569137,-0.03471406311050802}

#define ZIMAG_RESULT11 { +0.54521434381604206,+0.04263232089408980,+0.01080182331995891,-0.16507135273435664,\
+0.08926061372379648,+0.01327764965004020,-0.10524284127370778,-0.02743692407216465,\
+0.01530157234736101,-0.10904555975048559,+0.08146574602618931}



#define ZREAL_RESULT12 {  0.56766566393586493,-0.02954179400241778, 0.03231997586448970,-0.08061527988562983,\
 0.12393588887272257,-0.00005011576253769,-0.04579487590429687, 0.18096695473171109,\
-0.02460040668391173, 0.04301438496137660, 0.06736119091227670,-0.07861773287010031}

#define ZIMAG_RESULT12 { +0.42399044404737651,-0.08988211863004507,-0.06417895954194777,-0.05903535817439356,\
-0.03585983457125090,-0.08163805724791348,+0.00020448095165193,-0.00326952155569338,\
-0.06391751330264545,+0.08415352273732421,+0.16527532188314309,-0.07732802237804590}

#define ZREAL_RESULT13 {  0.45819311546018493, 0.04715640399909302,-0.02841189190012001, 0.01471405402274759,\
-0.06955117696838889, 0.08520673650173283,-0.08379007722081511,-0.06164570839841994,\
-0.07511713555210725, 0.07790018146444697, 0.05666948441941852,-0.04832646187709128,\
-0.08077086016003779}

#define ZIMAG_RESULT13 { +0.41406970618006128,-0.00941536236456233,-0.04150925434362464,+0.07093289809698045,\
+0.05642954059283156,-0.13856574403878158,-0.06657177623345527,+0.08978984633083976,\
-0.03246486275259956,+0.03475196544714984,-0.02951956674329188,-0.08259093889333179,\
-0.00138085118416531}




#define ZREAL_RESULT14 {  0.55365722569903086,-0.03431117289603670,-0.07434176530056802, 0.09479458498406634,\
 0.04225809799112697, 0.10711066217766220, 0.09039926979534348, 0.04258158766398475,\
-0.06766097168605688, 0.02983162597647674, 0.16148055537460848, 0.00306446154680962,\
-0.01447548152915653,-0.05597609967970790}

#define ZIMAG_RESULT14 { +0.41925417428969269,-0.02346521727072965,-0.07537174377902310,+0.12152793513158673,\
-0.05435563225189670,+0.05588517124820697,+0.03948028804374615,-0.02547196042723954,\
+0.04750412291672844,+0.09081187301248270,+0.02410149252380316,-0.00728204510680848,\
+0.07924950091433607,+0.04222609705593354}




#define ZREAL_RESULT15 {  0.60260595980410780,-0.09025313009309296, 0.07296362400098141,-0.01312199542974801,\
 0.05196795946996628, 0.00939805486763355, 0.07359039581140392, 0.07244754050834930,\
-0.13409874398389474,-0.06419327756841406,-0.04062806443214259, 0.19283708306652805,\
 0.04903632944886174,-0.00831723960417976, 0.05579715986245712}

#define ZIMAG_RESULT15 { +0.52861572646846366,-0.09479760020968324,+0.06725385149296267,+0.01462043430946725,\
-0.00503137552191604,-0.05388544405841567,-0.05301311586381348,-0.11414862867731596,\
+0.0617906214666224 ,+0.14723827146724461,-0.10948893804382542,+0.00343655851962967,\
-0.00538333435303664,-0.03924473093854008,+0.06244360380584638}




#define ZREAL_RESULT16 {  0.45740074128843844,-0.00119937109332679,-0.09568979823192530,-0.01738371075123765,\
-0.03869746159762144,-0.05391866381953740, 0.12416191581397301,-0.03041585422893309,\
-0.03257706691510975, 0.07000836728092155,-0.00597921899357861,-0.03766276570110767,\
-0.07796601625159383,-0.03039291932884991,-0.03422318488120085, 0.03575872707088203}

#define ZIMAG_RESULT16 { +0.40480042458511889,+0.06953094033520663,+0.02374777548578576,-0.03393710545080092,\
-0.04651592345908284,+0.02783744013794178,-0.03035825342604098,-0.06447129475682091,\
-0.11649833549745381,-0.11413977271896644,+0.11722373704793855,+0.08102030470498481,\
-0.06532836984843016,+0.00779635766618338,-0.01208991333178105,+0.02069446945645629}





#define ZREAL_RESULT32 { 0.47864284948445857, 0.05482834035731039,-0.01530773095660632,-0.01750083123738436,\
                        -0.05071297103611097, 0.08766815171372286, 0.02106608347521239,-0.05109320958418949,\
                        -0.01096817385405302, 0.02416709596518544,-0.05098444057349576,-0.01448958336761648,\
                         0.02532074256370129,-0.09984237842467819,-0.06358972398280244,-0.04611519259707146,\
                         0.01104057650081813,-0.03475148917606731, 0.00813855773340646,-0.00568704237169753,\
                         0.01830285230723486, 0.02749700538425181,-0.04780122330152622, 0.06632936566012661,\
                        -0.01447601290419698, 0.03116875276343186, 0.00123360692547217, 0.01833898177168673,\
                        -0.03654957125464739, 0.00547684889715929,-0.05208112856470785,-0.05594424285220599}



#define ZIMAG_RESULT32 {+0.48466352955438197,+0.04862008402656083,+0.04890918577469192,-0.07661834259188156,\
                        +0.01684868035424048,-0.06277273594077343,-0.03536555772325747,-0.04086803002308132,\
                        -0.04373891325667500,+0.03378928555068783,+0.09272389885362962,-0.01252633931303314,\
                        +0.04715379187194862,+0.04212975779784539,+0.00924226411866865,-0.06146319342441199,\
                        -0.06222798605449498,+0.03846733709242996,-0.06362612522976926,+0.02110331414670667,\
                        +0.06568727290671772,-0.09854847704278780,+0.04828755172931871,-0.06711471562709365,\
                        -0.03694727551192045,-0.04323841461679329,-0.07059643871085296,-0.01972938456264230,\
                        +0.04778892314703904,+0.02763841783182228,+0.00662935995827029,-0.01498824415525237}

static void zifftmaTest2 (void )
{
      int i = 0 ;

    double tRealIn [] = ZREAL_IN2;
    double tImagIn [] = ZIMAG_IN2 ;



    double tRealResult [] = ZREAL_RESULT2 ;
    double tImagResult [] = ZIMAG_RESULT2 ;



    doubleComplex*  out     = (doubleComplex*) malloc ( sizeof(doubleComplex) * (unsigned int) (ROW*COLS2));
    doubleComplex*  in      = DoubleComplexMatrix ( tRealIn , tImagIn , ROW*COLS2 );
    doubleComplex* Result   = DoubleComplexMatrix ( tRealResult , tImagResult ,ROW*COLS2) ;



    zifftma ( in , ROW , COLS2 , out ) ;

    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/


	for ( i = 0 ; i < (ROW*COLS2 )  ; i++ )
	{
	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
                i ,
                zreals(out[i]) ,
                zimags(out[i]),
                zreals (Result[i])  ,
                zimags (Result[i]),
                fabs(  zreals(out[i]) -  zreals (Result[i]) ) / fabs (zreals (out[i])) ,
                fabs(  zimags(out[i]) -  zimags (Result[i]) ) / fabs (zimags (out[i])));

   if (  zreals(out[i])  < 1e-14 && zreals (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  zreals(out[i]) -  zreals (Result[i]) ) / fabs (zreals (out[i]))  < 3e-16 );


    if (  zimags(out[i])  < 1e-14 && zimags (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  zimags(out[i]) -  zimags (Result[i]) ) / fabs (zimags (out[i]))  < 3e-16 ) ;

    }

    free(out);
    free(in);
    free(Result);

}

static void zifftmaTest3 (void )
{
      int i = 0 ;

    double tRealIn [] = ZREAL_IN3;
    double tImagIn [] = ZIMAG_IN3 ;



    double tRealResult [] = ZREAL_RESULT3;
    double tImagResult [] = ZIMAG_RESULT3 ;



    doubleComplex*  out     = (doubleComplex*) malloc ( sizeof(doubleComplex) * (unsigned int) (ROW*COLS3));
    doubleComplex*  in      = DoubleComplexMatrix ( tRealIn , tImagIn , ROW*COLS3 );
    doubleComplex* Result   = DoubleComplexMatrix ( tRealResult , tImagResult ,ROW*COLS3) ;



    zifftma ( in , ROW , COLS3 , out ) ;

    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/


	for ( i = 0 ; i < (ROW*COLS3 )  ; i++ )
	{
	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
                i ,
                zreals(out[i]) ,
                zimags(out[i]),
                zreals (Result[i])  ,
                zimags (Result[i]),
                fabs(  zreals(out[i]) -  zreals (Result[i]) ) / fabs (zreals (out[i])) ,
                fabs(  zimags(out[i]) -  zimags (Result[i]) ) / fabs (zimags (out[i])));

   if (  zreals(out[i])  < 1e-14 && zreals (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  zreals(out[i]) -  zreals (Result[i]) ) / fabs (zreals (out[i]))  < 3e-16 );


    if (  zimags(out[i])  < 1e-14 && zimags (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  zimags(out[i]) -  zimags (Result[i]) ) / fabs (zimags (out[i]))  < 3e-15 ) ;

    }

    free(out);
    free(in);
    free(Result);


}


static void zifftmaTest4 (void )
{
      int i = 0 ;

    double tRealIn [] = ZREAL_IN4;
    double tImagIn [] = ZIMAG_IN4 ;



    double tRealResult [] = ZREAL_RESULT4 ;
    double tImagResult [] = ZIMAG_RESULT4 ;



    doubleComplex*  out     = (doubleComplex*) malloc ( sizeof(doubleComplex) * (unsigned int) (ROW*COLS4));
    doubleComplex*  in      = DoubleComplexMatrix ( tRealIn , tImagIn , ROW*COLS4 );
    doubleComplex* Result   = DoubleComplexMatrix ( tRealResult , tImagResult ,ROW*COLS4) ;


    zifftma ( in , ROW , COLS4 , out ) ;

    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/
	for ( i = 0 ; i < (ROW*COLS4 )  ; i++ )
	{
	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
              i ,zreals(out[i]) , zimags(out[i]), zreals (Result[i])  , zimags (Result[i]),
              fabs(  zreals(out[i]) -  zreals (Result[i]) ) / fabs (zreals (out[i])) ,
              fabs(  zimags(out[i]) -  zimags (Result[i]) ) / fabs (zimags (out[i])));

   if (  zreals(out[i])  < 1e-14 && zreals (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  zreals(out[i]) -  zreals (Result[i]) ) / fabs (zreals (out[i]))  < 3e-16 );


    if (  zimags(out[i])  < 1e-14 && zimags (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  zimags(out[i]) -  zimags (Result[i]) ) / fabs (zimags (out[i]))  < 3e-16 ) ;

    }

    free(out);
    free(in);
    free(Result);


}


static void zifftmaTest5 (void )
{
      int i = 0 ;

    double tRealIn [] = ZREAL_IN5;
    double tImagIn [] = ZIMAG_IN5 ;



    double tRealResult [] = ZREAL_RESULT5;
    double tImagResult [] = ZIMAG_RESULT5 ;



    doubleComplex*  out     = (doubleComplex*) malloc ( sizeof(doubleComplex) * (unsigned int) (ROW*COLS5));
    doubleComplex*  in      = DoubleComplexMatrix ( tRealIn , tImagIn , ROW*COLS5 );
    doubleComplex* Result   = DoubleComplexMatrix ( tRealResult , tImagResult ,ROW*COLS5) ;



    zifftma ( in , ROW , COLS5 , out ) ;

    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/


	for ( i = 0 ; i < (ROW*COLS5 )  ; i++ )
	{
	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
                i ,
                zreals(out[i]) ,
                zimags(out[i]),
                zreals (Result[i])  ,
                zimags (Result[i]),
                fabs(  zreals(out[i]) -  zreals (Result[i]) ) / fabs (zreals (out[i])) ,
                fabs(  zimags(out[i]) -  zimags (Result[i]) ) / fabs (zimags (out[i])));

   if (  zreals(out[i])  < 1e-14 && zreals (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  zreals(out[i]) -  zreals (Result[i]) ) / fabs (zreals (out[i]))  < 3e-15 );


    if (  zimags(out[i])  < 1e-14 && zimags (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  zimags(out[i]) -  zimags (Result[i]) ) / fabs (zimags (out[i]))  < 3e-15 ) ;

    }

    free(out);
    free(in);
    free(Result);


}


static void zifftmaTest6 (void )
{
      int i = 0 ;

    double tRealIn [] = ZREAL_IN6;
    double tImagIn [] = ZIMAG_IN6 ;



    double tRealResult [] = ZREAL_RESULT6;
    double tImagResult [] = ZIMAG_RESULT6;



    doubleComplex*  out     = (doubleComplex*) malloc ( sizeof(doubleComplex) * (unsigned int) (ROW*COLS6));
    doubleComplex*  in      = DoubleComplexMatrix ( tRealIn , tImagIn , ROW*COLS6 );
    doubleComplex* Result   = DoubleComplexMatrix ( tRealResult , tImagResult ,ROW*COLS6) ;



    zifftma ( in , ROW , COLS6 , out ) ;

    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/


	for ( i = 0 ; i < (ROW*COLS6 )  ; i++ )
	{
	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
                i ,
                zreals(out[i]) ,
                zimags(out[i]),
                zreals (Result[i])  ,
                zimags (Result[i]),
                fabs(  zreals(out[i]) -  zreals (Result[i]) ) / fabs (zreals (out[i])) ,
                fabs(  zimags(out[i]) -  zimags (Result[i]) ) / fabs (zimags (out[i])));

   if (  zreals(out[i])  < 1e-14 && zreals (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  zreals(out[i]) -  zreals (Result[i]) ) / fabs (zreals (out[i]))  < 3e-15 );


    if (  zimags(out[i])  < 1e-14 && zimags (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  zimags(out[i]) -  zimags (Result[i]) ) / fabs (zimags (out[i]))  < 3e-15 ) ;

    }

    free(out);
    free(in);
    free(Result);


}


static void zifftmaTest7 (void )
{
      int i = 0 ;

    double tRealIn [] = ZREAL_IN7;
    double tImagIn [] = ZIMAG_IN7 ;



    double tRealResult [] = ZREAL_RESULT7;
    double tImagResult [] = ZIMAG_RESULT7;



    doubleComplex*  out     = (doubleComplex*) malloc ( sizeof(doubleComplex) * (unsigned int) (ROW*COLS7));
    doubleComplex*  in      = DoubleComplexMatrix ( tRealIn , tImagIn , ROW*COLS7 );
    doubleComplex* Result   = DoubleComplexMatrix ( tRealResult , tImagResult ,ROW*COLS7) ;



    zifftma ( in , ROW , COLS7 , out ) ;

    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/


	for ( i = 0 ; i < (ROW*COLS7 )  ; i++ )
	{
	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
                i ,
                zreals(out[i]) ,
                zimags(out[i]),
                zreals (Result[i])  ,
                zimags (Result[i]),
                fabs(  zreals(out[i]) -  zreals (Result[i]) ) / fabs (zreals (out[i])) ,
                fabs(  zimags(out[i]) -  zimags (Result[i]) ) / fabs (zimags (out[i])));

   if (  zreals(out[i])  < 1e-14 && zreals (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  zreals(out[i]) -  zreals (Result[i]) ) / fabs (zreals (out[i]))  < 3e-15 );


    if (  zimags(out[i])  < 1e-14 && zimags (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  zimags(out[i]) -  zimags (Result[i]) ) / fabs (zimags (out[i]))  < 3e-15 ) ;

    }

    free(out);
    free(in);
    free(Result);


}

static void zifftmaTest8 (void )
{
      int i = 0 ;

    double tRealIn [] = ZREAL_IN8;
    double tImagIn [] = ZIMAG_IN8 ;



    double tRealResult [] = ZREAL_RESULT8 ;
    double tImagResult [] = ZIMAG_RESULT8 ;



    doubleComplex*  out     = (doubleComplex*) malloc ( sizeof(doubleComplex) * (unsigned int) (ROW*COLS8));
    doubleComplex*  in      = DoubleComplexMatrix ( tRealIn , tImagIn , ROW*COLS8 );
    doubleComplex* Result   = DoubleComplexMatrix ( tRealResult , tImagResult ,ROW*COLS8) ;


    zifftma ( in , ROW , COLS8 , out ) ;

    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/
	for ( i = 0 ; i < (ROW*COLS8 )  ; i++ )
	{
	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
              i ,zreals(out[i]) , zimags(out[i]), zreals (Result[i])  , zimags (Result[i]),
              fabs(  zreals(out[i]) -  zreals (Result[i]) ) / fabs (zreals (out[i])) ,
              fabs(  zimags(out[i]) -  zimags (Result[i]) ) / fabs (zimags (out[i])));
/*
   if (  zreals(out[i])  < 1e-14 && zreals (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  zreals(out[i]) -  zreals (Result[i]) ) / fabs (zreals (out[i]))  < 3e-16 );


    if (  zimags(out[i])  < 1e-14 && zimags (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  zimags(out[i]) -  zimags (Result[i]) ) / fabs (zimags (out[i]))  < 3e-16 ) ;
*/
    }

    free(out);
    free(in);
    free(Result);


}


static void zifftmaTest9 (void )
{
      int i = 0 ;

    double tRealIn [] = ZREAL_IN9;
    double tImagIn [] = ZIMAG_IN9 ;



    double tRealResult [] = ZREAL_RESULT9 ;
    double tImagResult [] = ZIMAG_RESULT9 ;



    doubleComplex*  out     = (doubleComplex*) malloc ( sizeof(doubleComplex) * (unsigned int) (ROW*COLS9));
    doubleComplex*  in      = DoubleComplexMatrix ( tRealIn , tImagIn , ROW*COLS9 );
    doubleComplex* Result   = DoubleComplexMatrix ( tRealResult , tImagResult ,ROW*COLS9) ;


    zifftma ( in , ROW , COLS9 , out ) ;

    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/
	for ( i = 0 ; i < (ROW*COLS9 )  ; i++ )
	{
	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
              i ,zreals(out[i]) , zimags(out[i]), zreals (Result[i])  , zimags (Result[i]),
              fabs(  zreals(out[i]) -  zreals (Result[i]) ) / fabs (zreals (out[i])) ,
              fabs(  zimags(out[i]) -  zimags (Result[i]) ) / fabs (zimags (out[i])));

   if (  zreals(out[i])  < 1e-14 && zreals (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  zreals(out[i]) -  zreals (Result[i]) ) / fabs (zreals (out[i]))  < 3e-15 );


    if (  zimags(out[i])  < 1e-14 && zimags (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  zimags(out[i]) -  zimags (Result[i]) ) / fabs (zimags (out[i]))  < 3e-15 ) ;

    }

    free(out);
    free(in);
    free(Result);


}

static void zifftmaTest10 (void )
{
      int i = 0 ;

    double tRealIn [] = ZREAL_IN10;
    double tImagIn [] = ZIMAG_IN10 ;



    double tRealResult [] = ZREAL_RESULT10 ;
    double tImagResult [] = ZIMAG_RESULT10 ;



    doubleComplex*  out     = (doubleComplex*) malloc ( sizeof(doubleComplex) * (unsigned int) (ROW*COLS10));
    doubleComplex*  in      = DoubleComplexMatrix ( tRealIn , tImagIn , ROW*COLS10 );



    zifftma ( in , ROW , COLS10 , out );




    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/
		for ( i = 0 ; i < (ROW*COLS10 )  ; i++ )
	{


	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
                i ,
                zreals(out[i]) ,
                zimags(out[i]),
                tRealResult[i]  ,
                tImagResult[i],
                fabs(  zreals(out[i]) -  tRealResult[i] ) / fabs (zreals (out[i])) ,
                fabs(  zimags(out[i]) -  tImagResult[i] ) / fabs (zimags (out[i])));


   if (  zreals(out[i])  < 1e-14 && tRealResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  zreals(out[i]) -  tRealResult[i] ) / fabs (zreals (out[i]))  < 3e-15 );


    if (  zimags(out[i])  < 1e-14 && tImagResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  zimags(out[i]) -  tImagResult[i] ) / fabs (zimags (out[i]))  < 3e-15 ) ;

    }

    free(out);
    free(in);

}

static void zifftmaTest11 (void )
{
      int i = 0 ;

    double tRealIn [] = ZREAL_IN11;
    double tImagIn [] = ZIMAG_IN11 ;



    double tRealResult [] = ZREAL_RESULT11 ;
    double tImagResult [] = ZIMAG_RESULT11 ;



    doubleComplex*  out     = (doubleComplex*) malloc ( sizeof(doubleComplex) * (unsigned int) (ROW*COLS11));
    doubleComplex*  in      = DoubleComplexMatrix ( tRealIn , tImagIn , ROW*COLS11 );



    zifftma ( in , ROW , COLS11 , out );




    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/
		for ( i = 0 ; i < (ROW*COLS11 )  ; i++ )
	{


	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
                i ,
                zreals(out[i]) ,
                zimags(out[i]),
                tRealResult[i]  ,
                tImagResult[i],
                fabs(  zreals(out[i]) -  tRealResult[i] ) / fabs (zreals (out[i])) ,
                fabs(  zimags(out[i]) -  tImagResult[i] ) / fabs (zimags (out[i])));


   if (  zreals(out[i])  < 1e-14 && tRealResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  zreals(out[i]) -  tRealResult[i] ) / fabs (zreals (out[i]))  < 3e-15 );


    if (  zimags(out[i])  < 1e-14 && tImagResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  zimags(out[i]) -  tImagResult[i] ) / fabs (zimags (out[i]))  < 3e-15 ) ;

    }

    free(out);
    free(in);

}


static void zifftmaTest12 (void )
{
      int i = 0 ;

    double tRealIn [] = ZREAL_IN12;
    double tImagIn [] = ZIMAG_IN12 ;



    double tRealResult [] = ZREAL_RESULT12 ;
    double tImagResult [] = ZIMAG_RESULT12 ;



    doubleComplex*  out     = (doubleComplex*) malloc ( sizeof(doubleComplex) * (unsigned int) (ROW*COLS12));
    doubleComplex*  in      = DoubleComplexMatrix ( tRealIn , tImagIn , ROW*COLS12 );



    zifftma ( in , ROW , COLS12 , out );




    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/
		for ( i = 0 ; i < (ROW*COLS12 )  ; i++ )
	{


	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
                i ,
                zreals(out[i]) ,
                zimags(out[i]),
                tRealResult[i]  ,
                tImagResult[i],
                fabs(  zreals(out[i]) -  tRealResult[i] ) / fabs (zreals (out[i])) ,
                fabs(  zimags(out[i]) -  tImagResult[i] ) / fabs (zimags (out[i])));


   if (  zreals(out[i])  < 1e-14 && tRealResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  zreals(out[i]) -  tRealResult[i] ) / fabs (zreals (out[i]))  < 3e-15 );


    if (  zimags(out[i])  < 1e-14 && tImagResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  zimags(out[i]) -  tImagResult[i] ) / fabs (zimags (out[i]))  < 3e-14 ) ;

    }

    free(out);
    free(in);

}


static void zifftmaTest13 (void )
{
      int i = 0 ;

    double tRealIn [] = ZREAL_IN13;
    double tImagIn [] = ZIMAG_IN13 ;



    double tRealResult [] = ZREAL_RESULT13 ;
    double tImagResult [] = ZIMAG_RESULT13 ;



    doubleComplex*  out     = (doubleComplex*) malloc ( sizeof(doubleComplex) * (unsigned int) (ROW*COLS13));
    doubleComplex*  in      = DoubleComplexMatrix ( tRealIn , tImagIn , ROW*COLS13 );



    zifftma ( in , ROW , COLS13 , out );




    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/
		for ( i = 0 ; i < (ROW*COLS13 )  ; i++ )
	{


	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
                i ,
                zreals(out[i]) ,
                zimags(out[i]),
                tRealResult[i]  ,
                tImagResult[i],
                fabs(  zreals(out[i]) -  tRealResult[i] ) / fabs (zreals (out[i])) ,
                fabs(  zimags(out[i]) -  tImagResult[i] ) / fabs (zimags (out[i])));


   if (  zreals(out[i])  < 1e-14 && tRealResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  zreals(out[i]) -  tRealResult[i] ) / fabs (zreals (out[i]))  < 3e-15 );


    if (  zimags(out[i])  < 1e-14 && tImagResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  zimags(out[i]) -  tImagResult[i] ) / fabs (zimags (out[i]))  < 3e-15 ) ;

    }

    free(out);
    free(in);

}


static void zifftmaTest14 (void )
{
      int i = 0 ;

    double tRealIn [] = ZREAL_IN14;
    double tImagIn [] = ZIMAG_IN14 ;



    double tRealResult [] = ZREAL_RESULT14 ;
    double tImagResult [] = ZIMAG_RESULT14 ;



    doubleComplex*  out     = (doubleComplex*) malloc ( sizeof(doubleComplex) * (unsigned int) (ROW*COLS14));
    doubleComplex*  in      = DoubleComplexMatrix ( tRealIn , tImagIn , ROW*COLS14 );



    zifftma ( in , ROW , COLS14 , out );




    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/
		for ( i = 0 ; i < (ROW*COLS14 )  ; i++ )
	{


	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
                i ,
                zreals(out[i]) ,
                zimags(out[i]),
                tRealResult[i]  ,
                tImagResult[i],
                fabs(  zreals(out[i]) -  tRealResult[i] ) / fabs (zreals (out[i])) ,
                fabs(  zimags(out[i]) -  tImagResult[i] ) / fabs (zimags (out[i])));


   if (  zreals(out[i])  < 1e-14 && tRealResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  zreals(out[i]) -  tRealResult[i] ) / fabs (zreals (out[i]))  < 3e-14 );


    if (  zimags(out[i])  < 1e-14 && tImagResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  zimags(out[i]) -  tImagResult[i] ) / fabs (zimags (out[i]))  < 3e-15 ) ;

    }

    free(out);
    free(in);

}



static void zifftmaTest15 (void )
{
      int i = 0 ;

    double tRealIn [] = ZREAL_IN15;
    double tImagIn [] = ZIMAG_IN15 ;



    double tRealResult [] = ZREAL_RESULT15 ;
    double tImagResult [] = ZIMAG_RESULT15 ;



    doubleComplex*  out     = (doubleComplex*) malloc ( sizeof(doubleComplex) * (unsigned int) (ROW*COLS15));
    doubleComplex*  in      = DoubleComplexMatrix ( tRealIn , tImagIn , ROW*COLS15 );



    zifftma ( in , ROW , COLS15 , out );




    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/
		for ( i = 0 ; i < (ROW*COLS15 )  ; i++ )
	{


	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
                i ,
                zreals(out[i]) ,
                zimags(out[i]),
                tRealResult[i]  ,
                tImagResult[i],
                fabs(  zreals(out[i]) -  tRealResult[i] ) / fabs (zreals (out[i])) ,
                fabs(  zimags(out[i]) -  tImagResult[i] ) / fabs (zimags (out[i])));


   if (  zreals(out[i])  < 1e-14 && tRealResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  zreals(out[i]) -  tRealResult[i] ) / fabs (zreals (out[i]))  < 3e-15 );


    if (  zimags(out[i])  < 1e-14 && tImagResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  zimags(out[i]) -  tImagResult[i] ) / fabs (zimags (out[i]))  < 3e-14 ) ;

    }

    free(out);
    free(in);

}



static void zifftmaTest16 (void )
{
      int i = 0 ;

    double tRealIn [] = ZREAL_IN16;
    double tImagIn [] = ZIMAG_IN16 ;



    double tRealResult [] = ZREAL_RESULT16 ;
    double tImagResult [] = ZIMAG_RESULT16 ;



    doubleComplex*  out     = (doubleComplex*) malloc ( sizeof(doubleComplex) * (unsigned int) (ROW*COLS16));
    doubleComplex*  in      = DoubleComplexMatrix ( tRealIn , tImagIn , ROW*COLS16 );



    zifftma ( in , ROW , COLS16 , out );




    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/
		for ( i = 0 ; i < (ROW*COLS16 )  ; i++ )
	{


	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
                i ,
                zreals(out[i]) ,
                zimags(out[i]),
                tRealResult[i]  ,
                tImagResult[i],
                fabs(  zreals(out[i]) -  tRealResult[i] ) / fabs (zreals (out[i])) ,
                fabs(  zimags(out[i]) -  tImagResult[i] ) / fabs (zimags (out[i])));


   if (  zreals(out[i])  < 1e-14 && tRealResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  zreals(out[i]) -  tRealResult[i] ) / fabs (zreals (out[i]))  < 3e-15);


    if (  zimags(out[i])  < 1e-14 && tImagResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  zimags(out[i]) -  tImagResult[i] ) / fabs (zimags (out[i]))  < 3e-15 ) ;

    }

    free(out);
    free(in);

}



static void zifftmaTest32 (void )
{
      int i = 0 ;

    double tRealIn [] = ZREAL_IN32;
    double tImagIn [] = ZIMAG_IN32 ;



    double tRealResult [] = ZREAL_RESULT32 ;
    double tImagResult [] = ZIMAG_RESULT32 ;



    doubleComplex*  out     = (doubleComplex*) malloc ( sizeof(doubleComplex) * (unsigned int) (ROW*COLS32));
    doubleComplex*  in      = DoubleComplexMatrix ( tRealIn , tImagIn , ROW*COLS32 );



    zifftma ( in , ROW , COLS32 , out );




    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/
		for ( i = 0 ; i < (ROW*COLS32 )  ; i++ )
	{


	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
                i ,
                zreals(out[i]) ,
                zimags(out[i]),
                tRealResult[i]  ,
                tImagResult[i],
                fabs(  zreals(out[i]) -  tRealResult[i] ) / fabs (zreals (out[i])) ,
                fabs(  zimags(out[i]) -  tImagResult[i] ) / fabs (zimags (out[i])));


   if (  zreals(out[i])  < 1e-14 && tRealResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  zreals(out[i]) -  tRealResult[i] ) / fabs (zreals (out[i]))  < 3e-14 );


    if (  zimags(out[i])  < 1e-14 && tImagResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  zimags(out[i]) -  tImagResult[i] ) / fabs (zimags (out[i]))  < 3e-15 ) ;

    }

    free(out);
    free(in);


}

static int testFft(void) {

  printf("\n>>>> FFT Tests\n");
  printf("\t>>>> Matrix Double Realt Tests\n");
  /*dfftmaTest();*/

  printf("\n\n\n");

  printf("\n\t>>>> Vector 2 Double Complex Tests\n");
  zifftmaTest2();
  printf("\n\t>>>> Vector 3 Double Complex Tests\n");
  zifftmaTest3();

  printf("\n\t>>>> Vector 4 Double Complex Tests\n");
  zifftmaTest4();


  printf("\n\t>>>> Vector 5 Double Complex Tests\n");
  zifftmaTest5();
  printf("\n\t>>>> Vector 6 Double Complex Tests\n");
  zifftmaTest6();
  printf("\n\t>>>> Vector 7 Double Complex Tests\n");
  zifftmaTest7();

  printf("\n\t>>>> Vector 8 Double Complex Tests\n");
  zifftmaTest8();

  printf("\n\t>>>> Vector 9 Double Complex Tests\n");
  zifftmaTest9();
  printf("\n\t>>>> Vector 10 Double Complex Tests\n");
  zifftmaTest10();
  printf("\n\t>>>> Vector 11 Double Complex Tests\n");
  zifftmaTest11();
  printf("\n\t>>>> Vector 12 Double Complex Tests\n");
  zifftmaTest12();
  printf("\n\t>>>> Vector 13 Double Complex Tests\n");
  zifftmaTest13();
  printf("\n\t>>>> Vector 14 Double Complex Tests\n");
  zifftmaTest14();
  printf("\n\t>>>> Vector 14 Double Complex Tests\n");
  zifftmaTest15();
  printf("\n\t>>>> Vector 16 Double Complex Tests\n");
  zifftmaTest16();
  printf("\n\t>>>> Vector 32 Double Complex Tests\n");
  zifftmaTest32();

  return 0;
}



int main(void) {
  assert(testFft() == 0);
  return 0;
}

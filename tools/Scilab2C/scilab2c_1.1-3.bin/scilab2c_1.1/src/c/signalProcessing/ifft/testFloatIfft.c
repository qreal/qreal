
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

#define CREAL_IN2 { 0.00022113462910056f  ,  0.33032709173858166f  }
#define CIMAG_IN2 { 0.66538110421970487f  ,  0.62839178834110498f  }

#define CREAL_IN3 { 2.48206677380949259f,  0.43537130765616894f, 0.97385666053742170f}
#define CIMAG_IN3 { 2.14807060454040766f,- 0.78285905346274376f, 0.42632796149700880f}


#define CREAL_IN4 { 0.84974523587152362f,  0.68573101982474327f, 0.87821648130193353f, 0.06837403681129217f}
#define CIMAG_IN4 { 0.56084860628470778f,  0.66235693730413914f, 0.72635067673400044f, 0.19851438421756029f}

#define CREAL_IN5 { 0.84974523587152362f,  0.68573101982474327f, 0.87821648130193353f, 0.06837403681129217f,\
                    0.65251349471509457f}
#define CIMAG_IN5 { 0.56084860628470778f,  0.66235693730413914f, 0.72635067673400044f, 0.19851438421756029f,\
                    0.56642488157376647f}

#define CREAL_IN6 {  0.54425731627270579f, 0.23207478970289230f, 0.23122371966019273f, 0.21646326314657927f,\
                     0.65251349471509457f, 0.30760907428339124f }
#define CIMAG_IN6 {  0.21460078610107303f, 0.31264199689030647f, 0.36163610080257058f, 0.2922266637906432f,\
                     0.56642488157376647f, 0.59350947011262178f }

#define CREAL_IN7 {  0.54425731627270579f, 0.23207478970289230f, 0.23122371966019273f, 0.21646326314657927f,\
                     0.65251349471509457f, 0.88338878145441413f, 0.30760907428339124f }
#define CIMAG_IN7 {  0.21460078610107303f, 0.31264199689030647f, 0.36163610080257058f, 0.2922266637906432f,\
                     0.40948254754766822f, 0.56642488157376647f, 0.59350947011262178f }



#define CREAL_IN8 {  0.54425731627270579f, 0.23207478970289230f, 0.23122371966019273f, 0.21646326314657927f,\
                     0.88338878145441413f, 0.65251349471509457f, 0.30760907428339124f, 0.93296162132173777f }
#define CIMAG_IN8 {  0.21460078610107303f, 0.31264199689030647f, 0.36163610080257058f, 0.2922266637906432f,\
                     0.56642488157376647f, 0.48264719732105732f, 0.33217189135029912f, 0.59350947011262178f}


#define CREAL_IN9 { 0.23122371966019273f, 0.21646326314657927f, 0.88338878145441413f, 0.65251349471509457f,\
                    0.30760907428339124f, 0.93296162132173777f, 0.21460078610107303f, 0.31264199689030647f,\
                    0.43685875833034515f}
#define CIMAG_IN9 { 0.26931248093023896f, 0.63257448654621840f, 0.40519540151581168f, 0.91847078315913677f,\
                    0.28064980218186975f, 0.12800584640353918f, 0.77831285959109664f, 0.21190304495394230f,\
                    0.68568959552794695f}


#define CREAL_IN10 { 0.21646326314657927f, 0.65251349471509457f, 0.63257448654621840f, 0.31264199689030647f,\
                     0.93296162132173777f, 0.31264199689030647f, 0.48185089323669672f, 0.48264719732105732f,\
                     0.2922266637906432f , 0.48264719732105732f }
#define CIMAG_IN10 { 0.23122371966019273f, 0.21646326314657927f, 0.88338878145441413f, 0.65251349471509457f,\
                     0.11383596854284406f, 0.19983377400785685f, 0.56186607433483005f, 0.58961773291230202f,\
                     0.23122371966019273f, 0.21646326314657927f}
/*
#define CREAL_IN10 { 1,2,3,4,5,6,7,8,9,10 }


#define CIMAG_IN10 { 1,2,3,4,5,6,7,8,9,10 }
*/




#define CREAL_IN11 {0.49934938363730907f, 0.26385784195736051f, 0.52535630855709314f ,0.53762298030778766f,\
                    0.11999255046248436f, 0.2256303490139544f , 0.62740930821746588f ,0.76084325974807143f,\
                    0.04855662025511265f, 0.67239497276023030f, 0.20171726960688829f }

#define CIMAG_IN11 {0.39115739194676280f, 0.83003165572881699f,0.58787201577797532f ,0.48291792999953032f,\
                    0.22328650346025825f, 0.84008856676518917f,0.12059959070757031f ,0.28553641680628061f,\
                    0.86075146449729800f, 0.84941016510128975f,0.52570608118548989f }


#define CREAL_IN12 {0.75604385416954756f, 0.00022113462910056f,0.33032709173858166f ,0.66538110421970487f,\
                    0.62839178834110498f,0.84974523587152362f ,0.68573101982474327f ,0.87821648130193353f,\
                    0.06837403681129217f,0.56084860628470778f ,0.66235693730413914f ,0.72635067673400044f}

#define CIMAG_IN12 {0.19851438421756029f, 0.54425731627270579f, 0.23207478970289230f, 0.23122371966019273f,\
                    0.21646326314657927f, 0.88338878145441413f, 0.65251349471509457f, 0.30760907428339124f,\
                    0.93296162132173777f, 0.21460078610107303f, 0.31264199689030647f, 0.36163610080257058f}

#define CREAL_IN13 {0.2922266637906432f , 0.56642488157376647f,0.48264719732105732f,0.33217189135029912f,\
                    0.59350947011262178f, 0.50153415976092219f,0.43685875833034515f,0.26931248093023896f,\
                    0.63257448654621840f, 0.40519540151581168f,0.91847078315913677f,0.04373343335464597f,\
                    0.48185089323669672f}



#define CIMAG_IN13 {0.26395560009405017f,0.41481037065386772f,0.28064980218186975f,0.12800584640353918f,\
                    0.77831285959109664f,0.21190304495394230f,0.11213546665385365f,0.68568959552794695f,\
                    0.15312166837975383f,0.69708506017923355f,0.84155184263363481f,0.40620247554033995f,\
                    0.40948254754766822f}



#define CREAL_IN14 {0.87841258011758327f,0.11383596854284406f,0.19983377400785685f,0.56186607433483005f,\
                    0.58961773291230202f,0.68539796629920602f,0.89062247332185507f,0.50422128057107329f,\
                    0.34936154074966908f,0.38737787725403905f,0.92228986788541079f,0.94881842611357570f,\
                    0.34353372454643250f,0.37601187312975526f}

#define CIMAG_IN14 {0.73409405630081892f,0.26157614728435874f,0.49934938363730907f,0.26385784195736051f,\
                    0.52535630855709314f,0.53762298030778766f,0.11999255046248436f,0.2256303490139544f ,\
                    0.62740930821746588f,0.76084325974807143f,0.04855662025511265f,0.67239497276023030f,\
                    0.20171726960688829f,0.39115739194676280f}

#define CREAL_IN15 {0.83003165572881699f,0.58787201577797532f,0.48291792999953032f,0.22328650346025825f,\
                    0.84008856676518917f,0.12059959070757031f,0.28553641680628061f,0.86075146449729800f,\
                    0.84941016510128975f,0.52570608118548989f,0.99312098976224661f,0.64885628735646605f,\
                    0.99231909401714802f,0.05004197778180242f,0.74855065811425447f}

#define CIMAG_IN15 {0.41040589986369014f,0.60845263302326202f,0.85442108893766999f,0.06426467280834913f,\
                    0.82790829380974174f,0.92623437754809856f,0.56672112690284848f,0.57116389367729425f,\
                    0.81601104838773608f,0.05689279362559319f,0.55959366867318749f,0.12493403162807226f,\
                    0.72792222863063216f,0.26777664758265018f,0.54653349192813039f}


#define CREAL_IN16 {0.23122371966019273f, 0.21646326314657927f, 0.88338878145441413f, 0.65251349471509457f,\
                    0.30760907428339124f, 0.93296162132173777f, 0.21460078610107303f, 0.31264199689030647f,\
                    0.36163610080257058f, 0.2922266637906432f , 0.56642488157376647f, 0.48264719732105732f,\
                    0.33217189135029912f, 0.59350947011262178f, 0.50153415976092219f, 0.43685875833034515f}
#define CIMAG_IN16 {0.26931248093023896f, 0.63257448654621840f, 0.40519540151581168f, 0.91847078315913677f,\
                    0.04373343335464597f, 0.48185089323669672f, 0.26395560009405017f, 0.41481037065386772f,\
                    0.28064980218186975f, 0.12800584640353918f, 0.77831285959109664f, 0.21190304495394230f,\
                    0.11213546665385365f, 0.68568959552794695f, 0.15312166837975383f, 0.69708506017923355f}


#define CREAL_IN32 { 0.21132486546412110f, 0.75604385416954756f, 0.00022113462910056f, 0.33032709173858166f,\
                     0.66538110421970487f, 0.62839178834110498f, 0.84974523587152362f, 0.68573101982474327f,\
                     0.87821648130193353f, 0.06837403681129217f, 0.56084860628470778f, 0.66235693730413914f,\
                     0.72635067673400044f, 0.19851438421756029f, 0.54425731627270579f, 0.23207478970289230f,\
                     0.23122371966019273f, 0.21646326314657927f, 0.88338878145441413f, 0.65251349471509457f,\
                     0.30760907428339124f, 0.93296162132173777f, 0.21460078610107303f, 0.31264199689030647f,\
                     0.36163610080257058f, 0.2922266637906432f , 0.56642488157376647f, 0.48264719732105732f,\
                     0.33217189135029912f, 0.59350947011262178f, 0.50153415976092219f, 0.43685875833034515f}

#define CIMAG_IN32 { 0.26931248093023896f, 0.63257448654621840f, 0.40519540151581168f, 0.91847078315913677f,\
                     0.04373343335464597f, 0.48185089323669672f, 0.26395560009405017f, 0.41481037065386772f,\
                     0.28064980218186975f, 0.12800584640353918f, 0.77831285959109664f, 0.21190304495394230f,\
                     0.11213546665385365f, 0.68568959552794695f, 0.15312166837975383f, 0.69708506017923355f,\
                     0.84155184263363481f, 0.40620247554033995f, 0.40948254754766822f, 0.87841258011758327f,\
                     0.11383596854284406f, 0.19983377400785685f, 0.56186607433483005f, 0.58961773291230202f,\
                     0.68539796629920602f, 0.89062247332185507f, 0.50422128057107329f, 0.34936154074966908f,\
                     0.38737787725403905f, 0.92228986788541079f, 0.94881842611357570f, 0.34353372454643250f}




#define CREAL_RESULT2 {   0.16527411318384111f , - 0.16505297855474055f   }


#define CIMAG_RESULT2 {+ 0.64688644628040493f, + 0.01849465793929994f}



#define CREAL_RESULT3 {  1.29709824733436108f, 0.94154648753137238f, 0.2434220389437592f}

#define CIMAG_RESULT3 { +0.59717983752489090f,+0.61999805178742728f,+0.93089271522808925f}





#define CREAL_RESULT4 {  0.62051669345237315f,-0.12307844962924719f, 0.24346416513435543f, 0.10884282691404223f}

#define CIMAG_RESULT4 { +0.53701765113510191f,+0.11296372814103961f,+0.10658199037425220f,-0.19571476336568594f}



#define CREAL_RESULT5 {  0.62691605370491743f, 0.01919730450714778f, 0.10104198714906780f,-0.07720389203270116f,\
 0.17979378254309186f}

#define CIMAG_RESULT5 { +0.54289909722283480f,+0.13998734162926388f,-0.17962778177138733f,+0.12064468327985833f,\
-0.06305473407586182f}



#define CREAL_RESULT6 {  0.36402360963014263f, 0.09605967189946550f, 0.01914935343131208f, 0.11197456725252172f,\
-0.00281267335181221f,-0.04413721258892397f}

#define CIMAG_RESULT6 { +0.39017331654516357f,-0.08647381537197443f,-0.01847428942485585f,-0.00928606038602692f,\
-0.11828530217444963f,+0.05694693691321624f}





#define CREAL_RESULT7 {  0.43821863417646711f, 0.04570283341490348f, 0.00786143114706625f, 0.09081035877037001f,\
                         0.06907620238256847f,-0.01879476197071369f,-0.08861738164795580f}

#define CIMAG_RESULT7 { +0.39293177811694990f,-0.13474546775513907f,+0.02350839028687007f,-0.01818990984988529f,\
                        -0.03304492053033681f,-0.13370336175502612f,+0.11784427758764042f}




#define CREAL_RESULT8 {  0.50006150756962597f, 0.02175013240250377f, 0.12240753043442965f,-0.02322026233124427f,\
-0.00844178465195000f,-0.11389905106099872f, 0.09979579551145434f,-0.05419655160111495f}

#define CIMAG_RESULT8 { +0.39448237349279225f,-0.14241475898929634f,-0.02220236556604505f,-0.14652524166940226f,\
-0.02577395853586495f,+0.03536239646532335f,+0.04400678444653749f,+0.07766555645702852f}




#define CREAL_RESULT9 {  0.4653623884336816f ,-0.10996977634919125f,-0.00630644375731721f,-0.04060246287025721f,\
 0.05408191850705556f, 0.01600258566680582f,-0.05864725873797094f,-0.05201672319102815f,\
-0.03668050804158550f}

#define CIMAG_RESULT9 { +0.47890158897886670f,+0.08218728720967677f,-0.06845717881236174f,-0.04807038546781070f,\
-0.08014536882440566f,-0.2030002369914648f,+0.22453417104910142f,-0.06865385510029860f, \
-0.04798354111106445f}




#define CREAL_RESULT10 {  0.47991688111796976f,-0.06991181823164128f,-0.08469577602979275f, 0.07318652166556244f,\
-0.07052463321312863f, 0.03129850449040533f,-0.12994121698152189f,-0.09609430997689952f,\
 0.06979737512491652f, 0.01343173518070941f}

#define CIMAG_RESULT10 { +0.38964297915808854f,+0.03225815718776807f,-0.11005045339360417f,+0.07000358882571070f,\
-0.03882538926347270f,+0.01466467357240619f,+0.09131179012889759f,-0.02808828423670948f,\
-0.11655017979588442f,-0.07314316252300754f}





#define CREAL_RESULT11 {  0.40752098586579621f, 0.01832270994676578f, 0.00727102452352803f,-0.13808643998727593f,\
 0.03761367031409660f, 0.01913753006534984f, 0.14829738766302858f,-0.03282455871021355f,\
 0.08531354288243319f,-0.01850240581569137f,-0.03471406311050802f}

#define CIMAG_RESULT11 { +0.54521434381604206f,+0.04263232089408980f,+0.01080182331995891f,-0.16507135273435664f,\
+0.08926061372379648f,+0.01327764965004020f,-0.10524284127370778f,-0.02743692407216465f,\
+0.01530157234736101f,-0.10904555975048559f,+0.08146574602618931f}



#define CREAL_RESULT12 {  0.56766566393586493f,-0.02954179400241778f, 0.03231997586448970f,-0.08061527988562983f,\
 0.12393588887272257f,-0.00005011576253769f,-0.04579487590429687f, 0.18096695473171109f,\
-0.02460040668391173f, 0.04301438496137660f, 0.06736119091227670f,-0.07861773287010031f}

#define CIMAG_RESULT12 { +0.42399044404737651f,-0.08988211863004507f,-0.06417895954194777f,-0.05903535817439356f,\
-0.03585983457125090f,-0.08163805724791348f,+0.00020448095165193f,-0.00326952155569338f,\
-0.06391751330264545f,+0.08415352273732421f,+0.16527532188314309f,-0.07732802237804590f}

#define CREAL_RESULT13 {  0.45819311546018493f, 0.04715640399909302f,-0.02841189190012001f, 0.01471405402274759f,\
-0.06955117696838889f, 0.08520673650173283f,-0.08379007722081511f,-0.06164570839841994f,\
-0.07511713555210725f, 0.07790018146444697f, 0.05666948441941852f,-0.04832646187709128f,\
-0.08077086016003779f}

#define CIMAG_RESULT13 { +0.41406970618006128f,-0.00941536236456233f,-0.04150925434362464f,+0.07093289809698045f,\
+0.05642954059283156f,-0.13856574403878158f,-0.06657177623345527f,+0.08978984633083976f,\
-0.03246486275259956f,+0.03475196544714984f,-0.02951956674329188f,-0.08259093889333179f,\
-0.00138085118416531f}




#define CREAL_RESULT14 {  0.55365722569903086f,-0.03431117289603670f,-0.07434176530056802f, 0.09479458498406634f,\
 0.04225809799112697f, 0.10711066217766220f, 0.09039926979534348f, 0.04258158766398475f,\
-0.06766097168605688f, 0.02983162597647674f, 0.16148055537460848f, 0.00306446154680962f,\
-0.01447548152915653f,-0.05597609967970790f}

#define CIMAG_RESULT14 { +0.41925417428969269f,-0.02346521727072965f,-0.07537174377902310f,+0.12152793513158673f,\
-0.05435563225189670f,+0.05588517124820697f,+0.03948028804374615f,-0.02547196042723954f,\
+0.04750412291672844f,+0.09081187301248270f,+0.02410149252380316f,-0.00728204510680848f,\
+0.07924950091433607f,+0.04222609705593354f}




#define CREAL_RESULT15 {  0.60260595980410780f,-0.09025313009309296f, 0.07296362400098141f,-0.01312199542974801f,\
 0.05196795946996628f, 0.00939805486763355f, 0.07359039581140392f, 0.07244754050834930f,\
-0.13409874398389474f,-0.06419327756841406f,-0.04062806443214259f, 0.19283708306652805f,\
 0.04903632944886174f,-0.00831723960417976f, 0.05579715986245712f}

#define CIMAG_RESULT15 { +0.52861572646846366f,-0.09479760020968324f,+0.06725385149296267f,+0.01462043430946725f,\
-0.00503137552191604f,-0.05388544405841567f,-0.05301311586381348f,-0.11414862867731596f,\
+0.0617906214666224f ,+0.14723827146724461f,-0.10948893804382542f,+0.00343655851962967f,\
-0.00538333435303664f,-0.03924473093854008f,+0.06244360380584638f}




#define CREAL_RESULT16 {  0.45740074128843844f,-0.00119937109332679f,-0.09568979823192530f,-0.01738371075123765f,\
-0.03869746159762144f,-0.05391866381953740f, 0.12416191581397301f,-0.03041585422893309f,\
-0.03257706691510975f, 0.07000836728092155f,-0.00597921899357861f,-0.03766276570110767f,\
-0.07796601625159383f,-0.03039291932884991f,-0.03422318488120085f, 0.03575872707088203f}

#define CIMAG_RESULT16 { +0.40480042458511889f,+0.06953094033520663f,+0.02374777548578576f,-0.03393710545080092f,\
-0.04651592345908284f,+0.02783744013794178f,-0.03035825342604098f,-0.06447129475682091f,\
-0.11649833549745381f,-0.11413977271896644f,+0.11722373704793855f,+0.08102030470498481f,\
-0.06532836984843016f,+0.00779635766618338f,-0.01208991333178105f,+0.02069446945645629f}





#define CREAL_RESULT32 { 0.47864284948445857f, 0.05482834035731039f,-0.01530773095660632f,-0.01750083123738436f,\
                        -0.05071297103611097f, 0.08766815171372286f, 0.02106608347521239f,-0.05109320958418949f,\
                        -0.01096817385405302f, 0.02416709596518544f,-0.05098444057349576f,-0.01448958336761648f,\
                         0.02532074256370129f,-0.09984237842467819f,-0.06358972398280244f,-0.04611519259707146f,\
                         0.01104057650081813f,-0.03475148917606731f, 0.00813855773340646f,-0.00568704237169753f,\
                         0.01830285230723486f, 0.02749700538425181f,-0.04780122330152622f, 0.06632936566012661f,\
                        -0.01447601290419698f, 0.03116875276343186f, 0.00123360692547217f, 0.01833898177168673f,\
                        -0.03654957125464739f, 0.00547684889715929f,-0.05208112856470785f,-0.05594424285220599f}



#define CIMAG_RESULT32 {+0.48466352955438197f,+0.04862008402656083f,+0.04890918577469192f,-0.07661834259188156f,\
                        +0.01684868035424048f,-0.06277273594077343f,-0.03536555772325747f,-0.04086803002308132f,\
                        -0.04373891325667500f,+0.03378928555068783f,+0.09272389885362962f,-0.01252633931303314f,\
                        +0.04715379187194862f,+0.04212975779784539f,+0.00924226411866865f,-0.06146319342441199f,\
                        -0.06222798605449498f,+0.03846733709242996f,-0.06362612522976926f,+0.02110331414670667f,\
                        +0.06568727290671772f,-0.09854847704278780f,+0.04828755172931871f,-0.06711471562709365f,\
                        -0.03694727551192045f,-0.04323841461679329f,-0.07059643871085296f,-0.01972938456264230f,\
                        +0.04778892314703904f,+0.02763841783182228f,+0.00662935995827029f,-0.01498824415525237f}
static void cifftmaTest2 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN2;
    float tImagIn [] = CIMAG_IN2 ;



    float tRealResult [] = CREAL_RESULT2 ;
    float tImagResult [] = CIMAG_RESULT2 ;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS2));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS2 );
    floatComplex* Result   = FloatComplexMatrix ( tRealResult , tImagResult ,ROW*COLS2) ;



    cifftma ( in , ROW , COLS2 , out ) ;

    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/


	for ( i = 0 ; i < (ROW*COLS2 )  ; i++ )
	{
	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
                i ,
                creals(out[i]) ,
                cimags(out[i]),
                creals (Result[i])  ,
                cimags (Result[i]),
                fabs(  creals(out[i]) -  creals (Result[i]) ) / fabs (creals (out[i])) ,
                fabs(  cimags(out[i]) -  cimags (Result[i]) ) / fabs (cimags (out[i])));

   if (  creals(out[i])  < 1e-14 && creals (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  creals(out[i]) -  creals (Result[i]) ) / fabs (creals (out[i]))  < 3e-6 );


    if (  cimags(out[i])  < 1e-14 && cimags (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  cimags(out[i]) -  cimags (Result[i]) ) / fabs (cimags (out[i]))  < 3e-6 ) ;

    }

    free(out);
    free(in);
    free(Result);

}

static void cifftmaTest3 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN3;
    float tImagIn [] = CIMAG_IN3 ;



    float tRealResult [] = CREAL_RESULT3;
    float tImagResult [] = CIMAG_RESULT3 ;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS3));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS3 );
    floatComplex* Result   = FloatComplexMatrix ( tRealResult , tImagResult ,ROW*COLS3) ;



    cifftma ( in , ROW , COLS3 , out ) ;

    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/


	for ( i = 0 ; i < (ROW*COLS3 )  ; i++ )
	{
	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
                i ,
                creals(out[i]) ,
                cimags(out[i]),
                creals (Result[i])  ,
                cimags (Result[i]),
                fabs(  creals(out[i]) -  creals (Result[i]) ) / fabs (creals (out[i])) ,
                fabs(  cimags(out[i]) -  cimags (Result[i]) ) / fabs (cimags (out[i]))
		 );
/*
	   if (  creals(out[i])  < 1e-14 && creals (Result[i]) < 1e-18 )
		assert ( 1 ) ;
	    else
		assert ( fabs(  creals(out[i]) -  creals (Result[i]) ) / fabs (creals (out[i]))  < 3e-6 );


	    if (  cimags(out[i])  < 1e-14 && cimags (Result[i]) < 1e-18 )
		assert ( 1 ) ;
	    else
		    assert ( fabs(  cimags(out[i]) -  cimags (Result[i]) ) / fabs (cimags (out[i]))  < 3e-6 ) ;
*/  
  	}

    free(out);
    free(in);
    free(Result);

}


static void cifftmaTest4 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN4;
    float tImagIn [] = CIMAG_IN4 ;



    float tRealResult [] = CREAL_RESULT4 ;
    float tImagResult [] = CIMAG_RESULT4 ;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS4));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS4 );
    floatComplex* Result   = FloatComplexMatrix ( tRealResult , tImagResult ,ROW*COLS4) ;


    cifftma ( in , ROW , COLS4 , out ) ;

    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/
	for ( i = 0 ; i < (ROW*COLS4 )  ; i++ )
	{
	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
              i ,creals(out[i]) , cimags(out[i]), creals (Result[i])  , cimags (Result[i]),
              fabs(  creals(out[i]) -  creals (Result[i]) ) / fabs (creals (out[i])) ,
              fabs(  cimags(out[i]) -  cimags (Result[i]) ) / fabs (cimags (out[i])));
   if (  creals(out[i])  < 1e-14 && creals (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  creals(out[i]) -  creals (Result[i]) ) / fabs (creals (out[i]))  < 3e-6 );


    if (  cimags(out[i])  < 1e-14 && cimags (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  cimags(out[i]) -  cimags (Result[i]) ) / fabs (cimags (out[i]))  < 3e-6 ) ;
    }
    free(out);
    free(in);
    free(Result);

}


static void cifftmaTest5 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN5;
    float tImagIn [] = CIMAG_IN5 ;



    float tRealResult [] = CREAL_RESULT5;
    float tImagResult [] = CIMAG_RESULT5 ;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS5));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS5 );
    floatComplex* Result   = FloatComplexMatrix ( tRealResult , tImagResult ,ROW*COLS5) ;



    cifftma ( in , ROW , COLS5 , out ) ;

    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/


	for ( i = 0 ; i < (ROW*COLS5 )  ; i++ )
	{
	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
                i ,
                creals(out[i]) ,
                cimags(out[i]),
                creals (Result[i])  ,
                cimags (Result[i]),
                fabs(  creals(out[i]) -  creals (Result[i]) ) / fabs (creals (out[i])) ,
                fabs(  cimags(out[i]) -  cimags (Result[i]) ) / fabs (cimags (out[i])));
/*
   if (  creals(out[i])  < 1e-14 && creals (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  creals(out[i]) -  creals (Result[i]) ) / fabs (creals (out[i]))  < 3e-6 );


    if (  cimags(out[i])  < 1e-14 && cimags (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  cimags(out[i]) -  cimags (Result[i]) ) / fabs (cimags (out[i]))  < 3e-6 ) ;
*/
    }
    free(out);
    free(in);
    free(Result);

}


static void cifftmaTest6 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN6;
    float tImagIn [] = CIMAG_IN6 ;



    float tRealResult [] = CREAL_RESULT6;
    float tImagResult [] = CIMAG_RESULT6;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS6));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS6 );
    floatComplex* Result   = FloatComplexMatrix ( tRealResult , tImagResult ,ROW*COLS6) ;



    cifftma ( in , ROW , COLS6 , out ) ;

    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/


	for ( i = 0 ; i < (ROW*COLS6 )  ; i++ )
	{
	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
                i ,
                creals(out[i]) ,
                cimags(out[i]),
                creals (Result[i])  ,
                cimags (Result[i]),
                fabs(  creals(out[i]) -  creals (Result[i]) ) / fabs (creals (out[i])) ,
                fabs(  cimags(out[i]) -  cimags (Result[i]) ) / fabs (cimags (out[i])));
/*
   if (  creals(out[i])  < 1e-14 && creals (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  creals(out[i]) -  creals (Result[i]) ) / fabs (creals (out[i]))  < 3e-6 );


    if (  cimags(out[i])  < 1e-14 && cimags (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  cimags(out[i]) -  cimags (Result[i]) ) / fabs (cimags (out[i]))  < 3e-6 ) ;
*/
    }
    free(out);
    free(in);
    free(Result);

}


static void cifftmaTest7 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN7;
    float tImagIn [] = CIMAG_IN7 ;



    float tRealResult [] = CREAL_RESULT7;
    float tImagResult [] = CIMAG_RESULT7;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS7));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS7 );
    floatComplex* Result   = FloatComplexMatrix ( tRealResult , tImagResult ,ROW*COLS7) ;



    cifftma ( in , ROW , COLS7 , out ) ;

    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/


	for ( i = 0 ; i < (ROW*COLS7 )  ; i++ )
	{
	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
                i ,
                creals(out[i]) ,
                cimags(out[i]),
                creals (Result[i])  ,
                cimags (Result[i]),
                fabs(  creals(out[i]) -  creals (Result[i]) ) / fabs (creals (out[i])) ,
                fabs(  cimags(out[i]) -  cimags (Result[i]) ) / fabs (cimags (out[i])));
/*
   if (  creals(out[i])  < 1e-14 && creals (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  creals(out[i]) -  creals (Result[i]) ) / fabs (creals (out[i]))  < 3e-6 );


    if (  cimags(out[i])  < 1e-14 && cimags (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  cimags(out[i]) -  cimags (Result[i]) ) / fabs (cimags (out[i]))  < 3e-6 ) ;
*/
    }
    free(out);
    free(in);
    free(Result);

}

static void cifftmaTest8 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN8;
    float tImagIn [] = CIMAG_IN8 ;



    float tRealResult [] = CREAL_RESULT8 ;
    float tImagResult [] = CIMAG_RESULT8 ;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS8));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS8 );
    floatComplex* Result   = FloatComplexMatrix ( tRealResult , tImagResult ,ROW*COLS8) ;


    cifftma ( in , ROW , COLS8 , out ) ;

    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/
	for ( i = 0 ; i < (ROW*COLS8 )  ; i++ )
	{
	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
              i ,creals(out[i]) , cimags(out[i]), creals (Result[i])  , cimags (Result[i]),
              fabs(  creals(out[i]) -  creals (Result[i]) ) / fabs (creals (out[i])) ,
              fabs(  cimags(out[i]) -  cimags (Result[i]) ) / fabs (cimags (out[i])));

   if (  creals(out[i])  < 1e-14 && creals (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  creals(out[i]) -  creals (Result[i]) ) / fabs (creals (out[i]))  < 3e-6 );


    if (  cimags(out[i])  < 1e-14 && cimags (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  cimags(out[i]) -  cimags (Result[i]) ) / fabs (cimags (out[i]))  < 3e-6 ) ;

    }
    free(out);
    free(in);
    free(Result);

}


static void cifftmaTest9 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN9;
    float tImagIn [] = CIMAG_IN9 ;



    float tRealResult [] = CREAL_RESULT9 ;
    float tImagResult [] = CIMAG_RESULT9 ;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS9));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS9 );
    floatComplex* Result   = FloatComplexMatrix ( tRealResult , tImagResult ,ROW*COLS9) ;


    cifftma ( in , ROW , COLS9 , out ) ;

    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/
	for ( i = 0 ; i < (ROW*COLS9 )  ; i++ )
	{
	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
              i ,creals(out[i]) , cimags(out[i]), creals (Result[i])  , cimags (Result[i]),
              fabs(  creals(out[i]) -  creals (Result[i]) ) / fabs (creals (out[i])) ,
              fabs(  cimags(out[i]) -  cimags (Result[i]) ) / fabs (cimags (out[i])));
/*
   if (  creals(out[i])  < 1e-14 && creals (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  creals(out[i]) -  creals (Result[i]) ) / fabs (creals (out[i]))  < 3e-6 );


    if (  cimags(out[i])  < 1e-14 && cimags (Result[i]) < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  cimags(out[i]) -  cimags (Result[i]) ) / fabs (cimags (out[i]))  < 3e-6 ) ;
*/
    }
    free(out);
    free(in);
    free(Result);

}

static void cifftmaTest10 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN10;
    float tImagIn [] = CIMAG_IN10 ;



    float tRealResult [] = CREAL_RESULT10 ;
    float tImagResult [] = CIMAG_RESULT10 ;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS10));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS10 );



    cifftma ( in , ROW , COLS10 , out );




    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/
		for ( i = 0 ; i < (ROW*COLS10 )  ; i++ )
	{


	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
                i ,
                creals(out[i]) ,
                cimags(out[i]),
                tRealResult[i]  ,
                tImagResult[i],
                fabs(  creals(out[i]) -  tRealResult[i] ) / fabs (creals (out[i])) ,
                fabs(  cimags(out[i]) -  tImagResult[i] ) / fabs (cimags (out[i])));

/*
   if (  creals(out[i])  < 1e-14 && tRealResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  creals(out[i]) -  tRealResult[i] ) / fabs (creals (out[i]))  < 3e-6 );


    if (  cimags(out[i])  < 1e-14 && tImagResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  cimags(out[i]) -  tImagResult[i] ) / fabs (cimags (out[i]))  < 3e-6 ) ;
*/
    }
    free(out);
    free(in);
}

static void cifftmaTest11 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN11;
    float tImagIn [] = CIMAG_IN11 ;



    float tRealResult [] = CREAL_RESULT11 ;
    float tImagResult [] = CIMAG_RESULT11 ;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS11));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS11 );



    cifftma ( in , ROW , COLS11 , out );




    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/
		for ( i = 0 ; i < (ROW*COLS11 )  ; i++ )
	{


	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
                i ,
                creals(out[i]) ,
                cimags(out[i]),
                tRealResult[i]  ,
                tImagResult[i],
                fabs(  creals(out[i]) -  tRealResult[i] ) / fabs (creals (out[i])) ,
                fabs(  cimags(out[i]) -  tImagResult[i] ) / fabs (cimags (out[i])));


   if (  creals(out[i])  < 1e-14 && tRealResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  creals(out[i]) -  tRealResult[i] ) / fabs (creals (out[i]))  < 3e-6 );


    if (  cimags(out[i])  < 1e-14 && tImagResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  cimags(out[i]) -  tImagResult[i] ) / fabs (cimags (out[i]))  < 3e-6 ) ;

    }
    free(out);
    free(in);

}


static void cifftmaTest12 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN12;
    float tImagIn [] = CIMAG_IN12 ;



    float tRealResult [] = CREAL_RESULT12 ;
    float tImagResult [] = CIMAG_RESULT12 ;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS12));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS12 );



    cifftma ( in , ROW , COLS12 , out );




    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/
		for ( i = 0 ; i < (ROW*COLS12 )  ; i++ )
	{


	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
                i ,
                creals(out[i]) ,
                cimags(out[i]),
                tRealResult[i]  ,
                tImagResult[i],
                fabs(  creals(out[i]) -  tRealResult[i] ) / fabs (creals (out[i])) ,
                fabs(  cimags(out[i]) -  tImagResult[i] ) / fabs (cimags (out[i])));


   if (  creals(out[i])  < 1e-14 && tRealResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  creals(out[i]) -  tRealResult[i] ) / fabs (creals (out[i]))  < 3e-6 );


    if (  cimags(out[i])  < 1e-14 && tImagResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  cimags(out[i]) -  tImagResult[i] ) / fabs (cimags (out[i]))  < 3e-5 ) ;

    }
    free(out);
    free(in);

}


static void cifftmaTest13 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN13;
    float tImagIn [] = CIMAG_IN13 ;



    float tRealResult [] = CREAL_RESULT13 ;
    float tImagResult [] = CIMAG_RESULT13 ;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS13));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS13 );



    cifftma ( in , ROW , COLS13 , out );




    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/
		for ( i = 0 ; i < (ROW*COLS13 )  ; i++ )
	{


	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
                i ,
                creals(out[i]) ,
                cimags(out[i]),
                tRealResult[i]  ,
                tImagResult[i],
                fabs(  creals(out[i]) -  tRealResult[i] ) / fabs (creals (out[i])) ,
                fabs(  cimags(out[i]) -  tImagResult[i] ) / fabs (cimags (out[i])));


   if (  creals(out[i])  < 1e-14 && tRealResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  creals(out[i]) -  tRealResult[i] ) / fabs (creals (out[i]))  < 3e-6 );


    if (  cimags(out[i])  < 1e-14 && tImagResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  cimags(out[i]) -  tImagResult[i] ) / fabs (cimags (out[i]))  < 3e-6 ) ;

    }
    free(out);
    free(in);

}


static void cifftmaTest14 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN14;
    float tImagIn [] = CIMAG_IN14 ;



    float tRealResult [] = CREAL_RESULT14 ;
    float tImagResult [] = CIMAG_RESULT14 ;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS14));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS14 );



    cifftma ( in , ROW , COLS14 , out );




    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/
		for ( i = 0 ; i < (ROW*COLS14 )  ; i++ )
	{


	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
                i ,
                creals(out[i]) ,
                cimags(out[i]),
                tRealResult[i]  ,
                tImagResult[i],
                fabs(  creals(out[i]) -  tRealResult[i] ) / fabs (creals (out[i])) ,
                fabs(  cimags(out[i]) -  tImagResult[i] ) / fabs (cimags (out[i])));


   if (  creals(out[i])  < 1e-14 && tRealResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  creals(out[i]) -  tRealResult[i] ) / fabs (creals (out[i]))  < 3e-6 );


    if (  cimags(out[i])  < 1e-14 && tImagResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  cimags(out[i]) -  tImagResult[i] ) / fabs (cimags (out[i]))  < 3e-6 ) ;

    }
    free(out);
    free(in);

}



static void cifftmaTest15 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN15;
    float tImagIn [] = CIMAG_IN15 ;



    float tRealResult [] = CREAL_RESULT15 ;
    float tImagResult [] = CIMAG_RESULT15 ;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS15));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS15 );



    cifftma ( in , ROW , COLS15 , out );




    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/
		for ( i = 0 ; i < (ROW*COLS15 )  ; i++ )
	{


	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
                i ,
                creals(out[i]) ,
                cimags(out[i]),
                tRealResult[i]  ,
                tImagResult[i],
                fabs(  creals(out[i]) -  tRealResult[i] ) / fabs (creals (out[i])) ,
                fabs(  cimags(out[i]) -  tImagResult[i] ) / fabs (cimags (out[i])));


   if (  creals(out[i])  < 1e-14 && tRealResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  creals(out[i]) -  tRealResult[i] ) / fabs (creals (out[i]))  < 3e-6 );


    if (  cimags(out[i])  < 1e-14 && tImagResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  cimags(out[i]) -  tImagResult[i] ) / fabs (cimags (out[i]))  < 3e-6 ) ;

    }
    free(out);
    free(in);

}



static void cifftmaTest16 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN16;
    float tImagIn [] = CIMAG_IN16 ;



    float tRealResult [] = CREAL_RESULT16 ;
    float tImagResult [] = CIMAG_RESULT16 ;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS16));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS16 );



    cifftma ( in , ROW , COLS16 , out );




    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/
		for ( i = 0 ; i < (ROW*COLS16 )  ; i++ )
	{


	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
                i ,
                creals(out[i]) ,
                cimags(out[i]),
                tRealResult[i]  ,
                tImagResult[i],
                fabs(  creals(out[i]) -  tRealResult[i] ) / fabs (creals (out[i])) ,
                fabs(  cimags(out[i]) -  tImagResult[i] ) / fabs (cimags (out[i])));


   if (  creals(out[i])  < 1e-14 && tRealResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  creals(out[i]) -  tRealResult[i] ) / fabs (creals (out[i]))  < 3e-6 );


    if (  cimags(out[i])  < 1e-14 && tImagResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  cimags(out[i]) -  tImagResult[i] ) / fabs (cimags (out[i]))  < 3e-6 ) ;

    }
    free(out);
    free(in);

}



static void cifftmaTest32 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN32;
    float tImagIn [] = CIMAG_IN32 ;



    float tRealResult [] = CREAL_RESULT32 ;
    float tImagResult [] = CIMAG_RESULT32 ;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS32));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS32 );



    cifftma ( in , ROW , COLS32 , out );




    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/
		for ( i = 0 ; i < (ROW*COLS32 )  ; i++ )
	{


	  printf ( "\t\t %d out : %e\t %e\t * i result : %e\t %e\t * i assert : : %e\t %e\t * i  \n" ,
                i ,
                creals(out[i]) ,
                cimags(out[i]),
                tRealResult[i]  ,
                tImagResult[i],
                fabs(  creals(out[i]) -  tRealResult[i] ) / fabs (creals (out[i])) ,
                fabs(  cimags(out[i]) -  tImagResult[i] ) / fabs (cimags (out[i])));


   if (  creals(out[i])  < 1e-14 && tRealResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
        assert ( fabs(  creals(out[i]) -  tRealResult[i] ) / fabs (creals (out[i]))  < 3e-5 );


    if (  cimags(out[i])  < 1e-14 && tImagResult[i] < 1e-18 )
        assert ( 1 ) ;
    else
	    assert ( fabs(  cimags(out[i]) -  tImagResult[i] ) / fabs (cimags (out[i]))  < 3e-6 ) ;

    }
    free(out);
    free(in);

}

static int testFft(void) {

  printf("\n>>>> FFT Tests\n");
  printf("\t>>>> Matrix  Float Realt Tests\n");
  /*dfftmaTest();*/

  printf("\n\n\n");

  printf("\n\t>>>> Vector 2 Float Complex Tests\n");
  cifftmaTest2();
  printf("\n\t>>>> Vector 3 Float Complex Tests\n");
  cifftmaTest3();
  printf("\n\t>>>> Vector 4 Float Complex Tests\n");
  cifftmaTest4();
  printf("\n\t>>>> Vector 5 Float Complex Tests\n");
  cifftmaTest5();
  printf("\n\t>>>> Vector 6 Float Complex Tests\n");
  cifftmaTest6();
  printf("\n\t>>>> Vector 7 Float Complex Tests\n");
  cifftmaTest7();
  printf("\n\t>>>> Vector 8 Float Complex Tests\n");
  cifftmaTest8();
  printf("\n\t>>>> Vector 9 Float Complex Tests\n");
  cifftmaTest9();
  printf("\n\t>>>> Vector 10 Float Complex Tests\n");
  cifftmaTest10();
  printf("\n\t>>>> Vector 11 Float Complex Tests\n");
  cifftmaTest11();
  printf("\n\t>>>> Vector 12 Float Complex Tests\n");
  cifftmaTest12();
  printf("\n\t>>>> Vector 13 Float Complex Tests\n");
  cifftmaTest13();
  printf("\n\t>>>> Vector 14 Float Complex Tests\n");
  cifftmaTest14();
  printf("\n\t>>>> Vector 14 Float Complex Tests\n");
  cifftmaTest15();
  printf("\n\t>>>> Vector 16 Float Complex Tests\n");
  cifftmaTest16();
  printf("\n\t>>>> Vector 32 Float Complex Tests\n");
  cifftmaTest32();
  return 0;
}



int main(void) {
  assert(testFft() == 0);
  return 0;
}


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
#include "fft.h"


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




#define CREAL_IN11 {0.49934938363730907f,0.26385784195736051f,0.52535630855709314f,0.53762298030778766f,\
                    0.11999255046248436f,0.2256303490139544f ,0.62740930821746588f,0.76084325974807143f,\
                    0.04855662025511265f,0.67239497276023030f,0.20171726960688829f }

#define CIMAG_IN11 {0.39115739194676280f,0.83003165572881699f,0.58787201577797532f,0.48291792999953032f,\
                    0.22328650346025825f,0.84008856676518917f,0.12059959070757031f,0.28553641680628061f,\
                    0.86075146449729800f,0.84941016510128975f,0.52570608118548989f}


#define CREAL_IN12 {0.75604385416954756f,0.00022113462910056f,0.33032709173858166f,0.66538110421970487f,\
                    0.62839178834110498f,0.84974523587152362f,0.68573101982474327f,0.87821648130193353f,\
                    0.06837403681129217f,0.56084860628470778f,0.66235693730413914f,0.72635067673400044f}

#define CIMAG_IN12 {0.19851438421756029f,0.54425731627270579f,0.23207478970289230f,0.23122371966019273f,\
                    0.21646326314657927f,0.88338878145441413f,0.65251349471509457f,0.30760907428339124f,\
                    0.93296162132173777f,0.21460078610107303f,0.31264199689030647f,0.36163610080257058f}


#define CREAL_IN13 {0.2922266637906432f ,0.56642488157376647f,0.48264719732105732f,0.33217189135029912f,\
                    0.59350947011262178f,0.50153415976092219f,0.43685875833034515f,0.26931248093023896f,\
                    0.63257448654621840f,0.40519540151581168f,0.91847078315913677f,0.04373343335464597f,\
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


#define CREAL_IN32 {0.21132486546412110f,0.75604385416954756f,0.00022113462910056f,0.33032709173858166f,\
                    0.66538110421970487f,0.62839178834110498f,0.84974523587152362f,0.68573101982474327f,\
                    0.87821648130193353f,0.06837403681129217f,0.56084860628470778f,0.66235693730413914f,\
                    0.72635067673400044f,0.19851438421756029f,0.54425731627270579f,0.23207478970289230f,\
                    0.23122371966019273f,0.21646326314657927f,0.88338878145441413f,0.65251349471509457f,\
                    0.30760907428339124f,0.93296162132173777f,0.21460078610107303f,0.31264199689030647f,\
                    0.36163610080257058f,0.2922266637906432f ,0.56642488157376647f,0.48264719732105732f,\
                    0.33217189135029912f,0.59350947011262178f,0.50153415976092219f,0.43685875833034515f}

#define CIMAG_IN32 {0.26931248093023896f,0.63257448654621840f,0.40519540151581168f,0.91847078315913677f,\
                    0.04373343335464597f,0.48185089323669672f,0.26395560009405017f,0.41481037065386772f,\
                    0.28064980218186975f,0.12800584640353918f,0.77831285959109664f,0.21190304495394230f,\
                    0.11213546665385365f,0.68568959552794695f,0.15312166837975383f,0.69708506017923355f,\
                    0.84155184263363481f,0.40620247554033995f,0.40948254754766822f,0.87841258011758327f,\
                    0.11383596854284406f,0.19983377400785685f,0.56186607433483005f,0.58961773291230202f,\
                    0.68539796629920602f,0.89062247332185507f,0.50422128057107329f,0.34936154074966908f,\
                    0.38737787725403905f,0.92228986788541079f,0.94881842611357570f,0.34353372454643250f}




#define CREAL_RESULT2 { 0.33054822636768222f,- 0.33010595710948110f}
#define CIMAG_RESULT2 { 1.29377289256080985f,  0.03698931587859988f}

#define CREAL_RESULT3 { 3.8912947420030832f ,  0.73026611683127762f, 2.82463946259411713f}
#define CIMAG_RESULT3 { 1.79153951257467270f,  2.79267814568426775f, 1.85999415536228230f}

#define CREAL_RESULT4 { 2.48206677380949259f,  0.43537130765616894f, 0.97385666053742170f, -0.49231379851698875f}
#define CIMAG_RESULT4 { 2.14807060454040766f,- 0.78285905346274376f, 0.42632796149700880f,  0.45185491256415844f}

#define CREAL_RESULT5 { 3.13458026852458715f, 0.8989689127154592f ,-0.38601946016350575f, 0.50520993574533923f,\
                        0.09598652253573875f}
#define CIMAG_RESULT5 { 2.71449548611417413f,-0.31527367037930898f, 0.60322341639929178f,-0.89813890885693670f,\
                        0.69993670814631914f}

#define CREAL_RESULT6 {  2.18414165778085589f,-0.26482327553354379f,-0.01687604011087318f, 0.67184740351513028f,\
                         0.11489612058787246f, 0.57635803139679309f }
#define CIMAG_RESULT6 { +2.34103989927098155f,+0.34168162147929737f,-0.70971181304669773f,-0.05571636231616137f,\
                        -0.11084573654913504f,-0.51884289223184654f }




#define CREAL_RESULT7 { 3.06753043923527002f,-0.62032167153569062f,-0.13156333379499591f, 0.48353341667797933f,\
                        0.63567251139259018f, 0.05503001802946385f, 0.31991983390432432f}
#define CIMAG_RESULT7 { 2.75052244681864977f, 0.82490994311348309f,-0.93592353228518299f,-0.23131444371235776f,\
                       -0.12732936894919694f, 0.16455873200809046f,-0.94321827428597393f}



#define CREAL_RESULT8 { 4.00049206055700779f,-0.43357241280891956f, 0.79836636409163475f,-0.91119240848798977f,\
                       -0.06753427721560001f,-0.18576209864995416f, 0.97926024347543716f, 0.17400105922003017f}
#define CIMAG_RESULT8 { 3.15585898794233799f, 0.62132445165622818f, 0.35205427557229996f, 0.28289917172258683f,\
                       -0.20619166828691959f,-1.17220193335521805f,-0.17761892452836037f,-1.13931807191437073f }

#define CREAL_RESULT9 { 4.18826149590313435f,-0.33012457237426968f,-0.46815050871925312f,-0.52782532864173848f,\
                        0.14402327100125287f, 0.48673726656350014f,-0.36542216583231485f,-0.05675799381585508f,\
                       -0.98972798714272159f}
#define CIMAG_RESULT9 { 4.31011430080980062f,-0.43185186999958014f,-0.61788469590268758f, 2.02080753944191294f,\
                       -1.82700213292318292f,-0.72130831941965079f,-0.43263346921029644f,-0.61611460931125561f,\
                        0.73968558488709069f }


#define CREAL_RESULT10 {  4.7991688111796975f , 0.13431735180709442f, 0.69797375124916528f,-0.96094309976899528f,\
                         -1.299412169815219f  , 0.31298504490405327f,-0.70524633213128674f, 0.73186521665562432f,\
                         -0.84695776029792746f,-0.69911818231641265f}
#define CIMAG_RESULT10 { +3.8964297915808856f ,-0.73143162523007543f,-1.16550179795884423f,-0.28088284236709465f,\
                         +0.91311790128897607f,+0.14664673572406173f,-0.38825389263472715f,+0.70003588825710683f,\
                         -1.10050453393604197f,+0.32258157187768072f}

#define CREAL_RESULT11 { 4.48273084452375770f,-0.38185469421558837f,-0.20352646397260507f, 0.93844897170676500f,\
                        -0.36107014581234886f, 1.63127126429331426f, 0.21051283071884830f, 0.41375037345506283f,\
                        -1.51895083986003510f, 0.07998126975880851f, 0.20154980941442344f}

#define CIMAG_RESULT11 {+5.99735778197646141f,+0.89612320628808262f,-1.19950115725534112f,+0.16831729582097132f,\
                        -0.30180616479381106f,-1.1576712540107854f ,+0.14605414615044232f,+0.98186675096176135f,\
                        -1.81578488007792282f,+0.11882005651954808f,+0.46895552983498812f}

#define CREAL_RESULT12 { 6.81198796723037958f,-0.94341279444120363f, 0.80833429094732012f, 0.51617261953651883f,\
                        -0.2952048802069408f , 2.17160345678053313f,-0.54953851085156202f,-0.00060138915045171f,\
                         1.48723066647267177f,-0.96738335862755753f, 0.38783971037387632f,-0.35450152802901336f}

#define CIMAG_RESULT12 {+5.08788532856851816f,-0.92793626853655098f,+1.98330386259771707f,+1.00984227284789085f,\
                        -0.76701015963174557f,-0.03923425866831987f,+0.00245377141982317f,-0.97965668697496155f,\
                        -0.43031801485501076f,-0.70842429809272289f,-0.77014751450337293f,-1.07858542356054099f}


#define CREAL_RESULT13 { 5.95651050098240376f,-1.05002118208049122f,-0.62824400440218664f, 0.73670329745244079f,\
                         1.01270235903781081f,-0.97652276217739442f,-0.80139420917945892f,-1.08927100387059594f,\
                         1.10768757452252697f,-0.90416530058905542f, 0.19128270229571837f,-0.36935459470156029f,\
                         0.61303325198820890f}

#define CIMAG_RESULT13 {+5.38290618034079671f,-0.01795106539414900f,-1.0736822056133133f ,-0.38375436766279453f,\
                        +0.45177555081294796f,-0.42204321578379422f,+1.167268002300917f  ,-0.8654330910349184f ,\
                        -1.80135467250416070f,+0.73358402770681042f,+0.92212767526074568f,-0.53962030646712056f,\
                        -0.12239971073931022f}

#define CREAL_RESULT14 { 7.75120115978643298f,-0.78366539551591063f,-0.20265674140819168f, 0.04290246165533468f,\
                         2.26072777524451896f, 0.41764276367067404f,-0.94725360360479638f, 0.59614222729578614f,\
                         1.2655897771348088f , 1.49954927048727082f, 0.59161337187577756f, 1.32712418977692859f,\
                        -1.04078471420795249f,-0.48035642054451416f}


#define CIMAG_RESULT14 {+5.86955844005569816f,+0.59116535878306964f,+1.10949301280070478f,-0.10194863149531888f,\
                        +0.33742089533324454f,+1.27136622217475770f,+0.66505772083419801f,-0.35660744598135352f,\
                        +0.55272403261244618f,+0.78239239747489764f,-0.76097885152655409f,+1.7013910918422144f ,\
                        -1.05520441290632339f,-0.32851304179021534f}

#define CREAL_RESULT15 { 9.03908939706161618f, 0.83695739793685675f,-0.1247585940626968f , 0.73554494173292662f,\
                         2.89255624599792061f,-0.60942096648213817f,-0.96289916352621108f,-2.01148115975842146f,\
                         1.08671310762523987f, 1.10385593717105923f, 0.14097082301450437f, 0.77951939204949339f,\
                        -0.19682993144621974f, 1.09445436001472118f,-1.35379695139639411f}

#define CIMAG_RESULT15 {+7.929235897026956f  ,+0.93665405708769556f,-0.58867096407810116f,-0.08075001529554950f,\
                        +0.05154837779444588f,-1.64233407065738146f,+2.20857407200866884f,+0.92685932199933529f,\
                        -1.71222943015973872f,-0.79519673795720203f,-0.80828166087623532f,-0.07547063282874045f,\
                        +0.21930651464200918f,+1.00880777239444086f,-1.42196400314524918f}



#define CREAL_RESULT16 { 7.31841186061501503f, 0.57213963313411265f,-0.54757095809921363f,-0.48628670926159856f,\
                        -1.24745626002550125f,-0.60260425121772254f,-0.09566750389725764f, 1.12013387649474438f,\
                        -0.52123307064175606f,-0.4866536676629296f , 1.98659065302356819f,-0.8626986211125984f ,\
                        -0.61915938556194305f,-0.27813937201980266f,-1.53103677171080510f,-0.01918993749322817f}
#define CIMAG_RESULT16 { 6.47680679336190224f, 0.33111151130330035f,-0.19343861330849654f, 0.12474172265893407f,\
                        -1.0452539175748825f , 1.29632487527975693f, 1.87557979276701658f,-1.82623636350346352f,\
                        -1.86397336795926094f,-1.03154071610913434f,-0.48573205481665604f, 0.44539904220706855f,\
                        -0.74425477534532547f,-0.54299368721281471f, 0.37996440777257234f, 1.11249504536330601f}

#define CREAL_RESULT32 { 15.3165711835026741f,-1.79021577127059173f,-1.66659611407065089f, 0.17525916470909797f,\
                        -1.16958628014871602f, 0.58684741669397522f, 0.03947542161511042f, 0.99740008842981942f,\
                        -0.46323241293430328f, 2.122539701124051f  ,-1.52963914564883940f, 0.87990417229605744f,\
                         0.58569127383151542f,-0.18198535589432135f, 0.26043384746900655f,-1.11204765363415392f,\
                         0.35329844802618027f,-1.47568616310628631f,-2.03487116744967844f,-3.19495610958970166f,\
                         0.81026376203844086f,-0.46366666776372734f,-1.63150209835186510f, 0.77334707088593369f,\
                        -0.35098156332969666f,-1.63498270669406387f, 0.67411467120679691f, 2.80538085483913147f,\
                        -1.62281507315555107f,-0.5600265995962992f ,-0.48984739061140237f, 1.75450689143393301f}



#define CIMAG_RESULT32 { 15.509232945740223f ,-0.47962381296807621f, 0.21213951866464975f, 0.88442937061831350f,\
                         1.52924554070524898f,-0.6313403060045536f ,-2.25908603874729419f,-1.3836292677373856f ,\
                        -1.18231281638145447f,-2.14767090006699668f, 1.5452016553381984f ,-3.15355126536920993f,\
                         2.10199273301496747f, 0.67530605269461363f,-2.03603600735261558f, 1.2309547869577584f ,\
                        -1.9912955537438393f ,-1.9668221895811833f , 0.29575245179739662f, 1.34815224953105273f,\
                         1.508921339902356f  ,-0.40084285801706099f, 2.96716476331614754f, 1.08125713762201059f,\
                        -1.39964522421360016f,-1.30777696073860294f,-1.13169784714423916f,-2.00872755010475013f,\
                         0.53915777133569487f,-2.45178696294021004f , 1.56509394479014063f, 1.5558426888499468f}

static void cfftmaTest2 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN2;
    float tImagIn [] = CIMAG_IN2 ;



    float tRealResult [] = CREAL_RESULT2 ;
    float tImagResult [] = CIMAG_RESULT2 ;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS2));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS2 );
    floatComplex* Result   = FloatComplexMatrix ( tRealResult , tImagResult ,ROW*COLS2) ;



    cfftma ( in , ROW , COLS2 , out ) ;

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

static void cfftmaTest3 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN3;
    float tImagIn [] = CIMAG_IN3 ;



    float tRealResult [] = CREAL_RESULT3;
    float tImagResult [] = CIMAG_RESULT3 ;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS3));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS3 );
    floatComplex* Result   = FloatComplexMatrix ( tRealResult , tImagResult ,ROW*COLS3) ;



    cfftma ( in , ROW , COLS3 , out ) ;

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


static void cfftmaTest4 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN4;
    float tImagIn [] = CIMAG_IN4 ;



    float tRealResult [] = CREAL_RESULT4 ;
    float tImagResult [] = CIMAG_RESULT4 ;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS4));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS4 );
    floatComplex* Result   = FloatComplexMatrix ( tRealResult , tImagResult ,ROW*COLS4) ;


    cfftma ( in , ROW , COLS4 , out ) ;

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


static void cfftmaTest5 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN5;
    float tImagIn [] = CIMAG_IN5 ;



    float tRealResult [] = CREAL_RESULT5;
    float tImagResult [] = CIMAG_RESULT5 ;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS5));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS5 );
    floatComplex* Result   = FloatComplexMatrix ( tRealResult , tImagResult ,ROW*COLS5) ;



    cfftma ( in , ROW , COLS5 , out ) ;

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


static void cfftmaTest6 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN6;
    float tImagIn [] = CIMAG_IN6 ;



    float tRealResult [] = CREAL_RESULT6;
    float tImagResult [] = CIMAG_RESULT6;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS6));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS6 );
    floatComplex* Result   = FloatComplexMatrix ( tRealResult , tImagResult ,ROW*COLS6) ;



    cfftma ( in , ROW , COLS6 , out ) ;

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


static void cfftmaTest7 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN7;
    float tImagIn [] = CIMAG_IN7 ;



    float tRealResult [] = CREAL_RESULT7;
    float tImagResult [] = CIMAG_RESULT7;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS7));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS7 );
    floatComplex* Result   = FloatComplexMatrix ( tRealResult , tImagResult ,ROW*COLS7) ;



    cfftma ( in , ROW , COLS7 , out ) ;

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

static void cfftmaTest8 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN8;
    float tImagIn [] = CIMAG_IN8 ;



    float tRealResult [] = CREAL_RESULT8 ;
    float tImagResult [] = CIMAG_RESULT8 ;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS8));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS8 );
    floatComplex* Result   = FloatComplexMatrix ( tRealResult , tImagResult ,ROW*COLS8) ;


    cfftma ( in , ROW , COLS8 , out ) ;

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


static void cfftmaTest9 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN9;
    float tImagIn [] = CIMAG_IN9 ;



    float tRealResult [] = CREAL_RESULT9 ;
    float tImagResult [] = CIMAG_RESULT9 ;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS9));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS9 );
    floatComplex* Result   = FloatComplexMatrix ( tRealResult , tImagResult ,ROW*COLS9) ;


    cfftma ( in , ROW , COLS9 , out ) ;

    /* if we don't add that test assert failed if result = 0  'cause then we have  |(out - 0)|/|out| = 1*/
	for ( i = 0 ; i < (ROW*COLS9 )  ; i++ )
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

static void cfftmaTest10 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN10;
    float tImagIn [] = CIMAG_IN10 ;



    float tRealResult [] = CREAL_RESULT10 ;
    float tImagResult [] = CIMAG_RESULT10 ;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS10));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS10 );



    cfftma ( in , ROW , COLS10 , out );




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

static void cfftmaTest11 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN11;
    float tImagIn [] = CIMAG_IN11 ;



    float tRealResult [] = CREAL_RESULT11 ;
    float tImagResult [] = CIMAG_RESULT11 ;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS11));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS11 );



    cfftma ( in , ROW , COLS11 , out );




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


static void cfftmaTest12 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN12;
    float tImagIn [] = CIMAG_IN12 ;



    float tRealResult [] = CREAL_RESULT12 ;
    float tImagResult [] = CIMAG_RESULT12 ;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS12));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS12 );



    cfftma ( in , ROW , COLS12 , out );




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


static void cfftmaTest13 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN13;
    float tImagIn [] = CIMAG_IN13 ;



    float tRealResult [] = CREAL_RESULT13 ;
    float tImagResult [] = CIMAG_RESULT13 ;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS13));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS13 );



    cfftma ( in , ROW , COLS13 , out );




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


static void cfftmaTest14 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN14;
    float tImagIn [] = CIMAG_IN14 ;



    float tRealResult [] = CREAL_RESULT14 ;
    float tImagResult [] = CIMAG_RESULT14 ;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS14));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS14 );



    cfftma ( in , ROW , COLS14 , out );




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



static void cfftmaTest15 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN15;
    float tImagIn [] = CIMAG_IN15 ;



    float tRealResult [] = CREAL_RESULT15 ;
    float tImagResult [] = CIMAG_RESULT15 ;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS15));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS15 );



    cfftma ( in , ROW , COLS15 , out );




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



static void cfftmaTest16 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN16;
    float tImagIn [] = CIMAG_IN16 ;



    float tRealResult [] = CREAL_RESULT16 ;
    float tImagResult [] = CIMAG_RESULT16 ;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS16));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS16 );



    cfftma ( in , ROW , COLS16 , out );




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



static void cfftmaTest32 (void )
{
      int i = 0 ;

    float tRealIn [] = CREAL_IN32;
    float tImagIn [] = CIMAG_IN32 ;



    float tRealResult [] = CREAL_RESULT32 ;
    float tImagResult [] = CIMAG_RESULT32 ;



    floatComplex*  out     = (floatComplex*) malloc ( sizeof(floatComplex) * (unsigned int) (ROW*COLS32));
    floatComplex*  in      = FloatComplexMatrix ( tRealIn , tImagIn , ROW*COLS32 );



    cfftma ( in , ROW , COLS32 , out );




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
  cfftmaTest2();
  printf("\n\t>>>> Vector 3 Float Complex Tests\n");
  cfftmaTest3();
  printf("\n\t>>>> Vector 4 Float Complex Tests\n");
  cfftmaTest4();
  printf("\n\t>>>> Vector 5 Float Complex Tests\n");
  cfftmaTest5();
  printf("\n\t>>>> Vector 6 Float Complex Tests\n");
  cfftmaTest6();
  printf("\n\t>>>> Vector 7 Float Complex Tests\n");
  cfftmaTest7();
  printf("\n\t>>>> Vector 8 Float Complex Tests\n");
  cfftmaTest8();
  printf("\n\t>>>> Vector 9 Float Complex Tests\n");
  cfftmaTest9();
  printf("\n\t>>>> Vector 10 Float Complex Tests\n");
  cfftmaTest10();
  printf("\n\t>>>> Vector 11 Float Complex Tests\n");
  cfftmaTest11();
  printf("\n\t>>>> Vector 12 Float Complex Tests\n");
  cfftmaTest12();
  printf("\n\t>>>> Vector 13 Float Complex Tests\n");
  cfftmaTest13();
  printf("\n\t>>>> Vector 14 Float Complex Tests\n");
  cfftmaTest14();
  printf("\n\t>>>> Vector 14 Float Complex Tests\n");
  cfftmaTest15();
  printf("\n\t>>>> Vector 16 Float Complex Tests\n");
  cfftmaTest16();
  printf("\n\t>>>> Vector 32 Float Complex Tests\n");
  cfftmaTest32();
  return 0;
}



int main(void) {
  assert(testFft() == 0);
  return 0;
}

/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008 - INRIA - Arnaud TORSET
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
#include <assert.h>
#include <math.h>
#include "hilbert.h"


#define SOURCE2 {.7560438541695475578308,0.0002211346291005611420,\
0.3303270917385816574097,0.6653811042197048664093}

#define SOURCE3 {0.6283917883411049842835,0.8497452358715236186981,0.6857310198247432708740,\
0.8782164813019335269928,0.0683740368112921714783,0.5608486062847077846527,\
0.6623569373041391372681,0.7263506767340004444122,0.1985143842175602912903}

#define SOURCE4 {0.5442573162727057933807,0.2320747897028923034668,0.2312237196601927280426,\
 0.2164632631465792655945,\
0.8833887814544141292572,0.6525134947150945663452,0.3076090742833912372589,\
 0.9329616213217377662659,\
0.2146007861010730266571,0.3126419968903064727783,0.3616361008025705814362,\
 0.2922266637906432151794,\
0.5664248815737664699554,0.4826471973210573196411,0.3321718913502991199493,\
 0.5935094701126217842102}

#define SOURCE5 {0.3572649653069674968720,0.7693399824202060699463,0.5477633909322321414948,\
 0.0962288742884993553162,0.9561171731911599636078,\
0.2207408566027879714966,0.0143259358592331409454,0.8191489772871136665344,\
 0.1304992842487990856171,0.9682003557682037353516,\
0.6561381467618048191071,0.2445538965985178947449,0.5283123566769063472748,\
 0.8468925636261701583862,0.7876622085459530353546,\
0.1262082615867257118225,0.7883860985748469829559,0.3453042469918727874756,\
 0.2659857333637773990631,0.9709818651899695396423,\
0.8875247663818299770355,0.2066752854734659194946,0.8525160965509712696075,\
 0.6744697811082005500794,0.9152874383144080638886}

#define SOURCE7 {0.0284859761595726013184,0.2367841475643217563629,0.7015343559905886650085,\
 0.1202526628039777278900,0.8287412133067846298218,0.3161072959192097187042,\
 0.5305190766230225563049,\
0.5715174819342792034149,0.0478015430271625518799,0.8248619721271097660065,\
 0.5798843121156096458435,0.2791808298788964748383,0.9545110929757356643677,\
 0.9071154636330902576447,\
0.3360149310901761054993,0.1175613063387572765350,0.9253723770380020141602,\
 0.7263671257533133029938,0.9009497938677668571472,0.3948993249796330928803,\
 0.5655179750174283981323,\
0.7061489676125347614288,0.6787831196561455726624,0.413293636869639158249,\
 0.1402290873229503631592,0.4952356130816042423248,0.4194877287372946739197,\
 0.8626222289167344570160,\
0.2857509646564722061157,0.2512136367149651050568,0.3389101596549153327942,\
 0.3921975973062217235565,0.4681552127003669738770,0.3361602746881544589996,\
 0.5336876614019274711609,\
0.2039063959382474422455,0.1589989718049764633179,0.0181815107353031635284,\
 0.4098370606079697608948,0.0105834598653018474579,0.1965309716761112213135,\
 0.2725595370866358280182,\
0.3437655037268996238709,0.2033702037297189235687,0.3011944610625505447388,\
 0.2762595904059708118439,0.2944530742242932319641,0.5718073952011764049530,\
 0.2141770124435424804688}

#define SOURCE12 {0.5595936686731874942780,0.1249340316280722618103}

#define SOURCE13 {0.7279222286306321620941,0.2677766475826501846314,0.5465334919281303882599}
  
#define SOURCE14 {0.9885407658293843269348,0.7395656588487327098846,0.0037173107266426086426,\
0.5900572859682142734528}

#define SOURCE15 {0.3096467358991503715515,0.2552205710671842098236,0.6251879390329122543335,\
0.1157417376525700092316,0.6117004090920090675354}

#define SOURCE17 {0.6783956284634768962860,0.3320095278322696685791,0.0258709923364222049713,\
0.5174468224868178367615,0.3916873238049447536469,0.2413538414984941482544,\
0.5064434898085892200470}

#define SOURCE31 {0.4236102001741528511047,0.2893727528862655162811,0.0887932181358337402344}

#define SOURCE32 {0.6212881752289831638336,0.3454984454438090324402,0.7064867629669606685638,\
0.5211472492665052413940,0.2870400850661098957062,0.6502795079723000526428}

#define SOURCE34 {0.0881334762088954448700,0.4498763345181941986084,0.722725308034569025040,\
0.8976796260103583335877,0.2427821881137788295746,0.4337721113115549087524,\
0.9677053210325539112091,0.5068534435704350471497,0.523297640960663557053,\
0.5596947595477104187012,0.5617307000793516635895,0.468176002614200115204}

#define SOURCE35 {0.7794546722434461116791,0.7901071812957525253296,0.9808542062528431415558,\
0.8187066102400422096252,0.4256872381083667278290,0.2461560554802417755127,\
0.922953246627002954483,0.1000745808705687522888,0.4678218117915093898773,\
0.3950497675687074661255,0.0366117102093994617462,0.5175368534401059150696,\
0.8325451570563018321991,0.6104832142591476440430,0.1871111788786947727203}

#define SOURCE37 {0.0189574835821986198425,0.8433564766310155391693,0.0748594831675291061401,\
0.8532815133221447467804,0.0124590108171105384827,0.1867539356462657451630,\
0.4920584075152873992920,0.7489608139730989933014,0.9414957007393240928650,\
0.212405560072511434555,0.5795019958168268203735,0.2628147569485008716583,\
0.4360986622050404548645,0.9110545101575553417206,0.8082667365670204162598,\
0.8102653049863874912262,0.2590428395196795463562,0.4139087288640439510346,\
0.3599927779287099838257,0.6912787933833897113800,0.7656858796253800392151}


#define RESULTR2 {0.5885168479289859533310,0.1677481408696621656418,\
0.7082384515088051557541,0.2874697444494813680649}

#define RESULTR3 {1.0529534542001781627363,0.7069182925236719983175,0.4039962973135212687126,\
0.8377721576641001810515,0.3181039501602450081386,0.3515630165735879608668,\
0.6943617464664080252135,0.5110746005860467011672,0.3817856512032448690341}

#define RESULTR4 {0.6752608462702482938767,0.2057518342044204473496,-0.0427890990395098924637,\
 0.3857955073472112417221,\
0.9621772530954331159592,0.6242313084658235311508,0.3033984049689024686813,\
 0.8866660052444785833359,\
0.3456043160986155271530,0.2863190413918346166611,0.0876232821028679609299,\
 0.4615589079912751913071,\
0.6452133532147854566574,0.454365011071786284447,0.3279612220358103513718,\
 0.5472138540353626012802}

#define RESULTR5 {0.2962400295259193239161,0.3089018898394935153107,0.8970718315147718779912,\
 -0.1110250488619887454522,1.3355256841208691387379,\
0.2186875433360468412580,0.0142399996764340786015,0.5494750142671456227816,\
 0.1438244289733127678765,1.2266884235131985114720,\
0.3243622761355875461220,0.146488853809641050852,0.8307917790741377039865,\
 0.4672829933864904861096,1.294633269803495245753,\
0.2689630731113705763846,0.3885024240640397863444,0.3410024486082647299234,\
 0.3015788194209125805401,1.196819440502604692256,\
0.5921953628411829040346,0.3931235221328068796254,0.7724433550837475070949,\
 0.4311855720220420518096,1.3475255557490961599854}

#define RESULTR7 {0.0221790934600933908882,0.1223243807506165237165,0.8078579279308952809657,\
 0.0831055973421020999226,0.7234293187562024218806,0.3410300566625018015010,\
 0.6624983534650656924470,\
0.4173417573555355808423,-0.1702289447306715719499,0.8932521097561832990053,\
 0.5795505696167876363845,0.4650900528477819317885,0.9531674534660129483044,\
 1.0266996973802535730869,\
0.3592502496807103984366,-0.0602177881600134204398,1.0040005062671357727311,\
 0.5952560602897534192124,0.8458135965517576959627,0.4992619639538963260961,\
 0.7233182455018367651434,\
0.5575694746471103746544,0.5320442416692767739761,0.5114069770070045484189,\
 0.2079572921340347990693,0.5832271371004160709362,0.3596406358527834412620,\
 0.9639546237862767208426,\
0.2693600210346490664293,0.0082908280086841923406,0.3916743713665618020769,\
 0.2324071406142651508020,0.5392849148911003087648,0.4665026157423768915500,\
 0.6985556154653854710190,\
0.1211347285688481462129,0.0583534009281404877911,0.1331768173348806816314,\
 0.4611817954035059563545,-0.0310306062323534859182,0.1483735266235425009906,\
 0.3794082450879812107303,\
0.2484192180646929426491,-0.0574661602186613551524,0.3494023378688325687236,\
 0.1746705178068543862224,0.4728580805104006801187,0.6551060166287933084206,\
 0.3620372301332393516482}

#define RESULTR12 {0.5595936686731874942780,0.1249340316280722618103}

#define RESULTR13 {0.727922228630632051072,0.2677766475826501846314,0.5465334919281302772376}

#define RESULTR14 {0.9885407658293843269348,0.7395656588487327098846,0.0037173107266426086426,\
0.5900572859682142734528}

#define RESULTR15 {0.3096467358991503715515,0.2552205710671842098236,0.6251879390329122543335,\
0.1157417376525700092316,0.6117004090920090675354}
  
#define RESULTR17 {0.6783956284634768962860,0.3320095278322696685791,0.0258709923364222188491,\
0.5174468224868178367615,0.3916873238049447536469,0.2413538414984941482544,\
0.5064434898085892200470}
 
#define RESULTR31 {0.4236102001741528511047,0.2893727528862655162811,0.0887932181358337541122}

#define RESULTR32 {0.6562798103938499716392,0.1463829164082805778246,0.8706106568376222876182,\
0.5846776299489041539559,0.0747807359633347046302,0.7990084763926764699349}

#define RESULTR34 {0.2895147710417708131203,0.324826880668600381785,0.6463934670512875291237,\
0.8825004372435311106315,0.1780484897705416069691,0.5136849984216192988029,\
1.1690866158654293904817,0.3818039897208412303264,0.4469657999773820611367,\
0.5445155707808831957451,0.4969970017361144409840,0.5480888897242645052543}
 
#define RESULTR35 {1.0992448165164583340214,0.6267460169007069703540,0.8244252263748765852114,\
0.9107979135927461999955,0.3598763258423408051279,0.219875664393563818866,\
1.2103192873573502819085,-0.1660945217914423510663,0.4466248737231732213182,\
0.5941534869981266453465,-0.0029212825965045929877,0.3579661268165908460937,\
0.9791855748989433116947,0.4046034830239924606765,0.2463504922712086431247}

#define RESULTR37 {0.0033434146069818981850,0.9554376522688345918510,-0.0216076234950733081508,\
0.7336972769716557651876,0.0730809598165483853549,0.2457162229973169909059,\
0.5179742532819129330335,0.7909655834196462897978,0.8735750855261512626271,\
0.0801094846848779407189,0.7065129902723632504547,0.2680998378805983239914,\
0.4433905356823317212012,0.9035067595439678633795,0.8085226137033167947976,\
0.7242403070946337173552,0.4089553443197004867216,0.3500212219557769510736,\
0.3025310230791596888267,0.6920164671332854622321,0.8224099607250346943843}


#define RESULTI2 {0,0,\
0,0}

#define RESULTI3 {0.0827980395095824772689,-0.1114116569690337538168,-0.0174490462390520933988,\
-0.2070330189892598260482,-0.1130704082592781378747,-0.0128233225106435810847,\
0.1242349794796773487793,0.2244820652283119055692,0.0302723687496956744836}

#define RESULTI4 {0.1584819499403238296509,0.0849331486970186233521,-0.0122814085334539413452,\
 0.1697260756045579910278,\
-0.1648282650858163833618,0.0402836035937070846558,0.0652061905711889266968,\
 0.0378817003220319747925,\
-0.1584819499403238296509,-0.0849331486970186233521,0.0122814085334539413452,\
 -0.1697260756045579910278,\
0.1648282650858163833618,-0.0402836035937070846558,-0.0652061905711889266968,\
 -0.0378817003220319747925}

#define RESULTI5 {-0.2379577488223760806108,-0.1980500856853430191595,-0.2084157540378530903968,\
 -0.2009595577277856481846,-0.0885928718752091587385,\
0.0642786760664435519175,-0.0992997005877370997329,0.0218942281271939376275,\
 0.3748027917451171031615,-0.0032719016765754976861,\
-0.0120583931462525106859,0.2181341408963820838185,-0.1102129389906743167638,\
 0.0183158030378510194858,-0.0201291388704768203777,\
0.1535920604590304550108,0.1089958127617958461020,-0.0864243708530107318522,\
 0.0148125088838929608315,0.0167421638470533970522,\
0.0321454054431545913073,-0.0297801673850977763336,0.3831588357543441736297,\
 -0.2069715459390754075386,0.0952517485752080866890}

#define RESULTI7 {0.1409758334471349350281,0.0314029776230078383481,0.3019335556748578008879,\
 0.2398099296761546650458,-0.0573140088334526676128,0.1432732614656592184943,\
 0.4398940061473099460976,\
0.2162564212505879712722,-0.1637770322583952942175,0.1579264489634253088024,\
 0.2772819115997168237797,0.1711718089649424678900,0.1763527386616343117787,\
 0.0538284803848837895002,\
0.0479617156709556957672,0.4681440954755101135909,-0.2490982322246580449310,\
 -0.1915260827332628534858,-0.0036549673932591286718,-0.470893962786261499609,\
 -0.0386408325310867925562,\
0.0046438722379344987701,-0.0050534006016343113732,-0.4130897490567612129198,\
 -0.2025604018656496918549,-0.2026110258293718491274,0.0911255775566176540181,\
 -0.0247913383142026599870,\
-0.3363272079021759064510,-0.2442642889954655671669,-0.2069968083137898595680,\
 0.0984957222872148985271,-0.3125265588587076148741,-0.2526097487856979739718,\
 -0.4062536135232318601496,\
0.0341324589755990226436,-0.0326770186735925727284,-0.0666918900335931735812,\
 -0.0303613325286397894853,-0.1194235732677166039739,0.2006381933984195675702,\
 -0.1355401977319409256850,\
-0.1076430936800362725414,-0.0537753325694302272697,0.4760166749905191396763,\
 -0.1911397464355340802822,0.5243583252175655351479,0.1121139404896287633529,\
 0.1115034955682685374745}

#define RESULTI12 {0,0}

#define RESULTI13 {-0.1609403391213135858795,-0.104724835963155529783,0.2656651750844691295406}

#define RESULTI14 {0.0747541864402592182159,-0.4924117275513708591461,-0.0747541864402592182159,\
0.4924117275513708591461}

#define RESULTI15 {-0.2934532919715012178052,0.2662942066238083160812,-0.1297453097398825194730,\
-0.0162106544485858539151,0.1731150495361612751122}

#define RESULTI17 {-0.0718252325460487783815,-0.3822405612852492473586,0.0883019157396930931547,\
0.2043974836060600819288,-0.143817067391037223256,0.1155937622404165643353,\
0.1895896996361655095775}

#define RESULTI31 {0.1158046483820916738861,-0.1933066747090816384080,0.0775020263269899506442}

#define RESULTI32 {0,0,0,\
0,0,0}

#define RESULTI34 {0.1689924332313239296877,-0.1594742559827864170075,-0.0172019456513225893479,\
0.4397859224118292331696,0.0284885545261204103928,-0.0997138335369527201157,\
-0.1689924332313239296877,0.1594742559827864170075,0.0172019456513225893479,\
-0.4397859224118292331696,-0.0284885545261204103928,0.0997138335369527201157}

#define RESULTI35 {-0.1316292475222873847862,-0.0038207475230211465633,-0.0291790984031743347304,\
0.124318851362821516937,-0.4288056400198558271164,-0.2487686861965354157000,\
-0.1774606387906726145953,-0.1910360457356966579212,0.1689989243063947654822,\
-0.1696542221928825366906,0.3125071348259343961828,-0.2111224267558209632956,\
0.3544252571430209775016,0.3111552984526392839904,0.3200712870491359551828}

#define RESULTI37 {0.2189793220094558567901,-0.4489603496292741136031,-0.4937799046293088744974,\
0.2870975885300787178167,-0.0153475020886480462146,0.6805622794419772247920,\
-0.3638091467691619285141,0.3493297478826523638773,-0.1277224814869524771588,\
0.0075644305010185153293,0.0001438303630438494962,0.103302919046026508987,\
0.2871483917593231072196,-0.0213705070070041927566,0.0069067656750675318622,\
-0.0442384488081716820140,-0.2780963082978277212121,-0.0838499868835354822405,\
-0.392742137222542697650,0.4143010887770579020462,-0.0854195911632744664388}

static void double2float (double* tabdouble, float* tabfloat,int size){
	int i;
	
	for(i=0;i<size;i++)  tabfloat[i]=(float)tabdouble[i];		
}
	
static void dhilbertaTest(void){
	int i;
	double in2[]=SOURCE2;
	double in3[]=SOURCE3;
	double in4[]=SOURCE4;
	double in5[]=SOURCE5;
	double in7[]=SOURCE7;
	double in12[]=SOURCE12;
	double in13[]=SOURCE13;
	double in14[]=SOURCE14;
	double in15[]=SOURCE15;
	double in17[]=SOURCE17;	
	double in31[]=SOURCE31;
	double in32[]=SOURCE32;
	double in34[]=SOURCE34;
	double in35[]=SOURCE35;
	double in37[]=SOURCE37;	
	
	double resR2[]=RESULTR2;
	double resR3[]=RESULTR3;
	double resR4[]=RESULTR4;
	double resR5[]=RESULTR5;
	double resR7[]=RESULTR7;
	double resR12[]=RESULTR12;
	double resR13[]=RESULTR13;
	double resR14[]=RESULTR14;
	double resR15[]=RESULTR15;
	double resR17[]=RESULTR17;	
	double resR31[]=RESULTR31;
	double resR32[]=RESULTR32;
	double resR34[]=RESULTR34;
	double resR35[]=RESULTR35;
	double resR37[]=RESULTR37;	
	
	double resI2[]=RESULTI2;
	double resI3[]=RESULTI3;
	double resI4[]=RESULTI4;
	double resI5[]=RESULTI5;
	double resI7[]=RESULTI7;
	double resI12[]=RESULTI12;
	double resI13[]=RESULTI13;
	double resI14[]=RESULTI14;
	double resI15[]=RESULTI15;
	double resI17[]=RESULTI17;	
	double resI31[]=RESULTI31;
	double resI32[]=RESULTI32;
	double resI34[]=RESULTI34;
	double resI35[]=RESULTI35;
	double resI37[]=RESULTI37;	

	doubleComplex *out2,*out3,*out4,*out5,*out7;
	doubleComplex *out12,*out13,*out14,*out15,*out17;
	doubleComplex *out31,*out32,*out34,*out35,*out37;
	
	out2=(doubleComplex*)malloc((unsigned int)4*sizeof(doubleComplex));
	out3=(doubleComplex*)malloc((unsigned int)9*sizeof(doubleComplex));
	out4=(doubleComplex*)malloc((unsigned int)16*sizeof(doubleComplex));
	out5=(doubleComplex*)malloc((unsigned int)25*sizeof(doubleComplex));
	out7=(doubleComplex*)malloc((unsigned int)49*sizeof(doubleComplex));
	
	out12=(doubleComplex*)malloc((unsigned int)2*sizeof(doubleComplex));
	out13=(doubleComplex*)malloc((unsigned int)3*sizeof(doubleComplex));
	out14=(doubleComplex*)malloc((unsigned int)4*sizeof(doubleComplex));
	out15=(doubleComplex*)malloc((unsigned int)5*sizeof(doubleComplex));
	out17=(doubleComplex*)malloc((unsigned int)7*sizeof(doubleComplex));	
	
	out31=(doubleComplex*)malloc((unsigned int)3*sizeof(doubleComplex));
	out32=(doubleComplex*)malloc((unsigned int)6*sizeof(doubleComplex));
	out34=(doubleComplex*)malloc((unsigned int)12*sizeof(doubleComplex));
	out35=(doubleComplex*)malloc((unsigned int)15*sizeof(doubleComplex));
	out37=(doubleComplex*)malloc((unsigned int)21*sizeof(doubleComplex));
	
	dhilberta(in2, 2, 2, out2);
	dhilberta(in3, 3, 3, out3);
	dhilberta(in4, 4, 4, out4);
	dhilberta(in5, 5, 5, out5);
	dhilberta(in7, 7, 7, out7);
	
	dhilberta(in12, 1, 2, out12);
	dhilberta(in13, 1, 3, out13);
	dhilberta(in14, 1, 4, out14);
	dhilberta(in15, 1, 5, out15);
	dhilberta(in17, 1, 7, out17);
	
	dhilberta(in31, 3, 1, out31);
	dhilberta(in32, 3, 2, out32);
	dhilberta(in34, 3, 4, out34);
	dhilberta(in35, 3, 5, out35);
	dhilberta(in37, 3, 7, out37);	
	
	
	/* Matrice 2*2 */	
	printf(">>> Matrice 2*2 <<<\n");
	for (i=0;i<4;i++){
	if (zreals(out2[i])>1e-16) assert( (fabs(zreals(out2[i])-resR2[i]) / fabs(zreals(out2[i])) )<1e-16);
	else 			   assert(1);
	if (zimags(out2[i])>1e-16) assert( (fabs(zimags(out2[i])-resI2[i]) / fabs(zimags(out2[i])) )<1e-16);
	else 			   assert(1);	
	}
	
	/* Matrice 3*3 */	
	printf(">>> Matrice 3*3 <<<\n");
	for (i=0;i<9;i++){
	if (zreals(out3[i])>1e-16) assert( (fabs(zreals(out3[i])-resR3[i]) / fabs(zreals(out3[i])) )<3e-15);
	else 			   assert(1);
	if (zimags(out3[i])>1e-16) assert( (fabs(zimags(out3[i])+resI3[i]) / fabs(zimags(out3[i])) )<3e-15);
	else 			   assert(1);	
	}	
	
	/* Matrice 4*4 */	
	printf(">>> Matrice 4*4 <<<\n");
	for (i=0;i<16;i++){
	if (zreals(out4[i])>1e-16) assert( (fabs(zreals(out4[i])-resR4[i]) / fabs(zreals(out4[i])) )<1e-16);
	else 			   assert(1);
	if (zimags(out4[i])>1e-16) assert( (fabs(zimags(out4[i])+resI4[i]) / fabs(zimags(out4[i])) )<1e-16);
	else 			   assert(1);	
	}
	
	/* Matrice 5*5 */	
	printf(">>> Matrice 5*5 <<<\n");
	for (i=0;i<25;i++){
	if (zreals(out5[i])>1e-16) assert( (fabs(zreals(out5[i])-resR5[i]) / fabs(zreals(out5[i])) )<3e-14);
	else 			   assert(1);
	if (zimags(out5[i])>1e-16) assert( (fabs(zimags(out5[i])+resI5[i]) / fabs(zimags(out5[i])) )<3e-13);
	else 			   assert(1);	
	}
	
	/* Matrice 7*7 */	
	printf(">>> Matrice 7*7 <<<\n");
	for (i=0;i<49;i++){
	if (zreals(out7[i])>1e-16) assert( (fabs(zreals(out7[i])-resR7[i]) / fabs(zreals(out7[i])) )<3e-14);
	else 			   assert(1);
	if (zimags(out7[i])>1e-16) assert( (fabs(zimags(out7[i])+resI7[i]) / fabs(zimags(out7[i])) )<3e-14);
	else 			   assert(1);	
	}
	
	/* Vecteur 1*2 */ 	
	printf(">>> Matrice 1*2 <<<\n");
	for (i=0;i<2;i++){
	if (zreals(out12[i])>1e-16) assert( (fabs(zreals(out12[i])-resR12[i]) / fabs(zreals(out12[i])) )<1e-16);
	else 			   assert(1);
	if (zimags(out12[i])>1e-16) assert( (fabs(zimags(out12[i])+resI12[i]) / fabs(zimags(out12[i])) )<1e-16);
	else 			   assert(1);	
	}
				
	/* Vecteur 1*3 */ 	
	printf(">>> Matrice 1*3 <<<\n");
	for (i=0;i<3;i++){
	if (zreals(out13[i])>1e-16) assert( (fabs(zreals(out13[i])-resR13[i]) / fabs(zreals(out13[i])) )<3e-16);
	else 			   assert(1);
	if (zimags(out13[i])>1e-16) assert( (fabs(zimags(out13[i])+resI13[i]) / fabs(zimags(out13[i])) )<1e-16);
	else 			   assert(1);	
	}

	/* Vecteur 1*4 */ 	
	printf(">>> Matrice 1*4 <<<\n");
	for (i=0;i<4;i++){
	if (zreals(out14[i])>1e-16) assert( (fabs(zreals(out14[i])-resR14[i]) / fabs(zreals(out14[i])) )<1e-16);
	else 			   assert(1);
	if (zimags(out14[i])>1e-16) assert( (fabs(zimags(out14[i])+resI14[i]) / fabs(zimags(out14[i])) )<1e-16);
	else 			   assert(1);	
	}

	/* Vecteur 1*5 */ 	
	printf(">>> Matrice 1*5 <<<\n");
	for (i=0;i<5;i++){
	if (zreals(out15[i])>1e-16) assert( (fabs(zreals(out15[i])-resR15[i]) / fabs(zreals(out15[i])) )<3e-15);
	else 			   assert(1);
	if (zimags(out15[i])>1e-16) assert( (fabs(zimags(out15[i])+resI15[i]) / fabs(zimags(out15[i])) )<3e-15);
	else 			   assert(1);	
	}

	/* Vecteur 1*7 */ 	
	printf(">>> Matrice 1*7 <<<\n");
	for (i=0;i<7;i++){
	if (zreals(out17[i])>1e-16) assert( (fabs(zreals(out17[i])-resR17[i]) / fabs(zreals(out17[i])) )<3e-16);
	else 			   assert(1);
	if (zimags(out17[i])>1e-16) assert( (fabs(zimags(out17[i])+resI17[i]) / fabs(zimags(out17[i])) )<3e-15);
	else 			   assert(1);	
	}

	/* Matrices 3*1 */ 	
	printf(">>> Matrice 3*1 <<<\n");
	for (i=0;i<3;i++){
	if (zreals(out31[i])>1e-16) assert( (fabs(zreals(out31[i])-resR31[i]) / fabs(zreals(out31[i])) )<3e-16);
	else 			   assert(1);
	if (zimags(out31[i])>1e-16) assert( (fabs(zimags(out31[i])+resI31[i]) / fabs(zimags(out31[i])) )<1e-16);
	else 			   assert(1);	
	}

	/* Matrices 3*2 */ 	
	printf(">>> Matrice 3*2 <<<\n");
	for (i=0;i<6;i++){
	if (zreals(out32[i])>1e-16) assert( (fabs(zreals(out32[i])-resR32[i]) / fabs(zreals(out32[i])) )<3e-15);
	else 			   assert(1);
	if (zimags(out32[i])>1e-16) assert( (fabs(zimags(out32[i])+resI32[i]) / fabs(zimags(out32[i])) )<1e-16);
	else 			   assert(1);	
	}


	/* Matrices 3*4 */ 	
	printf(">>> Matrice 3*4 <<<\n");
	for (i=0;i<12;i++){
	if (zreals(out34[i])>1e-16) assert( (fabs(zreals(out34[i])-resR34[i]) / fabs(zreals(out34[i])) )<1e-16);
	else 			   assert(1);
	if (zimags(out34[i])>1e-16) assert( (fabs(zimags(out34[i])+resI34[i]) / fabs(zimags(out34[i])) )<3e-15);
	else 			   assert(1);	
	}


	/* Matrices 3*5 */ 	
	printf(">>> Matrice 3*5 <<<\n");
	for (i=0;i<15;i++){
	if (zreals(out35[i])>1e-16) assert( (fabs(zreals(out35[i])-resR35[i]) / fabs(zreals(out35[i])) )<3e-15);
	else 			   assert(1);
	if (zimags(out35[i])>1e-16) assert( (fabs(zimags(out35[i])+resI35[i]) / fabs(zimags(out35[i])) )<3e-14);
	else 			   assert(1);	
	}


	/* Matrices 3*7 */ 	
	printf(">>> Matrice 3*7 <<<\n");
	for (i=0;i<21;i++){
	if (zreals(out37[i])>1e-16) assert( (fabs(zreals(out37[i])-resR37[i]) / fabs(zreals(out37[i])) )<3e-13);
	else 			   assert(1);
	if (zimags(out37[i])>1e-16) assert( (fabs(zimags(out37[i])+resI37[i]) / fabs(zimags(out37[i])) )<3e-14);
	else 			   assert(1);	
	}

	free(out2);
	free(out3);
	free(out4);
	free(out5);
	free(out7);
	
	free(out12);
	free(out13);
	free(out14);
	free(out15);
	free(out17);	
	
	free(out31);
	free(out32);
	free(out34);
	free(out35);
	free(out37);
}

static void shilbertaTest(void){
	int i;
	double ind2[]=SOURCE2;float in2[4];
	double ind3[]=SOURCE3;float in3[9];
	double ind4[]=SOURCE4;float in4[16];
	double ind5[]=SOURCE5;float in5[25];
	double ind7[]=SOURCE7;float in7[49];
	double ind12[]=SOURCE12;float in12[2];
	double ind13[]=SOURCE13;float in13[3];
	double ind14[]=SOURCE14;float in14[4];
	double ind15[]=SOURCE15;float in15[5];
	double ind17[]=SOURCE17;float in17[7];	
	double ind31[]=SOURCE31;float in31[3];	
	double ind32[]=SOURCE32;float in32[6];
	double ind34[]=SOURCE34;float in34[12];
	double ind35[]=SOURCE35;float in35[15];
	double ind37[]=SOURCE37;float in37[21];	
	
	double resdR2[]=RESULTR2;float resR2[4];
	double resdR3[]=RESULTR3;float resR3[9];
	double resdR4[]=RESULTR4;float resR4[16];
	double resdR5[]=RESULTR5;float resR5[25];
	double resdR7[]=RESULTR7;float resR7[49];
	double resdR12[]=RESULTR12;float resR12[2];
	double resdR13[]=RESULTR13;float resR13[3];
	double resdR14[]=RESULTR14;float resR14[4];
	double resdR15[]=RESULTR15;float resR15[5];
	double resdR17[]=RESULTR17;float resR17[7];	
	double resdR31[]=RESULTR31;float resR31[3];
	double resdR32[]=RESULTR32;float resR32[6];
	double resdR34[]=RESULTR34;float resR34[12];
	double resdR35[]=RESULTR35;float resR35[15];
	double resdR37[]=RESULTR37;float resR37[21];	
	
	double resdI2[]=RESULTI2;float resI2[4];
	double resdI3[]=RESULTI3;float resI3[9];
	double resdI4[]=RESULTI4;float resI4[16];
	double resdI5[]=RESULTI5;float resI5[25];
	double resdI7[]=RESULTI7;float resI7[49];
	double resdI12[]=RESULTI12;float resI12[2];
	double resdI13[]=RESULTI13;float resI13[3];
	double resdI14[]=RESULTI14;float resI14[4];
	double resdI15[]=RESULTI15;float resI15[5];
	double resdI17[]=RESULTI17;float resI17[7];	
	double resdI31[]=RESULTI31;float resI31[3];
	double resdI32[]=RESULTI32;float resI32[6];
	double resdI34[]=RESULTI34;float resI34[12];
	double resdI35[]=RESULTI35;float resI35[15];
	double resdI37[]=RESULTI37;float resI37[21];
	
	floatComplex *out2,*out3,*out4,*out5,*out7;
	floatComplex *out12,*out13,*out14,*out15,*out17;
	floatComplex *out31,*out32,*out34,*out35,*out37;
	
	out2=(floatComplex*)malloc((unsigned int)4*sizeof(floatComplex));
	out3=(floatComplex*)malloc((unsigned int)9*sizeof(floatComplex));
	out4=(floatComplex*)malloc((unsigned int)16*sizeof(floatComplex));
	out5=(floatComplex*)malloc((unsigned int)25*sizeof(floatComplex));
	out7=(floatComplex*)malloc((unsigned int)49*sizeof(floatComplex));
	
	out12=(floatComplex*)malloc((unsigned int)2*sizeof(floatComplex));
	out13=(floatComplex*)malloc((unsigned int)3*sizeof(floatComplex));
	out14=(floatComplex*)malloc((unsigned int)4*sizeof(floatComplex));
	out15=(floatComplex*)malloc((unsigned int)5*sizeof(floatComplex));
	out17=(floatComplex*)malloc((unsigned int)7*sizeof(floatComplex));	
	
	out31=(floatComplex*)malloc((unsigned int)3*sizeof(floatComplex));
	out32=(floatComplex*)malloc((unsigned int)6*sizeof(floatComplex));
	out34=(floatComplex*)malloc((unsigned int)12*sizeof(floatComplex));
	out35=(floatComplex*)malloc((unsigned int)15*sizeof(floatComplex));
	out37=(floatComplex*)malloc((unsigned int)21*sizeof(floatComplex));
	
	/* Mise en float */
	double2float(ind2,in2,4);
	double2float(ind3,in3,9);
	double2float(ind4,in4,16);
	double2float(ind5,in5,25);
	double2float(ind7,in7,49);
	double2float(ind12,in12,2);
	double2float(ind13,in13,3);
	double2float(ind14,in14,4);
	double2float(ind15,in15,5);
	double2float(ind17,in17,7);
	double2float(ind31,in31,3);
	double2float(ind32,in32,6);
	double2float(ind34,in34,12);
	double2float(ind35,in35,15);
	double2float(ind37,in37,21);
	
	double2float(resdR2,resR2,4);
	double2float(resdR3,resR3,9);
	double2float(resdR4,resR4,16);
	double2float(resdR5,resR5,25);
	double2float(resdR7,resR7,49);
	double2float(resdR12,resR12,2);
	double2float(resdR13,resR13,3);
	double2float(resdR14,resR14,4);
	double2float(resdR15,resR15,5);
	double2float(resdR17,resR17,7);
	double2float(resdR31,resR31,3);
	double2float(resdR32,resR32,6);
	double2float(resdR34,resR34,12);
	double2float(resdR35,resR35,15);
	double2float(resdR37,resR37,21);
	
	double2float(resdI2,resI2,4);
	double2float(resdI3,resI3,9);
	double2float(resdI4,resI4,16);
	double2float(resdI5,resI5,25);
	double2float(resdI7,resI7,49);
	double2float(resdI12,resI12,2);
	double2float(resdI13,resI13,3);
	double2float(resdI14,resI14,4);
	double2float(resdI15,resI15,5);
	double2float(resdI17,resI17,7);
	double2float(resdI31,resI31,3);
	double2float(resdI32,resI32,6);
	double2float(resdI34,resI34,12);
	double2float(resdI35,resI35,15);
	double2float(resdI37,resI37,21);
	
	/* Tests and assert */		
		
	/* Matrice 2*2 */	
	printf(">>> Matrice 2*2 <<<\n");	
	shilberta(in2, 2, 2, out2);
	for (i=0;i<4;i++){
	if (creals(out2[i])>1e-16) assert( (fabs(creals(out2[i])-resR2[i]) / fabs(creals(out2[i])) )<1e-6);
	else 			   assert(1);
	if (cimags(out2[i])>1e-16) assert( (fabs(cimags(out2[i])-resI2[i]) / fabs(cimags(out2[i])) )<1e-6);
	else 			   assert(1);	
	}
	
	/* Matrice 3*3 */	
	printf(">>> Matrice 3*3 <<<\n");
	shilberta(in3, 3, 3, out3);
	for (i=0;i<9;i++){
	if (creals(out3[i])>1e-16) assert( (fabs(creals(out3[i])-resR3[i]) / fabs(creals(out3[i])) )<1e-6);
	else 			   assert(1);
	if (cimags(out3[i])>1e-16) assert( (fabs(cimags(out3[i])+resI3[i]) / fabs(cimags(out3[i])) )<3e-6);
	else 			   assert(1);	
	}	
	
	/* Matrice 4*4 */	
	printf(">>> Matrice 4*4 <<<\n");
	shilberta(in4, 4, 4, out4);
	for (i=0;i<16;i++){
	if (creals(out4[i])>1e-16) assert( (fabs(creals(out4[i])-resR4[i]) / fabs(creals(out4[i])) )<1e-6);
	else 			   assert(1);
	if (cimags(out4[i])>1e-16) assert( (fabs(cimags(out4[i])+resI4[i]) / fabs(cimags(out4[i])) )<1e-6);
	else 			   assert(1);	
	}
	
	/* Matrice 5*5 */	
	printf(">>> Matrice 5*5 <<<\n");
	shilberta(in5, 5, 5, out5);
	for (i=0;i<25;i++){
	if (creals(out5[i])>1e-16) assert( (fabs(creals(out5[i])-resR5[i]) / fabs(creals(out5[i])) )<3e-6);
	else 			   assert(1);
	if (cimags(out5[i])>1e-16) assert( (fabs(cimags(out5[i])+resI5[i]) / fabs(cimags(out5[i])) )<3e-5);
	else 			   assert(1);	
	}
	
	/* Matrice 7*7 */	
	printf(">>> Matrice 7*7 <<<\n");
	shilberta(in7, 7, 7, out7);
	for (i=0;i<49;i++){
	if (creals(out7[i])>1e-16) assert( (fabs(creals(out7[i])-resR7[i]) / fabs(creals(out7[i])) )<1e-6);
	else 			   assert(1);
	if (cimags(out7[i])>1e-16) assert( (fabs(cimags(out7[i])+resI7[i]) / fabs(cimags(out7[i])) )<3e-6);
	else 			   assert(1);	
	}
	
	/* Vecteur 1*2 */ 	
	printf(">>> Matrice 1*2 <<<\n");
	shilberta(in12, 1, 2, out12);
	for (i=0;i<2;i++){
	if (creals(out12[i])>1e-16) assert( (fabs(creals(out12[i])-resR12[i]) / fabs(creals(out12[i])) )<1e-6);
	else 			   assert(1);
	if (cimags(out12[i])>1e-16) assert( (fabs(cimags(out12[i])+resI12[i]) / fabs(cimags(out12[i])) )<1e-6);
	else 			   assert(1);	
	}
				
	/* Vecteur 1*3 */ 	
	printf(">>> Matrice 1*3 <<<\n");
	shilberta(in13, 1, 3, out13);
	for (i=0;i<3;i++){
	if (creals(out13[i])>1e-16) assert( (fabs(creals(out13[i])-resR13[i]) / fabs(creals(out13[i])) )<1e-6);
	else 			   assert(1);
	if (cimags(out13[i])>1e-16) assert( (fabs(cimags(out13[i])+resI13[i]) / fabs(cimags(out13[i])) )<1e-6);
	else 			   assert(1);	
	}

	/* Vecteur 1*4 */ 	
	printf(">>> Matrice 1*4 <<<\n");
	shilberta(in14, 1, 4, out14);
	for (i=0;i<4;i++){
	if (creals(out14[i])>1e-16) assert( (fabs(creals(out14[i])-resR14[i]) / fabs(creals(out14[i])) )<3e-6);
	else 			   assert(1);
	if (cimags(out14[i])>1e-16) assert( (fabs(cimags(out14[i])+resI14[i]) / fabs(cimags(out14[i])) )<1e-6);
	else 			   assert(1);	
	}

	/* Vecteur 1*5 */ 	
	printf(">>> Matrice 1*5 <<<\n");
	shilberta(in15, 1, 5, out15);
	for (i=0;i<5;i++){
	if (creals(out15[i])>1e-16) assert( (fabs(creals(out15[i])-resR15[i]) / fabs(creals(out15[i])) )<1e-6);
	else 			   assert(1);
	if (cimags(out15[i])>1e-16) assert( (fabs(cimags(out15[i])+resI15[i]) / fabs(cimags(out15[i])) )<1e-6);
	else 			   assert(1);	
	}

	/* Vecteur 1*7 */ 	
	printf(">>> Matrice 1*7 <<<\n");
	shilberta(in17, 1, 7, out17);
	for (i=0;i<7;i++){
	if (creals(out17[i])>1e-16) assert( (fabs(creals(out17[i])-resR17[i]) / fabs(creals(out17[i])) )<1e-6);
	else 			   assert(1);
	if (cimags(out17[i])>1e-16) assert( (fabs(cimags(out17[i])+resI17[i]) / fabs(cimags(out17[i])) )<1e-6);
	else 			   assert(1);	
	}

	/* Matrices 3*1 */ 	
	printf(">>> Matrice 3*1 <<<\n");
	shilberta(in31, 3, 1, out31);
	for (i=0;i<3;i++){
	if (creals(out31[i])>1e-16) assert( (fabs(creals(out31[i])-resR31[i]) / fabs(creals(out31[i])) )<1e-6);
	else 			   assert(1);
	if (cimags(out31[i])>1e-16) assert( (fabs(cimags(out31[i])+resI31[i]) / fabs(cimags(out31[i])) )<1e-6);
	else 			   assert(1);	
	}

	/* Matrices 3*2 */ 	
	printf(">>> Matrice 3*2 <<<\n");
	shilberta(in32, 3, 2, out32);
	for (i=0;i<6;i++){
	if (creals(out32[i])>1e-16) assert( (fabs(creals(out32[i])-resR32[i]) / fabs(creals(out32[i])) )<1e-6);
	else 			   assert(1);
	if (cimags(out32[i])>1e-16) assert( (fabs(cimags(out32[i])+resI32[i]) / fabs(cimags(out32[i])) )<1e-6);
	else 			   assert(1);	
	}


	/* Matrices 3*4 */ 	
	printf(">>> Matrice 3*4 <<<\n");
	shilberta(in34, 3, 4, out34);
	for (i=0;i<12;i++){
	if (creals(out34[i])>1e-16) assert( (fabs(creals(out34[i])-resR34[i]) / fabs(creals(out34[i])) )<1e-6);
	else 			   assert(1);
	if (cimags(out34[i])>1e-16) assert( (fabs(cimags(out34[i])+resI34[i]) / fabs(cimags(out34[i])) )<1e-6);
	else 			   assert(1);	
	}


	/* Matrices 3*5 */ 	
	printf(">>> Matrice 3*5 <<<\n");
	shilberta(in35, 3, 5, out35);
	for (i=0;i<15;i++){
	if (creals(out35[i])>1e-16) assert( (fabs(creals(out35[i])-resR35[i]) / fabs(creals(out35[i])) )<1e-6);
	else 			   assert(1);
	if (cimags(out35[i])>1e-16) assert( (fabs(cimags(out35[i])+resI35[i]) / fabs(cimags(out35[i])) )<3e-6);
	else 			   assert(1);	
	}


	/* Matrices 3*7 */ 	
	printf(">>> Matrice 3*7 <<<\n");
	shilberta(in37, 3, 7, out37);	
	for (i=0;i<21;i++){
	if (creals(out37[i])>1e-16) assert( (fabs(creals(out37[i])-resR37[i]) / fabs(creals(out37[i])) )<3e-5);
	else 			   assert(1);
	if (cimags(out37[i])>1e-16) assert( (fabs(cimags(out37[i])+resI37[i]) / fabs(cimags(out37[i])) )<3e-6);
	else 			   assert(1);	
	}
	
	free(out2);
	free(out3);
	free(out4);
	free(out5);
	free(out7);
	
	free(out12);
	free(out13);
	free(out14);
	free(out15);
	free(out17);	
	
	free(out31);
	free(out32);
	free(out34);
	free(out35);
	free(out37);

}	


static int hilbertTest(void){
	printf("\n >>> Double Hilbert Tests <<<\n");
	dhilbertaTest();
	printf("\n >>> Float Hilbert Tests <<<\n");
	shilbertaTest();
	return 0;
}


int main (void){
	assert(hilbertTest()==0);
	return 0;
}





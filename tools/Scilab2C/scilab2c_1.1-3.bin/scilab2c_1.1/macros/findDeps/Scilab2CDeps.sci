//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Arnaud Torset
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function scilab2ccode = initDependecies()


/////////////////////////////////
////// AUXILIARY FUNCTIONS //////
/////////////////////////////////


//abs
scilab2ccode.deps.cabss=["ssqrts","sabss","creals","cimags"]
scilab2ccode.deps.cabsa=["cabss"]
scilab2ccode.deps.dabss=[]
scilab2ccode.deps.dabsa=["dabss"]
scilab2ccode.deps.sabss=[]
scilab2ccode.deps.sabsa=["sabss"]
scilab2ccode.deps.zabss=["dsqrts","dabss","zreals","zimags"]
scilab2ccode.deps.zabsa=["zabss"]


//conj
scilab2ccode.deps.cconjs=["FloatComplex","creals","cimags"]
scilab2ccode.deps.cconja=["cconjs"]
scilab2ccode.deps.zconjs=["DoubleComplex","zreals","zimags"]
scilab2ccode.deps.zconja=["zconjs"]

//find
scilab2ccode.deps.cfinda=["creals","cimags"]
scilab2ccode.deps.dfinda=[]
scilab2ccode.deps.sfinda=[]
scilab2ccode.deps.zfinda=["zreals","zimags"]


//find2d
scilab2ccode.deps.cfind2da=["creals","cimags"]
scilab2ccode.deps.dfind2da=[]
scilab2ccode.deps.sfind2da=[]
scilab2ccode.deps.zfind2da=["zreals","zimags"]


//frexp
scilab2ccode.deps.dfrexps=[]
scilab2ccode.deps.sfrexps=[]

//isempty
scilab2ccode.deps.cisemptys=[]
scilab2ccode.deps.cisemptya=["cfinda"]
scilab2ccode.deps.disemptys=[]
scilab2ccode.deps.disemptya=["dfinda"]
scilab2ccode.deps.sisemptys=[]
scilab2ccode.deps.sisemptya=["sfinda"]
scilab2ccode.deps.zisemptys=[]
scilab2ccode.deps.zisemptya=["zfinda"]

//isnan
scilab2ccode.deps.cisnans=["creals","cimags"]
scilab2ccode.deps.cisnana=["cisnans"]
scilab2ccode.deps.disnans=[]
scilab2ccode.deps.disnana=["disnans"]
scilab2ccode.deps.sisnans=[]
scilab2ccode.deps.sisnana=["sisnans"]
scilab2ccode.deps.zisnans=["zreals","zimags"]
scilab2ccode.deps.zisnana=["zisnans"]

//length

//pythag
scilab2ccode.deps.cpythags=["csqrts","cadds","cmuls"]
scilab2ccode.deps.dpythags=["dsqrts"]
scilab2ccode.deps.spythags=["ssqrts"]
scilab2ccode.deps.cpythags=["zsqrts","zadds","zmuls"]

//rand

//sign
scilab2ccode.deps.csigns=["FloatComplex","cabss","creals","cimags"]
scilab2ccode.deps.csigna=["csigns"]
scilab2ccode.deps.dsigns=[]
scilab2ccode.deps.dsigna=["dsigns"]
scilab2ccode.deps.dsigns=[]
scilab2ccode.deps.dsigna=["ssigns"]
scilab2ccode.deps.zsigns=["DoubleComplex","zabss","zreals","zimags"]
scilab2ccode.deps.zsigna=["zsigns"]

//size

//type


//////////////////////////////////
////// ELEMENTARY FUNCTIONS //////
//////////////////////////////////

//acos
scilab2ccode.deps.cacoss=["FloatComplex","ssqrts","sabss","sacoss","satans","slogs","slog1ps","creals","cimags"]
scilab2ccode.deps.cacosa=["cacoss"]
scilab2ccode.deps.dacoss=[]
scilab2ccode.deps.dacosa=["dacoss"]
scilab2ccode.deps.sacoss=[]
scilab2ccode.deps.sacosa=["sacoss"]
scilab2ccode.deps.zacoss=["DoubleComplex","dsqrts","dabss","dacoss","datans","dlogs","dlog1ps","zreals","zimags"]
scilab2ccode.deps.zacosa=["zacoss"]


//acosh
scilab2ccode.deps.cacoshs=["FloatComplex","cacoss","creals","cimags"]
scilab2ccode.deps.cacosha=["cacoshs"]
scilab2ccode.deps.dacoshs=[]
scilab2ccode.deps.dacosha=["dacoshs"]
scilab2ccode.deps.sacoshs=[]
scilab2ccode.deps.sacosha=["sacoshs"]
scilab2ccode.deps.zacoshs=["DoubleComplex","zacoss","zreals","zimags"]
scilab2ccode.deps.zacosha=["zacoshs"]

//asin
scilab2ccode.deps.casins=["FloatComplex","ssqrts","sabss","sasins","satans","slogs","slog1ps","dabss","creals","cimags"]
scilab2ccode.deps.casina=["casins"]
scilab2ccode.deps.dasins=[]
scilab2ccode.deps.dasina=["dasins"]
scilab2ccode.deps.sasins=[]
scilab2ccode.deps.sasina=["sasins"]
scilab2ccode.deps.zasins=["DoubleComplex","dsqrts","dabss","dasins","datans","dlog1ps","dlogs","zreals","zimags"]
scilab2ccode.deps.zasina=["zasins"]

//asinh
scilab2ccode.deps.casinhs=["FloatComplex","casins","creals","cimags"]
scilab2ccode.deps.casinha=["casinhs"]
scilab2ccode.deps.dasinhs=["DoubleComplex","zasins"]
scilab2ccode.deps.dasinha=["dasinhs"]
scilab2ccode.deps.sasinhs=["FloatComplex","casins"]
scilab2ccode.deps.sasinha=["sasinhs"]
scilab2ccode.deps.zasinhs=["DoubleComplex","zasins","zreals","zimags"]
scilab2ccode.deps.zasinha=["zasinhs"]


//atan
scilab2ccode.deps.catans=["satans","creals","cimags","dabss","slnp1m1s","sabss","FloatComplex"]
scilab2ccode.deps.catana=["catans"]
scilab2ccode.deps.datans=[]
scilab2ccode.deps.datana=["datans"]
scilab2ccode.deps.satans=[]
scilab2ccode.deps.satana=["satans"]
scilab2ccode.deps.zatans=["datans","zreals","zimags","dabss","dlnp1m1s","DoubleComplex"]
scilab2ccode.deps.zatana=["zatans"]


//atan2
scilab2ccode.deps.datan2s=[]
scilab2ccode.deps.datan2a=["datan2s"]
scilab2ccode.deps.satan2s=[]
scilab2ccode.deps.satan2a=["satan2s"]


//atanh
scilab2ccode.deps.catanhs=["FloatComplex","creals","cimags","catans"]
scilab2ccode.deps.catanha=["catanhs"]
scilab2ccode.deps.datanhs=["zimags","zatans","DoubleComplex"]
scilab2ccode.deps.datanha=["datanhs"]
scilab2ccode.deps.satanhs=["cimags","catans","FloatComplex"]
scilab2ccode.deps.satanha=["satanhs"]
scilab2ccode.deps.zatanhs=["DoubleComplex","zreals","zimags","zatans"]
scilab2ccode.deps.zatanha=["zatanhs"]


//cos
scilab2ccode.deps.ccoss=["creals","cimags","FloatComplex","scoss","scoshs","ssins","ssinhs"]
scilab2ccode.deps.ccosa=["ccoss"]
scilab2ccode.deps.dcoss=[]
scilab2ccode.deps.dcosa=["dcoss"]
scilab2ccode.deps.scoss=[]
scilab2ccode.deps.scosa=["scoss"]
scilab2ccode.deps.zcoss=["zreals","zimags","DoubleComplex","dcoss","dcoshs","dsins","dsinhs"]
scilab2ccode.deps.zcosa=["zcoss"]


//cosh
scilab2ccode.deps.ccoshs=["ccoss","FloatComplex","creals","cimags"]
scilab2ccode.deps.ccosha=["ccoshs"]
scilab2ccode.deps.dcoshs=["dexps","dabss"]
scilab2ccode.deps.dcosha=["dcoshs"]
scilab2ccode.deps.scoshs=["sexps","sabss"]
scilab2ccode.deps.scosha=["scoshs"]
scilab2ccode.deps.zcoshs=["zcoss","DoubleComplex","zreals","zimags"]
scilab2ccode.deps.zcosha=["zcoshs"]


//exp
scilab2ccode.deps.cexps=["creals","cimags","FloatComplex","sexps","scoss","ssins"]
scilab2ccode.deps.cexpa=["cexps"]
scilab2ccode.deps.dexps=[]
scilab2ccode.deps.dexpa=["dexps"]
scilab2ccode.deps.sexps=[]
scilab2ccode.deps.sexpa=["sexps"]
scilab2ccode.deps.zexps=["zreals","zimags","DoubleComplex","dexps","dcoss","dsins"]
scilab2ccode.deps.zexpa=["zexps"]


//exp10
scilab2ccode.deps.cexp10s=["cpows","FloatComplex"]
scilab2ccode.deps.cexp10a=["cexp10s"]
scilab2ccode.deps.dexp10s=[]
scilab2ccode.deps.dexp10a=["dexp10s"]
scilab2ccode.deps.sexp10s=[]
scilab2ccode.deps.sexp10a=["sexp10s"]
scilab2ccode.deps.zexp10s=["zpows","DoubleComplex"]
scilab2ccode.deps.zexp10a=["zexp10s"]


//lnp1m1
scilab2ccode.deps.dlnp1m1s=["dabss"]
scilab2ccode.deps.slnp1m1s=["sabss"]


//log
scilab2ccode.deps.clogs=["creals","cimags","slog1ps","slogs","spythags","FloatComplex"]
scilab2ccode.deps.cloga=["clogs"]
scilab2ccode.deps.dlogs=[]
scilab2ccode.deps.dloga=["dlogs"]
scilab2ccode.deps.slogs=[]
scilab2ccode.deps.sloga=["slogs"]
scilab2ccode.deps.zlogs=["zreals","zimags","zlog1ps","zlogs","zpythags","DoubleComplex"]
scilab2ccode.deps.zloga=["zlogs"]


//log1p
scilab2ccode.deps.clog1ps=["clogs","FloatComplex","creals","cimags"]
scilab2ccode.deps.clog1pa=["clog1ps"]
scilab2ccode.deps.dlog1ps=["dlnp1m1s","dlogs"]
scilab2ccode.deps.dlog1pa=["dlog1ps"]
scilab2ccode.deps.slog1ps=["slnp1m1s","slogs"]
scilab2ccode.deps.slog1pa=["slog1ps"]
scilab2ccode.deps.zlog1ps=["zlogs","DoubleComplex","zreals","zimags"]
scilab2ccode.deps.zlog1pa=["zlog1ps"]


//log10
scilab2ccode.deps.clog10s=["clogs","FloatComplex","creals","cimags","slogs"]
scilab2ccode.deps.clog10a=["clog10s"]
scilab2ccode.deps.dlog10s=[]
scilab2ccode.deps.dlog10a=["dlog10s"]
scilab2ccode.deps.slog10s=[]
scilab2ccode.deps.slog10a=["slog10s"]
scilab2ccode.deps.zlog10s=["zlogs","DoubleComplex","zreals","zimags","dlogs"]
scilab2ccode.deps.zlog10a=["zlog10s"]


//pow
scilab2ccode.deps.cpows=["cexps","cmuls","clogs"]
scilab2ccode.deps.cpowa=["cpows"]
scilab2ccode.deps.dpows=[]
scilab2ccode.deps.dpowa=["dpows"]
scilab2ccode.deps.spows=[]
scilab2ccode.deps.spowa=["spows"]
scilab2ccode.deps.zpows=["zexps","zmuls","zlogs"]
scilab2ccode.deps.zpowa=["zpows"]


//sin
scilab2ccode.deps.csins=["creals","cimags","FloatComplex","ssins","scoshs","scoss","ssinhs"]
scilab2ccode.deps.csina=["csins"]
scilab2ccode.deps.dsins=[]
scilab2ccode.deps.dsina=["dsins"]
scilab2ccode.deps.ssins=[]
scilab2ccode.deps.ssina=["ssins"]
scilab2ccode.deps.zsins=["zreals","zimags","DoubleComplex","dsins","dcoshs","dcoss","dsinhs"]
scilab2ccode.deps.zsina=["zsins"]


//sinh
scilab2ccode.deps.csinhs=["creals","cimags","csins","FloatComplex"]
scilab2ccode.deps.csinha=["csinhs"]
scilab2ccode.deps.dsinhs=[]
scilab2ccode.deps.dsinha=["dsinhs"]
scilab2ccode.deps.ssinhs=[]
scilab2ccode.deps.ssinha=["ssinhs"]
scilab2ccode.deps.zsinhs=["zreals","zimags","zsins","DoubleComplex"]
scilab2ccode.deps.zsinha=["zsinhs"]


//sqrt
scilab2ccode.deps.csqrts=["creals","cimags","dabss","ssqrts","sabss","spythags","FloatComplex"]
scilab2ccode.deps.csqrta=["csqrts"]
scilab2ccode.deps.dsqrts=[]
scilab2ccode.deps.dsqrta=["dsqrts"]
scilab2ccode.deps.ssqrts=[]
scilab2ccode.deps.ssqrta=["ssqrts"]
scilab2ccode.deps.zsqrts=["zreals","zimags","dabss","dsqrts","dpythags","DoubleComplex"]
scilab2ccode.deps.zsqrta=["zsqrts"]


//tan
scilab2ccode.deps.ctans=["slogs","ssqrts","creals","cimags","scoss","ssinhs","ssins","sabss","FloatComplex"]
scilab2ccode.deps.ctana=["ctans"]
scilab2ccode.deps.dtans=[]
scilab2ccode.deps.dtana=["dtans"]
scilab2ccode.deps.stans=[]
scilab2ccode.deps.stana=["stans"]
scilab2ccode.deps.ztans=["dlogs","dsqrts","zreals","zimags","dcoss","dsinhs","dsins","dabss","DoubleComplex"]
scilab2ccode.deps.ztana=["ztans"]


//tanh
scilab2ccode.deps.ctanhs=["creals","cimags","ctans","FloatComplex"]
scilab2ccode.deps.ctanha=["ctanhs"]
scilab2ccode.deps.dtanhs=[]
scilab2ccode.deps.dtanha=["dtanhs"]
scilab2ccode.deps.stanhs=[]
scilab2ccode.deps.stanha=["stanhs"]
scilab2ccode.deps.ztanhs=["zreals","zimags","ztans","DoubleComplex"]
scilab2ccode.deps.ztanha=["ztanhs"]



///////////////////////////////
////// MATRIX OPERATIONS //////
///////////////////////////////


//OpRc
scilab2ccode.deps.crowcats=[]
scilab2ccode.deps.crowcata=[]
scilab2ccode.deps.drowcats=[]
scilab2ccode.deps.drowcata=[]
scilab2ccode.deps.srowcats=[]
scilab2ccode.deps.srowcata=[]
scilab2ccode.deps.zrowcats=[]
scilab2ccode.deps.zrowcata=[]


//OpCc
scilab2ccode.deps.ccolumncats=[]
scilab2ccode.deps.ccolumncata=[]
scilab2ccode.deps.dcolumncats=[]
scilab2ccode.deps.dcolumncata=[]
scilab2ccode.deps.scolumncats=[]
scilab2ccode.deps.scolumncata=[]
scilab2ccode.deps.zcolumncats=[]
scilab2ccode.deps.zcolumncata=[]


//chol
scilab2ccode.deps.cchola=["DoubleComplex","creals","cimags","FloatComplex","zreals","zimags","cdiffs","cmuls","crdivs","csqrts"]
scilab2ccode.deps.dchols=["dsqrts"]
scilab2ccode.deps.dchola=[]
scilab2ccode.deps.schols=["ssqrts"]
scilab2ccode.deps.schola=["ssqrts"]
scilab2ccode.deps.zchola=["DoubleComplex","zreals","zimags","zdiffs","zmuls","zrdivs","zsqrts"]


//determ
scilab2ccode.deps.cdeterma=["cdiffs","FloatComplex","cmuls","cadds","DoubleComplex","creals","cimags","zreals","zimags","zmuls","crdivs"]
scilab2ccode.deps.ddeterma=[]
scilab2ccode.deps.sdeterma=[]
scilab2ccode.deps.zdeterma=["zdiffs","zmuls","DoubleComplex","zadds","zreals","zimags","zrdivs"]


//dist
scilab2ccode.deps.cdists=["spows","creals","cimags","ssqrts"]
scilab2ccode.deps.cdista=["spows","creals","cimags","ssqrts"]
scilab2ccode.deps.ddists=["dpows","dsqrts"]
scilab2ccode.deps.ddista=["dpows","dsqrts"]
scilab2ccode.deps.sdists=["spows","ssqrts"]
scilab2ccode.deps.sdista=["spows","ssqrts"]
scilab2ccode.deps.zdists=["dpows","zreals","zimags","dsqrts"]
scilab2ccode.deps.zdista=["dpows","zreals","zimags","dsqrts"]


//OpSlash
scilab2ccode.deps.crdivma=["DoubleComplex","creals","cimags","zrdivma","FloatComplex","zreals","zimags"]
scilab2ccode.deps.drdivma=["dtransposea"]
scilab2ccode.deps.srdivma=["drdivma"]
scilab2ccode.deps.zrdivma=["ztransposea","zconja","DoubleComplex","zreals","zimags"]


//OpBackSlash
scilab2ccode.deps.cldivma=["DoubleComplex","creals","cimags","zldivma","FloatComplex","zreals","zimags"]
scilab2ccode.deps.dldivma=[]
scilab2ccode.deps.sldivma=["dldivma"]
scilab2ccode.deps.zldivma=[]


//expm
scilab2ccode.deps.cexpma=["sfrexps","cinfnorma","spows","crdivs","FloatComplex","ceyea","cmuls","cadda","cdiffa","cmulma","cldivma"]
scilab2ccode.deps.dexpma=["dfrexps","dinfnorma","dpows","deyea","dadda","ddiffa","dmulma","dldivma"]
scilab2ccode.deps.sexpma=["sfrexps","sinfnorma","spows","seyea","sadda","sdiffa","smulma","sldivma"]
scilab2ccode.deps.zexpma=["dfrexps","zinfnorma","zrdivs","DoubleComplex","zeyea","zmuls","zadda","zdiffa","zmulma","zldivma"]


//eye
scilab2ccode.deps.ceyea= ["FloatComplex"]
scilab2ccode.deps.deyea= []
scilab2ccode.deps.seyea= []
scilab2ccode.deps.zeyea= ["DoubleComplex"]


//fill
scilab2ccode.deps.cfilla=["conesa","cmuls"]
scilab2ccode.deps.dfilla=["donesa"]
scilab2ccode.deps.sfilla=["sonesa"]
scilab2ccode.deps.zfilla=["zonesa","zmuls"]


//hilbert
scilab2ccode.deps.dhilberta=[]
scilab2ccode.deps.shilberta=[]

//infinite norm
scilab2ccode.deps.cinfnorma=["spythags","creals","cimags"]
scilab2ccode.deps.dinfnorma=[]
scilab2ccode.deps.sinfnorma=[]
scilab2ccode.deps.zinfnorma=["dpythags","zreals","zimags"]


//inversion
scilab2ccode.deps.cinverma=["DoubleComplex","creals","cimags","zinverma","FloatComplex","zreals","zimags"]
scilab2ccode.deps.dinverma=[]
scilab2ccode.deps.sinverma=["dinverma"]
scilab2ccode.deps.zinverma=[]


//jmat


//logm
scilab2ccode.deps.clogma=["DoubleComplex","creals","cimags","zlogma","FloatComplex","zreals","zimags"]
scilab2ccode.deps.dlogma=["DoubleComplex","zlogma"]
scilab2ccode.deps.slogma=["DoubleComplex","zlogma","FloatComplex","zreals","zimags"]
scilab2ccode.deps.zlogma=["ztransposea","zreals","zimags","zlogs","DoubleComplex","zmulma","zinverma"]


//magnitude
scilab2ccode.deps.cmagns=["creals","cimags","ssqrts"]
scilab2ccode.deps.cmagna=["cmagns"]
scilab2ccode.deps.dmagns=[]
scilab2ccode.deps.dmagna=["dmagns"]
scilab2ccode.deps.smagns=[]
scilab2ccode.deps.smagna=["smagns"]
scilab2ccode.deps.zmagns=["zreals","zimags","dsqrts"]
scilab2ccode.deps.zmagna=["zmagns"]


//OpStar
scilab2ccode.deps.cmulma=["FloatComplex","cadds","cmuls"]
scilab2ccode.deps.dmulma=[]
scilab2ccode.deps.smulma=[]
scilab2ccode.deps.zmulma=["zreala","zimaga","DoubleComplex","zadds","zmuls"]


//ones
scilab2ccode.deps.conesa=["FloatComplex"]
scilab2ccode.deps.donesa=[]
scilab2ccode.deps.sonesa=[]
scilab2ccode.deps.zonesa=["DoubleComplex"]


//Squared Magnitude
scilab2ccode.deps.csquMagns=["creals","cimags"]
scilab2ccode.deps.csquMagna=["cmagna"]
scilab2ccode.deps.dsquMagns=[]
scilab2ccode.deps.dsquMagna=["dmagna"]
scilab2ccode.deps.ssquMagns=[]
scilab2ccode.deps.ssquMagna=["smagna"]
scilab2ccode.deps.zsquMagns=["zreals","zimags"]
scilab2ccode.deps.zsquMagna=["zmagna"]


//trace
scilab2ccode.deps.ctracea=["creals","cimags","FloatComplex"]
scilab2ccode.deps.dtracea=[]
scilab2ccode.deps.stracea=[]
scilab2ccode.deps.ztracea=["zreals","zimags","DoubleComplex"]


//transpose
scilab2ccode.deps.ctransposea=["creals","cimags","FloatComplex"]
scilab2ccode.deps.dtransposea=[]
scilab2ccode.deps.stransposea=[]
scilab2ccode.deps.ztransposea=["zreals","zimags","DoubleComplex"]




////////////////////////
////// OPERATIONS //////
////////////////////////


//OpPlus
scilab2ccode.deps.cadds=["creals","cimags","FloatComplex"]
scilab2ccode.deps.cadda=["cadds"]
scilab2ccode.deps.dadds=[]
scilab2ccode.deps.dadda=["dadds"]
scilab2ccode.deps.sadds=[]
scilab2ccode.deps.sadda=["sadds"]
scilab2ccode.deps.zadds=["zreals","zimags","DoubleComplex"]
scilab2ccode.deps.zadda=["zadds"]


//OpDotSlash/OpSlash
scilab2ccode.deps.crdivs=["FloatComplex"]
scilab2ccode.deps.crdiva=["crdivs"]
scilab2ccode.deps.drdivs=[]
scilab2ccode.deps.drdiva=["drdivs"]
scilab2ccode.deps.srdivs=[]
scilab2ccode.deps.srdiva=["srdivs"]
scilab2ccode.deps.zrdivs=["DoubleComplex"]
scilab2ccode.deps.zrdiva=["zrdivs"]


//OpBackSlash
scilab2ccode.deps.cldivs=["cmuls","cconjs","creals","cimags","FloatComplex","crdivs"]
scilab2ccode.deps.cldiva=["cldivs"]
scilab2ccode.deps.dldivs=[]
scilab2ccode.deps.dldiva=["dldivs"]
scilab2ccode.deps.sldivs=[]
scilab2ccode.deps.sldiva=["scldivs"]
scilab2ccode.deps.zldivs=["zmuls","zconjs","zreals","zimags","DoubleComplex","zrdivs"]
scilab2ccode.deps.zldiva=["zldivs"]


//OpStar/OpDotStar
scilab2ccode.deps.cmuls=["FloatComplex"]
scilab2ccode.deps.cmula=["cmuls"]
scilab2ccode.deps.dmuls=[]
scilab2ccode.deps.dmula=["dmuls"]
scilab2ccode.deps.smuls=[]
scilab2ccode.deps.smula=["smuls"]
scilab2ccode.deps.zmuls=["DoubleComplex"]
scilab2ccode.deps.zmula=["zmuls"]


//OpMinus
scilab2ccode.deps.cdiffs=["creals","cimags","FloatComplex"]
scilab2ccode.deps.cdiffa=["cdiffs"]
scilab2ccode.deps.ddiffs=[]
scilab2ccode.deps.ddiffa=["ddiffs"]
scilab2ccode.deps.sdiffs=[]
scilab2ccode.deps.sdiffa=["sdiffs"]
scilab2ccode.deps.zdiffs=["zreals","zimags","DoubleComplex"]
scilab2ccode.deps.zdiffa=["zdiffs"]


///////////////////////////////
////// SIGNAL PROCESSING //////
///////////////////////////////


//conv
scilab2ccode.deps.cconva=["FloatComplex","cfftma","cmula","cifftma"]
scilab2ccode.deps.dconva=["DoubleComplex","zconva","zreala"]
scilab2ccode.deps.sconva=["FloatComplex","cconva","creala"]
scilab2ccode.deps.zconva=["DoubleComplex","zfftma","zmula","zifftma"]


//conv2d
scilab2ccode.deps.cconv2da=["FloatComplex","cadds","cmuls"]
scilab2ccode.deps.dconv2da=[]
scilab2ccode.deps.sconv2da=[]
scilab2ccode.deps.zconv2da=["DoubleComplex","zadds","zmuls"]


//cross correlation
scilab2ccode.deps.ccrossCorra=["cconjs","cconv2da"]
scilab2ccode.deps.dcrossCorra=["dconv2da"]
scilab2ccode.deps.scrossCorra=["sconv2da"]
scilab2ccode.deps.zcrossCorra=["zconjs","zconv2da"]


//fft
scilab2ccode.deps.cfftma=["DoubleComplex","creals","cimags","zfftma","FloatComplex","zreals","zimags"]
scilab2ccode.deps.dfft2=["dfftbi"]
scilab2ccode.deps.dfftbi=["dfftmx"]
scilab2ccode.deps.dfftmx=[]
scilab2ccode.deps.fft842=["r2tx","r4tx","r8tx","DoubleComplex","zreals","zimags"]
scilab2ccode.deps.r2tx=["zadds","zdiffs","DoubleComplex","zreals","zimags"]
scilab2ccode.deps.r4tx=["zadds","zdiffs","DoubleComplex","zreals","zimags"]
scilab2ccode.deps.r8tx=["zadds","zdiffs","DoubleComplex","zreals","zimags"]
scilab2ccode.deps.zfftma=["zreala","zimaga","fft842","dfft2","DoubleComplexMatrix"]


//hilbert
scilab2ccode.deps.dhilberts=[]
scilab2ccode.deps.dhilberta=["DoubleComplex","zfftma","zmuls","zifftma"]
scilab2ccode.deps.shilberts=[]
scilab2ccode.deps.shilberta=["FloatComplex","cfftma","cmuls","cifftma"]


//ifft
scilab2ccode.deps.cifftma=["DoubleComplex","creals","cimags","zifftma","FloatComplex","zreals","zimags"]
scilab2ccode.deps.difft2=["difftbi"]
scilab2ccode.deps.difftbi=["difftmx"]
scilab2ccode.deps.difftmx=[]
scilab2ccode.deps.ifft842=["ir2tx","ir4tx","ir8tx","DoubleComplex","zreals","zimags"]
scilab2ccode.deps.ir2tx=["zadds","zdiffs","DoubleComplex","zreals","zimags"]
scilab2ccode.deps.ir4tx=["zadds","zdiffs","DoubleComplex","zreals","zimags"]
scilab2ccode.deps.ir8tx=["zadds","zdiffs","DoubleComplex","zreals","zimags"]
scilab2ccode.deps.zifftma=["zreala","zimaga","ifft842","difft2","DoubleComplexMatrix"]


//levin
scilab2ccode.deps.dlevina=["dinitTab","dr1","dr2","dr3","dr4","dlevinmul","dinverma","dmulma","dlevinmul2","ddecalage","dlevinsub","dlevinsig"]
scilab2ccode.deps.slevina=["sinitTab","sr1","sr2","sr3","sr4","slevinmul","sinverma","smulma","slevinmul2","sdecalage","slevinsub","slevinsig"]
scilab2ccode.deps.dinitTab=[]
scilab2ccode.deps.dr1=[]
scilab2ccode.deps.dr2=[]
scilab2ccode.deps.dr3=[]
scilab2ccode.deps.dr4=[]
scilab2ccode.deps.dlevinmul=[]
scilab2ccode.deps.dlevinmul2=[]
scilab2ccode.deps.ddecalage=[]
scilab2ccode.deps.dlevinsub=[]
scilab2ccode.deps.dlevinsig=[]
scilab2ccode.deps.sinitTab=[]
scilab2ccode.deps.sr1=[]
scilab2ccode.deps.sr2=[]
scilab2ccode.deps.sr3=[]
scilab2ccode.deps.sr4=[]
scilab2ccode.deps.slevinmul=[]
scilab2ccode.deps.slevinmul2=[]
scilab2ccode.deps.sdecalage=[]
scilab2ccode.deps.slevinsub=[]
scilab2ccode.deps.slevinsig=[]


//lpc2cep
scilab2ccode.deps.clpc2cepa=["cfftma","clogma","cifftma"]
scilab2ccode.deps.dlpc2cepa=["DoubleComplex","zfftma","zlogma","zifftma","zreala"]
scilab2ccode.deps.slpc2cepa=["FloatComplex","cfftma","clogma","cifftma","creala"]
scilab2ccode.deps.zlpc2cepa=["zfftma","zlogma","zifftma"]


/////////////////////////////////
////// STATISTIC FUNCTIONS //////
/////////////////////////////////


//mean
scilab2ccode.deps.cmeana=["FloatComplex","cadds","creals","cimags"]
scilab2ccode.deps.dmeana=[]
scilab2ccode.deps.smeana=[]
scilab2ccode.deps.zmeana=["DoubleComplex","zadds","zreals","zimags"]
scilab2ccode.deps.ccolumnmeana=["ccolumnsuma","crdivs","FloatComplex"]
scilab2ccode.deps.dcolumnmeana=["dcolumnsuma"]
scilab2ccode.deps.scolumnmeana=["scolumnsuma"]
scilab2ccode.deps.zcolumnmeana=["zcolumnsuma","zrdivs","DoubleComplex"]
scilab2ccode.deps.crowmeana=["crowsuma","crdivs","FloatComplex"]
scilab2ccode.deps.drowmeana=["drowsuma"]
scilab2ccode.deps.srowmeana=["srowsuma"]
scilab2ccode.deps.zrowmeana=["zrowsuma","zrdivs","DoubleComplex"]


//prod
scilab2ccode.deps.cproda=["cmuls"]
scilab2ccode.deps.dproda=[]
scilab2ccode.deps.sproda=[]
scilab2ccode.deps.zproda=["zmuls"]
scilab2ccode.deps.ccolumnproda=["cmuls"]
scilab2ccode.deps.dcolumnproda=[]
scilab2ccode.deps.scolumnproda=[]
scilab2ccode.deps.zcolumnproda=["zmuls"]
scilab2ccode.deps.crowproda=["cmuls"]
scilab2ccode.deps.drowproda=[]
scilab2ccode.deps.srowproda=[]
scilab2ccode.deps.zrowproda=["zmuls"]


//sum
scilab2ccode.deps.csuma=["cadds"]
scilab2ccode.deps.dsuma=[]
scilab2ccode.deps.ssuma=[]
scilab2ccode.deps.zsuma=["zadds"]
scilab2ccode.deps.ccolumnsuma=["cadds"]
scilab2ccode.deps.dcolumnsuma=[]
scilab2ccode.deps.scolumnsuma=[]
scilab2ccode.deps.zcolumnsuma=["zadds"]
scilab2ccode.deps.crowsuma=["cadds"]
scilab2ccode.deps.drowsuma=[]
scilab2ccode.deps.srowsuma=[]
scilab2ccode.deps.zrowsuma=["zadds"]


//variance
scilab2ccode.deps.cvariancea=["FloatComplex","cmeana","cdiffs","cadds","cpows","creals","cimags","crdivs"]
scilab2ccode.deps.dvariancea=["dmeana","dpows"]
scilab2ccode.deps.svariancea=["smeana","spows"]
scilab2ccode.deps.zvariancea=["DoubleComplex","zmeana","zdiffs","zadds","zpows","zreals","zimags","zrdivs"]
scilab2ccode.deps.ccolumnvariancea=["cvariancea"]
scilab2ccode.deps.dcolumnvariancea=["dvariancea"]
scilab2ccode.deps.scolumnvariancea=["svariancea"]
scilab2ccode.deps.zcolumnvariancea=["zvariancea"]
scilab2ccode.deps.crowvariancea=["ctransposea","FloatComplex","creals","cimags","cvariancea"]
scilab2ccode.deps.drowvariancea=["dtransposea","dvariancea"]
scilab2ccode.deps.srowvariancea=["stransposea","svariancea"]
scilab2ccode.deps.zrowvariancea=["ztransposea","DoubleComplex","zreals","zimags","zvariancea"]


////////////////////
////// STRING //////
////////////////////


//disp
scilab2ccode.deps.cdisps=["creals","cimags"]
scilab2ccode.deps.cdispa=["creals","cimags"]
scilab2ccode.deps.ddisps=[]
scilab2ccode.deps.ddispa=[]
scilab2ccode.deps.sdisps=[]
scilab2ccode.deps.sdispa=[]
scilab2ccode.deps.zdisps=["zreals","zimags"]
scilab2ccode.deps.zdispa=["zreals","zimags"]


///////////////////
////// TYPES //////
///////////////////


//floatComplex
scilab2ccode.deps.creals=[]
scilab2ccode.deps.cimags=[]
scilab2ccode.deps.creala=["creals"]
scilab2ccode.deps.cimaga=["cimags"]
scilab2ccode.deps.FloatComplex=[]
scilab2ccode.deps.FloatComplexMatrix=["FloatComplex"]
scilab2ccode.deps.cisreals=["cimags"]
scilab2ccode.deps.cisimags=["creals"]


//doubleComplex
scilab2ccode.deps.zreals=[]
scilab2ccode.deps.zimags=[]
scilab2ccode.deps.zreala=["zreals"]
scilab2ccode.deps.zimaga=["zimags"]
scilab2ccode.deps.DoubleComplex=[]
scilab2ccode.deps.DoubleComplexMatrix=["DoubleComplex"]
scilab2ccode.deps.zisreals=["zimags"]
scilab2ccode.deps.zisimags=["zreals"]

endfunction

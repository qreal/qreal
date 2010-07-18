function INIT_FillSCI2LibCDirs(FileInfo,SharedInfoExtension)
// function INIT_FillSCI2LibCDirs(FileInfo,SharedInfoExtension)
// -----------------------------------------------------------------
// #RNU_RES_B
// Generates files for the SCI2CLib CFunctionList and CAnnotations
// directories.
//
// Input data:
// FileInfo: structure containing all info about SCI2C files.
// SharedInfoExtension: structure containing the file extensions.
//
// Output data:
// ---
//
// #RNU_RES_E
// Status:
// 24-Dec-2007 -- Raffaele Nutricato: Author.
// 24-Dec-2007 -- Alberto Morea: Test Ok.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),2,2);

// -----------------------
// --- Initialization. ---
// -----------------------
SCI2CLibCAnnClsDir    = FileInfo.SCI2CLibCAnnCls;
ExtensionCAnnCls      = SharedInfoExtension.AnnotationClasses;

SCI2CLibCAnnFunDir    = FileInfo.SCI2CLibCAnnFun;
ExtensionCAnnFun      = SharedInfoExtension.AnnotationFunctions;

SCI2CLibCFLClsDir     = FileInfo.SCI2CLibCFLCls;
ExtensionCFuncListCls = SharedInfoExtension.FuncListClasses;

SCI2CLibCFLFunDir     = FileInfo.SCI2CLibCFLFun;
ExtensionCFuncListFun = SharedInfoExtension.FuncListFunctions;

GeneralReport         = FileInfo.GeneralReport;
ArgSeparator          = ',';
// ---------------------------
// --- End Initialization. ---
// ---------------------------

// --------------------------------------------------------------------------------
// --- Generate Function List class files for C functions of the SCI2C library. ---
// --------------------------------------------------------------------------------
//NUT old call to INIT_GenSCI2CLibCFLCls
// INIT_GenSCI2CLibCFLCls(SCI2CLibCFLClsDir,ExtensionCFuncListCls,GeneralReport)
//                        SCI2CLibCFLClsDir,ExtensionCFLCls,GeneralReport

//NUT verifica le annotazioni di tutte le classi.

// -----------------------------------------------------------------------------------------
// --- Generate Function List and Annotation files for C functions of the SCI2C library. ---
// -----------------------------------------------------------------------------------------


// ---------------------
// --- Class Global. ---
// ---------------------
ClassName = 'Global';
// #RNU_RES_B
//NUT: global function can work with a generic number of input arguments.
//NUT: we force the global function to work with one input argument only.
// --- Class Annotation. ---
// #RNU_RES_E
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    ''d''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= ''1''',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('g2'+ArgSeparator+'d0',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'global';
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// ---------------------
// --- Class Float. ---
// ---------------------
ClassName = 'Float';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    ''s''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= IN(1).SZ(1)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= IN(1).SZ(2)',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'s0',ClassFileName,'file','y');
//PrintStringInfo('c0'+ArgSeparator+'s0',ClassFileName,'file','y');
//PrintStringInfo('z0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'s2',ClassFileName,'file','y');
//PrintStringInfo('c2'+ArgSeparator+'s2',ClassFileName,'file','y');
//PrintStringInfo('z2'+ArgSeparator+'s2',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'float';
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// ---------------------
// --- Class Double. ---
// ---------------------
ClassName = 'Double';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    ''d''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= IN(1).SZ(1)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= IN(1).SZ(2)',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'d0',ClassFileName,'file','y');
//PrintStringInfo('c0'+ArgSeparator+'d0',ClassFileName,'file','y');
//PrintStringInfo('z0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('s2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'d2',ClassFileName,'file','y');
//PrintStringInfo('c2'+ArgSeparator+'d2',ClassFileName,'file','y');
//PrintStringInfo('z2'+ArgSeparator+'d2',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'double';
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// ----------------------------
// --- Class FloatComplex. ---
// ----------------------------
//NUT sulla complex c'e' da capire se servono due o un solo argomento.
//NUT secondo me ne va bene uno perche' serve per fare il casting di una variabile
//NUT reale in una variabile complessa
ClassName = 'FloatComplex';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    ''c''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= IN(1).SZ(1)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= IN(1).SZ(2)',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('s2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2'+ArgSeparator+'c2',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'floatcomplex';
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// ----------------------------
// --- Class DoubleComplex. ---
// ----------------------------
//NUT sulla complex c'e' da capire se servono due o un solo argomento.
//NUT secondo me ne va bene uno perche' serve per fare il casting di una variabile
//NUT reale in una variabile complessa
ClassName = 'DoubleComplex';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    ''z''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= IN(1).SZ(1)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= IN(1).SZ(2)',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('c0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('z0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('s2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('c2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2'+ArgSeparator+'z2',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'doublecomplex';
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// ------------------
// --- Class Sin. ---
// ------------------
ClassName = 'Sin';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= IN(1).SZ(1)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= IN(1).SZ(2)',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2'+ArgSeparator+'z2',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'sin'; //BJ : Done AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'sinh'; //BJ : Done AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'asinh'; //BJ : Done AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'cos'; //BJ : Done AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'cosh'; //BJ : Done AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'tan'; //BJ : Done AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'tanh'; //BJ : Done AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'exp'; //BJ : Done AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'conj'; //BJ : Done AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

//NUT verifica
FunctionName = 'inv'; //BJ : Done AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'ceil'; //BJ : Done AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'fix'; //BJ : Done AS : Float_Done 
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'floor'; //BJ : Done AS : Float_Done 
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'round'; //BJ : Done AS : Float_Done 
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'int'; //BJ : Done AS : Float_Done 
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'OpLogNot'; //BJ : Done AS : Float_Done 
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'SCI2Cresize';
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'sign'; //BJ : Done AS : Float_Done 
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'chol'; //BJ : Done AS : Float_Done 
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// -------------------
// --- Class Atan. ---
// -------------------
ClassName = 'Atan';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= IN(1).SZ(1)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= IN(1).SZ(2)',ClassFileName,'file','y');

PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= IN(1).SZ(1)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= IN(1).SZ(2)',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('s0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
// PrintStringInfo('c0c0'+ArgSeparator+'c0',ClassFileName,'file','y');
// PrintStringInfo('z0z0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('s2s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2d2'+ArgSeparator+'d2',ClassFileName,'file','y');
// PrintStringInfo('c2c2'+ArgSeparator+'c2',ClassFileName,'file','y');
// PrintStringInfo('z2z2'+ArgSeparator+'z2',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'atan'; //BJ : atan AS : Float_Done 
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// -------------------
// --- Class Sqrt. ---
// -------------------
ClassName = 'Sqrt';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
//Was FA_TP_USER
//Cause some trouble if user specify some precision and if input(and also output) is complex.
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y'); //FOR INRIA changed from IN(1).TP to FA_TP_USER
PrintStringInfo('OUT(1).SZ(1)= IN(1).SZ(1)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= IN(1).SZ(2)',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);

PrintStringInfo('s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('s0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z0'+ArgSeparator+'z0',ClassFileName,'file','y');

PrintStringInfo('s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('s2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2'+ArgSeparator+'z2',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'asin'; //BJ : Works but not able to generate all cases , AS : same for float
                       //--> asin(%pi).
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'acos'; //BJ : Works but not able to generate all cases , AS : same for float
		       //--> acos(%pi)
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'acosh'; // BJ : Done AS : Float_Done 
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'atanh'; // BJ : Done AS : Float_Done 
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'sqrt'; // BJ : Done AS : Float_Done 
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'log'; // BJ : Done AS : Float_Done 
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'log10'; // BJ : Done AS : Float_Done 
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'log1p'; // BJ : Ok AS : Float_Done 
			//--> log1p(%i) Not implemented in Scilab.
                        //WARNING z0log1pz0 will never happened.
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// --------------------
// --- Class Zeros. ---
// --------------------
ClassName = 'Zeros';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          0',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1',ClassFileName,'file','y');
//Was FA_TP_USER
//Cause some trouble if user specify some precision and if input(and also output) is complex.
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= ''1''',ClassFileName,'file','y');

PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1',ClassFileName,'file','y');
//Was FA_TP_USER
//Cause some trouble if user specify some precision and if input(and also output) is complex.
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= FA_SZ_1(IN(1).SZ)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= FA_SZ_2(IN(1).SZ)',ClassFileName,'file','y');

PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1',ClassFileName,'file','y');
//Was FA_TP_USER
//Cause some trouble if user specify some precision and if input(and also output) is complex.
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= FA_SZ_FROM_VAL(IN(1).VAL,IN(1).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= FA_SZ_FROM_VAL(IN(2).VAL,IN(2).TP)',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);

PrintStringInfo(ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo(ArgSeparator+'d0',ClassFileName,'file','y');

PrintStringInfo('s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2'+ArgSeparator+'z2',ClassFileName,'file','y');

//NUT: no mixed input types are allowed.
PrintStringInfo('s0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('s0s0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('s0s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('s0s0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('d0d0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('d0d0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d0d0'+ArgSeparator+'d2',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'zeros'; // BJ : Done AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'ones'; // BJ : Done AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'eye'; // BJ : Done AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

//NUT rand  function doesn't behave like zeros and ones functions.
FunctionName = 'rand'; // BJ : Done  =>  rien pour f loat
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// --------------------
// --- Class Sum. ---
// --------------------
ClassName = 'Sum';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= ''1''',ClassFileName,'file','y');

PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= FA_SZ_SEL1(IN(1).SZ(1),IN(2).VAL)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= FA_SZ_SEL2(IN(1).SZ(2),IN(2).VAL)',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('s2'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c2'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z2'+ArgSeparator+'z0',ClassFileName,'file','y');

PrintStringInfo('s0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('c0s0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z0d0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('s2s0'+ArgSeparator+'s0',ClassFileName,'file','y'); //* possible ? */
PrintStringInfo('d2d0'+ArgSeparator+'d0',ClassFileName,'file','y'); //* possible ? */
PrintStringInfo('c2s0'+ArgSeparator+'c0',ClassFileName,'file','y'); //* possible ? */
PrintStringInfo('z2d0'+ArgSeparator+'z0',ClassFileName,'file','y'); //* possible ? */
PrintStringInfo('s2s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2d0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c2s0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2d0'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('s0g2'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0g2'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0g2'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z0g2'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('s2g2'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d2g2'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c2g2'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z2g2'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('s2g2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2g2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c2g2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2g2'+ArgSeparator+'z2',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'sum'; // BJ : Done AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'prod'; // BJ : Done AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'mean'; // BJ : Done AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'st_deviation'; // BJ : Not implemented
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'variance'; // BJ : Done AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);



// --------------------
// --- Class Max. ---
// --------------------
ClassName = 'Max';


// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= ''1''',ClassFileName,'file','y');

PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_MAX(IN(1).TP,IN(2).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= FA_SZ_1(FA_SZ_OPPLUS(IN(1).SZ,IN(2).SZ,IN(1).TP,IN(2).TP))',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= FA_SZ_2(FA_SZ_OPPLUS(IN(1).SZ,IN(2).SZ,IN(1).TP,IN(2).TP))',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('s2'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'d0',ClassFileName,'file','y');


PrintStringInfo('s0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('s2s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('s0s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('s2s2'+ArgSeparator+'s2',ClassFileName,'file','y');

PrintStringInfo('d0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('d2d0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('d0d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('d2d2'+ArgSeparator+'d2',ClassFileName,'file','y');

FunctionName = 'max'; // BJ : Done AS : Float_Done
		      // WARNING : Complex case will never be
                      // WARNING : max(x, 2) <=> max(x, 2 * ones(x)) nothing to do with 'r' nor 'c'
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'min'; // BJ : Done AS : Float_Done
		      // WARNING : Complex case will never be
                      // WARNING : max(x, 2) <=> max(x, 2 * ones(x)) nothing to do with 'r' nor 'c'
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);





// --------------------
// --- Class Abs. ---
// --------------------
ClassName = 'Abs';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_REAL(IN(1).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= IN(1).SZ(1)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= IN(1).SZ(2)',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('z0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('z2'+ArgSeparator+'d2',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'abs'; // BJ : Done AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'real'; // BJ : Done AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'imag'; // BJ : Done AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// ------------------------
// --- Class OpDotStar. ---
// ------------------------
ClassName = 'OpDotStar';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_MAX(IN(1).TP,IN(2).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= FA_SZ_1(FA_SZ_OPDOTSTAR(IN(1).SZ,IN(2).SZ))',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= FA_SZ_2(FA_SZ_OPDOTSTAR(IN(1).SZ,IN(2).SZ))',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('s0c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('c0s0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z0z0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('d0z0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('z0d0'+ArgSeparator+'z0',ClassFileName,'file','y');

PrintStringInfo('s2s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2d0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c2c0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('s2c0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2s0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2z0'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2d0'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('d2z0'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('s0s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d0d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c0c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('s0c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c0s2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z0z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('d0z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z0d2'+ArgSeparator+'z2',ClassFileName,'file','y');
   
PrintStringInfo('s2s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c2c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('s2c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2s2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('d2z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2d2'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('s2s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c2c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('s2c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2s2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('d2z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2d2'+ArgSeparator+'z2',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'OpDotStar'; // BJ : Done AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'OpDotSlash'; // BJ : Done AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);


// ------------------------
// --- Class OpDotHat. ---
// ------------------------
ClassName = 'OpDotHat';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_MAX(IN(1).TP,IN(2).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= FA_SZ_1(FA_SZ_OPDOTSTAR(IN(1).SZ,IN(2).SZ))',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= FA_SZ_2(FA_SZ_OPDOTSTAR(IN(1).SZ,IN(2).SZ))',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z0z0'+ArgSeparator+'z0',ClassFileName,'file','y');

PrintStringInfo('s2s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2d0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c2c0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2z0'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('s0s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d0d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c0c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z0z2'+ArgSeparator+'z2',ClassFileName,'file','y');
   
PrintStringInfo('s2s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c2c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2z2'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('s2c0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d2z0'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('c2s0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2d0'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('s0c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d0z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('c0s2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z0d2'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('s0c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('d0z0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('c0s0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z0d0'+ArgSeparator+'z0',ClassFileName,'file','y');

PrintStringInfo('s2c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d2z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('c2s2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2d2'+ArgSeparator+'z2',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'OpDotHat'; // BJ : Done AS : Float_Done but poor precision
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

//NUT verifica l'hat se e' ben annotato. L'input deve essere una matrice quadrata
FunctionName = 'OpHat'; // BJ : Done AS : Float_Done but poor precision
			// WARNING : d2d2 / z2z2 / d2z2 / z2d2 Not implemented in Scilab ...
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'OpDotBackSlash'; // BJ : Done AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'OpLogLt'; // BJ : Done AS : Float_Done
			  // WARNING : Complex cases are useless.
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'OpLogLe'; // BJ : Done AS : Float_Done
			  // WARNING : Complex cases are useless.
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'OpLogGt'; // BJ : Done AS : Float_Done
			  // WARNING : Complex cases are useless.
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'OpLogGe'; // BJ : Done AS : Float_Done
			  // WARNING : Complex cases are useless.
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);



// ----------------------
// --- Class OpLogEq. ---
// ----------------------
ClassName = 'OpLogEq';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_MIN_REAL(IN(1).TP,IN(2).TP)',ClassFileName,'file','y'); //RNU
PrintStringInfo('OUT(1).SZ(1)= FA_SZ_1(FA_SZ_OPDOTSTAR(IN(1).SZ,IN(2).SZ))',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= FA_SZ_2(FA_SZ_OPDOTSTAR(IN(1).SZ,IN(2).SZ))',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0c0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('z0z0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('g0g0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('g0g0'+ArgSeparator+'d0',ClassFileName,'file','y');

PrintStringInfo('s2s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2d0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c2c0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('z2z0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('g2g0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('g2g0'+ArgSeparator+'d2',ClassFileName,'file','y');

PrintStringInfo('s0s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d0d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c0c2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('z0z2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('g0g2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('g0g2'+ArgSeparator+'d2',ClassFileName,'file','y');
   
PrintStringInfo('s2s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c2c2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('z2z2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('g2g2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('g2g2'+ArgSeparator+'d2',ClassFileName,'file','y');

//mixed types
PrintStringInfo('c2s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('z2d0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('d2z0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('s2c0'+ArgSeparator+'s2',ClassFileName,'file','y');

PrintStringInfo('z0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0z0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('s0c0'+ArgSeparator+'s0',ClassFileName,'file','y');

PrintStringInfo('c0s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('z0d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('d0z2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('s0c2'+ArgSeparator+'s2',ClassFileName,'file','y');

PrintStringInfo('c2s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('z2d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('d2z2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('s2c2'+ArgSeparator+'s2',ClassFileName,'file','y');


// --- Annotation Function And Function List Function. ---
FunctionName = 'OpLogEq'; // BJ : Done AS : Float_Done
			  // WARNING : Unable to generate g2 nor g0 code ...
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'OpLogNe'; // BJ : Done AS : Float_Done
			  // WARNING : Unable to generate g2 nor g0 code ...
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'OpLogAnd'; // AS : Double and Float_Done
			   // ERROR : z0z0OpLogAndz0 -> z0z0OpLogAndd0 
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'OpLogOr'; // AS : Double and Float_Done
			  // ERROR : z0z0OpLogAndz0 -> z0z0OpLogAndd0 
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// ---------------------
// --- Class OpStar. ---
// ---------------------
ClassName = 'OpStar';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_MAX(IN(1).TP,IN(2).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= FA_SZ_1(FA_SZ_OPSTAR(IN(1).SZ,IN(2).SZ))',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= FA_SZ_2(FA_SZ_OPSTAR(IN(1).SZ,IN(2).SZ))',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('s0c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('c0s0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('c0c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('d0z0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('z0d0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('z0z0'+ArgSeparator+'z0',ClassFileName,'file','y');

PrintStringInfo('s2s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2d0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('s2c0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2s0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2c0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d2z0'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2d0'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2z0'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('s0s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d0d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('s0c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c0s2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c0c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d0z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z0d2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z0z2'+ArgSeparator+'z2',ClassFileName,'file','y');
   
PrintStringInfo('s2s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('s2c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2s2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d2z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2d2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2z2'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('s2s2'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d2d2'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('s2c2'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('c2s2'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('c2c2'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('d2z2'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('z2d2'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('z2z2'+ArgSeparator+'z0',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'OpStar'; // BJ : Done AS : Float_Done
			
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);
//NUT vedi che la opstar non funziona quando faccio scalare*matrice. o matrice*scalare.
//NUT la lista delle funzioni disponibili e' identica a quella della classe 3.


// ---------------------
// --- Class OpSlash. ---
// ---------------------
ClassName = 'OpSlash';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_MAX(IN(1).TP,IN(2).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= FA_SZ_1(FA_SZ_OPSLASH(IN(1).SZ,IN(2).SZ))',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= FA_SZ_2(FA_SZ_OPSLASH(IN(1).SZ,IN(2).SZ))',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('s0c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('c0s0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('c0c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('d0z0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('z0d0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('z0z0'+ArgSeparator+'z0',ClassFileName,'file','y');

PrintStringInfo('s2s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2d0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('s2c0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2s0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2c0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d2z0'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2d0'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2z0'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('s0s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d0d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('s0c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c0s2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c0c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d0z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z0d2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z0z2'+ArgSeparator+'z2',ClassFileName,'file','y');
   
PrintStringInfo('s2s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('s2c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2s2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d2z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2d2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2z2'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('s2s2'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d2d2'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('s2c2'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('c2s2'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('c2c2'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('d2z2'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('z2d2'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('z2z2'+ArgSeparator+'z0',ClassFileName,'file','y');


FunctionName = 'OpSlash'; // BJ : Done AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// ---------------------
// --- Class OpApex. ---
// ---------------------
ClassName = 'OpApex';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= IN(1).SZ(2)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= IN(1).SZ(1)',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2'+ArgSeparator+'z2',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'OpApex'; // BJ : Done AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'OpDotApex'; // BJ : Done AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);


// ---------------------
// --- Class IsNan. ---
// ---------------------
ClassName = 'IsNan';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_REAL(IN(1).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= IN(1).SZ(2)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= IN(1).SZ(1)',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('z0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('z2'+ArgSeparator+'d2',ClassFileName,'file','y');


FunctionName = 'isnan'; // BJ : Done AS : Float_Done
			// ERROR : z2isnanz2 must be z2isnand2 // z0isnanz0 must be z0isnand0
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// ---------------------
// --- Class OpPlus. ---
// ---------------------
ClassName = 'OpPlus';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= IN(1).SZ(1)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= IN(1).SZ(2)',ClassFileName,'file','y');
PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_MAX(IN(1).TP,IN(2).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= FA_SZ_1(FA_SZ_OPPLUS(IN(1).SZ,IN(2).SZ,IN(1).TP,IN(2).TP))',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= FA_SZ_2(FA_SZ_OPPLUS(IN(1).SZ,IN(2).SZ,IN(1).TP,IN(2).TP))',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z0'+ArgSeparator+'z0',ClassFileName,'file','y');

PrintStringInfo('s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('s0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('s0c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('c0s0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('c0c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('d0z0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('z0d0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('z0z0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('g0g0'+ArgSeparator+'g2',ClassFileName,'file','y');

PrintStringInfo('s2s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2d0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('s2c0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2s0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2c0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d2z0'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2d0'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2z0'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('g2g0'+ArgSeparator+'g2',ClassFileName,'file','y');

PrintStringInfo('s0s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d0d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('s0c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c0s2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c0c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d0z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z0d2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z0z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('g0g2'+ArgSeparator+'g2',ClassFileName,'file','y');
   
PrintStringInfo('s2s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('s2c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2s2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d2z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2d2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('g2g2'+ArgSeparator+'g2',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'OpPlus'; // BJ : Done AS : Float_Done
			 // ERROR : Strings are not correctly allowed.
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);





// ----------------------
// --- Class OpMinus. ---
// ----------------------
ClassName = 'OpMinus';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= IN(1).SZ(1)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= IN(1).SZ(2)',ClassFileName,'file','y');
PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_MAX(IN(1).TP,IN(2).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= FA_SZ_1(FA_SZ_OPMINUS(IN(1).SZ,IN(2).SZ))',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= FA_SZ_2(FA_SZ_OPMINUS(IN(1).SZ,IN(2).SZ))',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z0'+ArgSeparator+'z0',ClassFileName,'file','y');

PrintStringInfo('s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('s0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('s0c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('c0s0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('c0c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('d0z0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('z0d0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('z0z0'+ArgSeparator+'z0',ClassFileName,'file','y');

PrintStringInfo('s2s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2d0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('s2c0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2s0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2c0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d2z0'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2d0'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2z0'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('s0s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d0d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('s0c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c0s2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c0c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d0z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z0d2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z0z2'+ArgSeparator+'z2',ClassFileName,'file','y');
   
PrintStringInfo('s2s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('s2c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2s2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d2z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2d2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2z2'+ArgSeparator+'z2',ClassFileName,'file','y');


// --- Annotation Function And Function List Function. ---
FunctionName = 'OpMinus'; // AS : Done  AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// -------------------
// --- Class OpRc. ---
// -------------------
ClassName = 'OpRc';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_MAX(IN(1).TP,IN(2).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= IN(1).SZ(1)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= FA_ADD(IN(1).SZ(2),IN(2).SZ(2))',ClassFileName,'file','y');

// --- Function List Class. ---
//NUT: no mixed data types considered
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('s0s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('s2s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('s2s2'+ArgSeparator+'s2',ClassFileName,'file','y');

PrintStringInfo('d0d0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('d0d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('d2d0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('d2d2'+ArgSeparator+'d2',ClassFileName,'file','y');

PrintStringInfo('c0c0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c0c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2c0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2c2'+ArgSeparator+'c2',ClassFileName,'file','y');

PrintStringInfo('z0z0'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z0z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2z0'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2z2'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('z0d0'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2d0'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('c0s0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2s0'+ArgSeparator+'c2',ClassFileName,'file','y');

PrintStringInfo('s0c0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('s2c0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d0z0'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('d2z0'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('s2c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2s2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d2z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2d2'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('s0c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c0s2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d0z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z0d2'+ArgSeparator+'z2',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'OpRc'; // AS : Done  AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// -------------------
// --- Class OpCc. ---
// -------------------
ClassName = 'OpCc';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_MAX(IN(1).TP,IN(2).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= FA_ADD(IN(1).SZ(1),IN(2).SZ(1))',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= IN(1).SZ(2)',ClassFileName,'file','y');

// --- Function List Class. ---
//NUT: no mixed data types considered
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('s0s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('s2s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('s2s2'+ArgSeparator+'s2',ClassFileName,'file','y');

PrintStringInfo('d0d0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('d0d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('d2d0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('d2d2'+ArgSeparator+'d2',ClassFileName,'file','y');

PrintStringInfo('c0c0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c0c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2c0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2c2'+ArgSeparator+'c2',ClassFileName,'file','y');

PrintStringInfo('z0z0'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z0z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2z0'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2z2'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('z0d0'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2d0'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('c0s0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2s0'+ArgSeparator+'c2',ClassFileName,'file','y');

PrintStringInfo('s0c0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('s2c0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d0z0'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('d2z0'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('s2c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2s2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d2z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2d2'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('s0c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c0s2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d0z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z0d2'+ArgSeparator+'z2',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'OpCc'; // AS : Done  AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);


// -------------------
// --- Class Find. ---
// -------------------
ClassName = 'Find';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= FA_SZ_RTMAX(FA_MUL(IN(1).SZ(1),IN(1).SZ(2)))',ClassFileName,'file','y');

PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         2       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= FA_SZ_RTMAX(FA_MUL(IN(1).SZ(1),IN(1).SZ(2)))',ClassFileName,'file','y');
PrintStringInfo('OUT(2).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(2).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(2).SZ(2)= FA_SZ_RTMAX(FA_MUL(IN(1).SZ(1),IN(1).SZ(2)))',ClassFileName,'file','y');

PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= FA_SZ_FROM_VAL(FA_MIN( IN(2).VAL ,FA_MUL(IN(1).SZ(1),IN(1).SZ(2))) ,IN(2).TP)',ClassFileName,'file','y');

PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         2       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= FA_SZ_FROM_VAL(FA_MIN( IN(2).VAL ,FA_MUL(IN(1).SZ(1),IN(1).SZ(2))) ,IN(2).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(2).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(2).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(2).SZ(2)= FA_SZ_FROM_VAL(FA_MIN( IN(2).VAL ,FA_MUL(IN(1).SZ(1),IN(1).SZ(2))) ,IN(2).TP)',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'d2',ClassFileName,'file','y');

PrintStringInfo('s0'+ArgSeparator+'s0s0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'d0d0',ClassFileName,'file','y');
PrintStringInfo('s2'+ArgSeparator+'s2s2',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'d2d2',ClassFileName,'file','y');

PrintStringInfo('s0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('s2s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('d2d0'+ArgSeparator+'d2',ClassFileName,'file','y');

PrintStringInfo('s0s0'+ArgSeparator+'s0s0',ClassFileName,'file','y');
PrintStringInfo('s2s0'+ArgSeparator+'s2s2',ClassFileName,'file','y');
PrintStringInfo('d0d0'+ArgSeparator+'d0d0',ClassFileName,'file','y');
PrintStringInfo('d2d0'+ArgSeparator+'d2d2',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'find'; // AS : Done  AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// ---------------------
// --- Class Length. ---
// ---------------------
ClassName = 'Length';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_USER',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= ''1''',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('z0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('g0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('g0'+ArgSeparator+'d0',ClassFileName,'file','y');

PrintStringInfo('s2'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c2'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('z2'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('g2'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('g2'+ArgSeparator+'d0',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'length'; // AS : done  AS : Float_Done
                         // warning for string input , the length is +1 than scilab because
                         // in C we must take in account the ending '\0'
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'type'; // AS : done  AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// -------------------
// --- Class Size. ---
// -------------------
ClassName = 'Size';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_REAL(IN(1).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= ''2''',ClassFileName,'file','y');

PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         2       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_REAL(IN(1).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(2).TP=    FA_TP_REAL(IN(1).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(2).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(2).SZ(2)= ''1''',ClassFileName,'file','y');

PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_REAL(IN(1).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= ''1''',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('z0'+ArgSeparator+'d2',ClassFileName,'file','y');

PrintStringInfo('s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('z2'+ArgSeparator+'d2',ClassFileName,'file','y');

PrintStringInfo('s0'+ArgSeparator+'s0s0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'d0d0',ClassFileName,'file','y');
PrintStringInfo('c0'+ArgSeparator+'s0s0',ClassFileName,'file','y');
PrintStringInfo('z0'+ArgSeparator+'d0d0',ClassFileName,'file','y');

PrintStringInfo('s2'+ArgSeparator+'s0s0',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'d0d0',ClassFileName,'file','y');
PrintStringInfo('c2'+ArgSeparator+'s0s0',ClassFileName,'file','y');
PrintStringInfo('z2'+ArgSeparator+'d0d0',ClassFileName,'file','y');

PrintStringInfo('s0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('z0d0'+ArgSeparator+'d0',ClassFileName,'file','y');

PrintStringInfo('s2s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d2d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c2s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('z2d0'+ArgSeparator+'d0',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'size'; // AS : Done  AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// ---------------------
// --- Class Return. ---
// ---------------------
ClassName = 'Return';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          0',ClassFileName,'file','y');
PrintStringInfo('NOUT=         0',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo(ArgSeparator,ClassFileName,'file','y');

//NUT anche se metto Return funziona bene comunque! cerca di capire il motivo.
//NUT limited use to zero in and out args only.
// --- Annotation Function And Function List Function. ---
FunctionName = 'return'; 
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// ----------------------
// --- Class OpColon. ---
// ----------------------
ClassName = 'OpColon';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_MIN_REAL(IN(1).TP,IN(2).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= FA_SZ_FROM_VAL(FA_ADD(FA_SUB(FA_REAL(IN(2).VAL,IN(2).TP),FA_REAL(IN(1).VAL,IN(1).TP)),''1''),FA_TP_REAL(IN(2).TP))',ClassFileName,'file','y');
PrintStringInfo('NIN=          3',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_MIN_REAL(IN(1).TP,FA_TP_MIN_REAL(IN(2).TP,IN(3).TP))',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= FA_SZ_FROM_VAL(FA_ADD(FA_DIV(FA_SUB(FA_REAL(IN(3).VAL,IN(3).TP),FA_REAL(IN(1).VAL, IN(1).TP)),FA_REAL(IN(2).VAL,IN(2).TP)),''1''),FA_TP_REAL(IN(3).TP))',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0c0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('z0z0'+ArgSeparator+'d0',ClassFileName,'file','y');

PrintStringInfo('s0s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d0d0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c0c0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('z0z0'+ArgSeparator+'d2',ClassFileName,'file','y');


PrintStringInfo('s0c0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0z0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('z0d0'+ArgSeparator+'d0',ClassFileName,'file','y');


PrintStringInfo('s0c0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d0z0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c0s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('z0d0'+ArgSeparator+'d2',ClassFileName,'file','y');




PrintStringInfo('s0s0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0d0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0c0c0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('z0z0z0'+ArgSeparator+'d0',ClassFileName,'file','y');

PrintStringInfo('s0s0c0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('s0c0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('s0c0c0'+ArgSeparator+'s0',ClassFileName,'file','y');
	
PrintStringInfo('c0s0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('c0c0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('c0s0c0'+ArgSeparator+'s0',ClassFileName,'file','y');

PrintStringInfo('d0z0z0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('d0d0z0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('d0z0d0'+ArgSeparator+'d0',ClassFileName,'file','y');

PrintStringInfo('z0d0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('z0z0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('z0d0z0'+ArgSeparator+'d0',ClassFileName,'file','y');


PrintStringInfo('s0s0s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d0d0d0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c0c0c0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('z0z0z0'+ArgSeparator+'d2',ClassFileName,'file','y');


PrintStringInfo('s0s0c0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('s0c0s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('s0c0c0'+ArgSeparator+'s2',ClassFileName,'file','y');
	
PrintStringInfo('c0s0s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('c0c0s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('c0s0c0'+ArgSeparator+'s2',ClassFileName,'file','y');

PrintStringInfo('d0z0z0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('d0d0z0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('d0z0d0'+ArgSeparator+'d2',ClassFileName,'file','y');

PrintStringInfo('z0d0d0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('z0z0d0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('z0d0z0'+ArgSeparator+'d2',ClassFileName,'file','y');


// --- Annotation Function And Function List Function. ---
FunctionName = 'OpColon'; // AS : done  AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// ----------------------
// --- Class IsEmpty. ---
// ----------------------
ClassName = 'IsEmpty';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_USER',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= ''1''',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('z0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('s2'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c2'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('z2'+ArgSeparator+'d0',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'isempty';
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// ----------------------
// --- Class Trace. ---
// ----------------------
ClassName = 'Trace';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= ''1''',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('s2'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c2'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z2'+ArgSeparator+'z0',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'trace'; // AS : Done  AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

//NUT det is a little bit complex but for the moment we assume that
//NUT that det works as trace function.
FunctionName = 'det'; // AS : Done   AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// --------------------
// --- Class OpIns. ---
// --------------------
ClassName = 'OpIns';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          3',ClassFileName,'file','y');
PrintStringInfo('NOUT=         0',ClassFileName,'file','y');

PrintStringInfo('NIN=          4',ClassFileName,'file','y');
PrintStringInfo('NOUT=         0',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s2s0s0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('s2s2s0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('s2s2s2'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('s2s0s0s0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('s2s0s2s0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('s2s2s0s0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('s2s2s2s0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('s2s0s2s2'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('s2s2s0s2'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('s2s2s2s2'+ArgSeparator,ClassFileName,'file','y');

PrintStringInfo('d2d0d0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('d2d2d0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('d2d2d2'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('d2d0d0d0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('d2d0d2d0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('d2d2d0d0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('d2d2d2d0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('d2d0d2d2'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('d2d2d0d2'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('d2d2d2d2'+ArgSeparator,ClassFileName,'file','y');

PrintStringInfo('c2s0c0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('c2s2c0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('c2s2c2'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('c2s0s0c0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('c2s0s2c0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('c2s2s0c0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('c2s2s2c0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('c2s0s2c2'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('c2s2s0c2'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('c2s2s2c2'+ArgSeparator,ClassFileName,'file','y');

PrintStringInfo('z2d0z0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('z2d2z0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('z2d2z2'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('z2d0d0z0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('z2d0d2z0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('z2d2d0z0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('z2d2d2z0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('z2d0d2z2'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('z2d2d0z2'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('z2d2d2z2'+ArgSeparator,ClassFileName,'file','y');

//Mixed input arguments
PrintStringInfo('s2s0c0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('s2s2c0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('s2s2c2'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('s2s0s0c0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('s2s0s2c0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('s2s2s0c0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('s2s2s2c0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('s2s0s2c2'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('s2s2s0c2'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('s2s2s2c2'+ArgSeparator,ClassFileName,'file','y');

PrintStringInfo('d2d0z0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('d2d2z0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('d2d2z2'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('d2d0d0z0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('d2d0d2z0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('d2d2d0z0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('d2d2d2z0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('d2d0d2z2'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('d2d2d0z2'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('d2d2d2z2'+ArgSeparator,ClassFileName,'file','y');

PrintStringInfo('c2s0s0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('c2s2s0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('c2s2s2'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('c2s0s0s0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('c2s0s2s0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('c2s2s0s0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('c2s2s2s0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('c2s0s2s2'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('c2s2s0s2'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('c2s2s2s2'+ArgSeparator,ClassFileName,'file','y');

PrintStringInfo('z2d0d0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('z2d2d0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('z2d2d2'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('z2d0d0d0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('z2d0d2d0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('z2d2d0d0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('z2d2d2d0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('z2d0d2d2'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('z2d2d0d2'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('z2d2d2d2'+ArgSeparator,ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'OpIns'; // AS : Done  AS : Float_Done	
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// --------------------
// --- Class OpExt. ---
// --------------------
ClassName = 'OpExt';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= FA_MUL(IN(2).SZ(1),IN(2).SZ(2))',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= ''1''',ClassFileName,'file','y');

PrintStringInfo('NIN=          3',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= FA_MUL(IN(2).SZ(1),IN(2).SZ(2))',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= FA_MUL(IN(3).SZ(1),IN(3).SZ(2))',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s2s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('s2s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('s2s0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('s2s2s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('s2s0s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('s2s2s2'+ArgSeparator+'s2',ClassFileName,'file','y');

PrintStringInfo('d2d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('d2d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('d2d0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('d2d2d0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('d2d0d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('d2d2d2'+ArgSeparator+'d2',ClassFileName,'file','y');

PrintStringInfo('c2s0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('c2s2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2s0s0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('c2s2s0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2s0s2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2s2s2'+ArgSeparator+'c2',ClassFileName,'file','y');

PrintStringInfo('z2d0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('z2d2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2d0d0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('z2d2d0'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2d0d2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2d2d2'+ArgSeparator+'z2',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'OpExt'; // AS : Done  AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// -------------------
// --- Class Disp. ---
// -------------------
ClassName = 'Disp';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    ''d''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= ''1''',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('z0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('g0'+ArgSeparator+'d0',ClassFileName,'file','y');

PrintStringInfo('s2'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c2'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('z2'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('g2'+ArgSeparator+'d0',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'disp'; // AS : Done  AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// ----------------------
// --- Class OpEqual. ---
// ----------------------
ClassName = 'OpEqual';
// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= FA_SZ_1(IN(1).SZ)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= FA_SZ_2(IN(1).SZ)',ClassFileName,'file','y');
PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         2       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= FA_SZ_1(IN(1).SZ)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= FA_SZ_2(IN(1).SZ)',ClassFileName,'file','y');
PrintStringInfo('OUT(2).TP=    IN(2).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(2).SZ(1)= FA_SZ_1(IN(2).SZ)',ClassFileName,'file','y');
PrintStringInfo('OUT(2).SZ(2)= FA_SZ_2(IN(2).SZ)',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('g0'+ArgSeparator+'g0',ClassFileName,'file','y');
PrintStringInfo('s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('g2'+ArgSeparator+'g2',ClassFileName,'file','y');
//NUT per ora non considero le equal con nin != 1

// --- Annotation Function And Function List Function. ---
FunctionName = 'OpEqual'; // AS : Done  AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);


// --------------------
// --- Class Mopen. ---
// --------------------
ClassName = 'Mopen';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    ''f''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= ''1''',ClassFileName,'file','y');

PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         2',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    ''f''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(2).TP=    FA_TP_USER',ClassFileName,'file','y');
PrintStringInfo('OUT(2).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(2).SZ(2)= ''1''',ClassFileName,'file','y');

PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    ''f''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= ''1''',ClassFileName,'file','y');

PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         2',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    ''f''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(2).TP=    FA_TP_USER',ClassFileName,'file','y');
PrintStringInfo('OUT(2).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(2).SZ(2)= ''1''',ClassFileName,'file','y');

PrintStringInfo('NIN=          3',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    ''f''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= ''1''',ClassFileName,'file','y');

PrintStringInfo('NIN=          3',ClassFileName,'file','y');
PrintStringInfo('NOUT=         2',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    ''f''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(2).TP=    FA_TP_USER',ClassFileName,'file','y');
PrintStringInfo('OUT(2).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(2).SZ(2)= ''1''',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('g2'+ArgSeparator+'f0',ClassFileName,'file','y');
PrintStringInfo('g2'+ArgSeparator+'f0s0',ClassFileName,'file','y');
PrintStringInfo('g2'+ArgSeparator+'f0d0',ClassFileName,'file','y');
PrintStringInfo('g2g2'+ArgSeparator+'f0',ClassFileName,'file','y');
PrintStringInfo('g2g2'+ArgSeparator+'f0s0',ClassFileName,'file','y');
PrintStringInfo('g2g2'+ArgSeparator+'f0d0',ClassFileName,'file','y');
PrintStringInfo('g2g2s0'+ArgSeparator+'f0s0',ClassFileName,'file','y');
PrintStringInfo('g2g2s0'+ArgSeparator+'f0d0',ClassFileName,'file','y');
PrintStringInfo('g2g2d0'+ArgSeparator+'f0s0',ClassFileName,'file','y');
PrintStringInfo('g2g2d0'+ArgSeparator+'f0d0',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'mopen';
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// -------------------
// --- Class Mput. ---
// -------------------
ClassName = 'Mput';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          3',ClassFileName,'file','y');
PrintStringInfo('NOUT=         0       ',ClassFileName,'file','y');
PrintStringInfo('NIN=          3',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    ''i''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= ''1''',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0g2f0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('s2g2f0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('d0g2f0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('d2g2f0'+ArgSeparator,ClassFileName,'file','y');
PrintStringInfo('s0g2f0'+ArgSeparator+'i0',ClassFileName,'file','y'); //NUT la mput e' strana 
PrintStringInfo('s2g2f0'+ArgSeparator+'i0',ClassFileName,'file','y');
PrintStringInfo('d0g2f0'+ArgSeparator+'i0',ClassFileName,'file','y');
PrintStringInfo('d2g2f0'+ArgSeparator+'i0',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'mput';
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// -------------------
// --- Class Mget. ---
// -------------------
ClassName = 'Mget';
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          3',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_USER',ClassFileName,'file','y'); 
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y'); 
PrintStringInfo('OUT(1).SZ(2)= FA_SZ_RTMAX(FA_SZ_FROM_VAL(IN(1).VAL,IN(1).TP))',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0g2f0'+ArgSeparator+'s0',ClassFileName,'file','y'); //NUT da chiarire
PrintStringInfo('s0g2f0'+ArgSeparator+'d0',ClassFileName,'file','y'); //NUT da chiarire
PrintStringInfo('d0g2f0'+ArgSeparator+'s0',ClassFileName,'file','y'); //NUT da chiarire
PrintStringInfo('d0g2f0'+ArgSeparator+'d0',ClassFileName,'file','y'); //NUT da chiarire
PrintStringInfo('s0g2f0'+ArgSeparator+'s2',ClassFileName,'file','y'); //NUT da chiarire
PrintStringInfo('s0g2f0'+ArgSeparator+'d2',ClassFileName,'file','y'); //NUT da chiarire
PrintStringInfo('d0g2f0'+ArgSeparator+'s2',ClassFileName,'file','y'); //NUT da chiarire
PrintStringInfo('d0g2f0'+ArgSeparator+'d2',ClassFileName,'file','y'); //NUT da chiarire

// --- Annotation Function And Function List Function. ---
FunctionName = 'mget';
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// ---------------------
// --- Class Mclose. ---
// ---------------------
ClassName = 'Mclose';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          0',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    ''i''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= ''1''',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('f0'+ArgSeparator,ClassFileName,'file','y'); //NUT da chiarire
PrintStringInfo('f0'+ArgSeparator+'i0',ClassFileName,'file','y'); //NUT da chiarire

// --- Annotation Function And Function List Function. ---
FunctionName = 'mclose';
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);
  
// ---------------------
// --- Class Mseek. ---
// ---------------------
ClassName = 'Mseek';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         0       ',ClassFileName,'file','y');
PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         0       ',ClassFileName,'file','y');
PrintStringInfo('NIN=          3',ClassFileName,'file','y');
PrintStringInfo('NOUT=         0       ',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0'+ArgSeparator,ClassFileName,'file','y'); //NUT da chiarire
PrintStringInfo('s0f0'+ArgSeparator,ClassFileName,'file','y'); //NUT da chiarire
PrintStringInfo('s0f0g2'+ArgSeparator,ClassFileName,'file','y'); //NUT da chiarire

PrintStringInfo('d0'+ArgSeparator,ClassFileName,'file','y'); //NUT da chiarire
PrintStringInfo('d0f0'+ArgSeparator,ClassFileName,'file','y'); //NUT da chiarire
PrintStringInfo('d0f0g2'+ArgSeparator,ClassFileName,'file','y'); //NUT da chiarire

// --- Annotation Function And Function List Function. ---
FunctionName = 'mseek';
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// ---------------------
// --- Class Convol. ---
// ---------------------
ClassName = 'Convol';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_MAX(IN(1).TP,IN(2).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= FA_SUB(FA_ADD(IN(1).SZ(2),IN(2).SZ(2)),''1'')',ClassFileName,'file','y');

PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         2       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_MAX(IN(1).TP,IN(2).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= IN(2).SZ(2)',ClassFileName,'file','y');
PrintStringInfo('OUT(2).TP=    FA_TP_MAX(IN(1).TP,IN(2).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(2).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(2).SZ(2)= FA_SUB(FA_ADD(IN(1).SZ(2),IN(2).SZ(2)),''1'')',ClassFileName,'file','y');

PrintStringInfo('NIN=          3',ClassFileName,'file','y');
PrintStringInfo('NOUT=         2       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_MAX(FA_TP_MAX(IN(1).TP,IN(2).TP),IN(3).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= IN(2).SZ(2)',ClassFileName,'file','y');
PrintStringInfo('OUT(2).TP=    FA_TP_MAX(FA_TP_MAX(IN(1).TP,IN(2).TP),IN(3).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(2).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(2).SZ(2)= FA_SUB(FA_ADD(IN(1).SZ(2),IN(2).SZ(2)),''1'')',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z0z0'+ArgSeparator+'z0',ClassFileName,'file','y');

PrintStringInfo('s0s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d0d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c0c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z0z2'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('s2s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2d0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c2c0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2z0'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('s2s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c2c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2z2'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('s0s0'+ArgSeparator+'s0s0',ClassFileName,'file','y');
PrintStringInfo('d0d0'+ArgSeparator+'d0d0',ClassFileName,'file','y');
PrintStringInfo('c0c0'+ArgSeparator+'c0c0',ClassFileName,'file','y');
PrintStringInfo('z0z0'+ArgSeparator+'z0z0',ClassFileName,'file','y');

PrintStringInfo('s0s2'+ArgSeparator+'s2s2',ClassFileName,'file','y');
PrintStringInfo('d0d2'+ArgSeparator+'d2d2',ClassFileName,'file','y');
PrintStringInfo('c0c2'+ArgSeparator+'c2c2',ClassFileName,'file','y');
PrintStringInfo('z0z2'+ArgSeparator+'z2z2',ClassFileName,'file','y');

PrintStringInfo('s2s0'+ArgSeparator+'s2s2',ClassFileName,'file','y');
PrintStringInfo('d2d0'+ArgSeparator+'d2d2',ClassFileName,'file','y');
PrintStringInfo('c2c0'+ArgSeparator+'c2c2',ClassFileName,'file','y');
PrintStringInfo('z2z0'+ArgSeparator+'z2z2',ClassFileName,'file','y');

PrintStringInfo('s2s2'+ArgSeparator+'s2s2',ClassFileName,'file','y');
PrintStringInfo('d2d2'+ArgSeparator+'d2d2',ClassFileName,'file','y');
PrintStringInfo('c2c2'+ArgSeparator+'c2c2',ClassFileName,'file','y');
PrintStringInfo('z2z2'+ArgSeparator+'z2z2',ClassFileName,'file','y');

PrintStringInfo('s0s0s0'+ArgSeparator+'s0s0',ClassFileName,'file','y');
PrintStringInfo('d0d0d0'+ArgSeparator+'d0d0',ClassFileName,'file','y');
PrintStringInfo('c0c0c0'+ArgSeparator+'c0c0',ClassFileName,'file','y');
PrintStringInfo('z0z0z0'+ArgSeparator+'z0z0',ClassFileName,'file','y');

PrintStringInfo('s0s2s0'+ArgSeparator+'s2s0',ClassFileName,'file','y');
PrintStringInfo('d0d2d0'+ArgSeparator+'d2d0',ClassFileName,'file','y');
PrintStringInfo('c0c2c0'+ArgSeparator+'c2c0',ClassFileName,'file','y');
PrintStringInfo('z0z2z0'+ArgSeparator+'z2z0',ClassFileName,'file','y');

PrintStringInfo('s2s2s2'+ArgSeparator+'s2s2',ClassFileName,'file','y');
PrintStringInfo('d2d2d2'+ArgSeparator+'d2d2',ClassFileName,'file','y');
PrintStringInfo('c2c2c2'+ArgSeparator+'c2c2',ClassFileName,'file','y');
PrintStringInfo('z2z2z2'+ArgSeparator+'z2z2',ClassFileName,'file','y');

PrintStringInfo('s0c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('d0z0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('c0s0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z0d0'+ArgSeparator+'z0',ClassFileName,'file','y');

PrintStringInfo('s2c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d2z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('c2s2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2d2'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('s0c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d0z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('c0s2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z0d2'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('s2c0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d2z0'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('c2s0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2d0'+ArgSeparator+'z2',ClassFileName,'file','y');

//added combination

PrintStringInfo('d2d0'+ArgSeparator+'d0d2',ClassFileName,'file','y');
PrintStringInfo('z2z0'+ArgSeparator+'z0z2',ClassFileName,'file','y');
PrintStringInfo('d0z2'+ArgSeparator+'z2z2',ClassFileName,'file','y');
PrintStringInfo('z2d0'+ArgSeparator+'z0z2',ClassFileName,'file','y');
PrintStringInfo('d2z0'+ArgSeparator+'z0z2',ClassFileName,'file','y');


// --- Annotation Function And Function List Function. ---
FunctionName = 'convol'; // AS : done except 2 ouputs cases and 3 inputs cases   AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// -------------------
// --- Class IFFT. ---
// -------------------
ClassName = 'IFFT';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
//Was FA_TP_USER
//Cause some trouble if user specify some precision and if input(and also output) is complex.
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y'); //FOR INRIA changed from IN(1).TP to FA_TP_USER
PrintStringInfo('OUT(1).SZ(1)= IN(1).SZ(1)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= IN(1).SZ(2)',ClassFileName,'file','y');

// --- Function List Class. ---
//NUT: no mixed data types
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);

PrintStringInfo('s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('s0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z0'+ArgSeparator+'z0',ClassFileName,'file','y');

PrintStringInfo('s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('s2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2'+ArgSeparator+'z2',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'ifft'; // AS : Done  AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// ------------------
// --- Class FFT. ---
// ------------------
ClassName = 'FFT';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= IN(1).SZ(1)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= IN(1).SZ(2)',ClassFileName,'file','y');
PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= IN(1).SZ(1)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= IN(1).SZ(2)',ClassFileName,'file','y');
PrintStringInfo('NIN=          3',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= IN(1).SZ(1)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= IN(1).SZ(2)',ClassFileName,'file','y');
PrintStringInfo('NIN=          4',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= IN(1).SZ(1)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= IN(1).SZ(2)',ClassFileName,'file','y');

// --- Function List Class. ---
//NUT: no mixed data types
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('s0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z0'+ArgSeparator+'z0',ClassFileName,'file','y');

PrintStringInfo('s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('s2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('s0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('s0s0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('d0d0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('c0s0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z0d0'+ArgSeparator+'z0',ClassFileName,'file','y');

PrintStringInfo('s2s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2d0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('s2s0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d2d0'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('c2s0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2d0'+ArgSeparator+'z2',ClassFileName,'file','y');

// scilab2c doesn't handle multidimensionnal Fft

//PrintStringInfo('s0s0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
//PrintStringInfo('d0d0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
//PrintStringInfo('s0s0s0'+ArgSeparator+'c0',ClassFileName,'file','y');
//PrintStringInfo('d0d0d0'+ArgSeparator+'z0',ClassFileName,'file','y');
//PrintStringInfo('c0s0s0'+ArgSeparator+'c0',ClassFileName,'file','y');
//PrintStringInfo('z0d0d0'+ArgSeparator+'z0',ClassFileName,'file','y');

//PrintStringInfo('s2s0s0'+ArgSeparator+'s2',ClassFileName,'file','y');
//PrintStringInfo('d2d0d0'+ArgSeparator+'d2',ClassFileName,'file','y');
//PrintStringInfo('s2s0s0'+ArgSeparator+'c2',ClassFileName,'file','y');
//PrintStringInfo('d2d0d0'+ArgSeparator+'z2',ClassFileName,'file','y');
//PrintStringInfo('c2s0s0'+ArgSeparator+'c2',ClassFileName,'file','y');
//PrintStringInfo('z2d0d0'+ArgSeparator+'z2',ClassFileName,'file','y');

//PrintStringInfo('s0s0s0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
//PrintStringInfo('d0d0d0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
//PrintStringInfo('s0s0s0s0'+ArgSeparator+'c0',ClassFileName,'file','y');
//PrintStringInfo('d0d0d0d0'+ArgSeparator+'z0',ClassFileName,'file','y');
//PrintStringInfo('c0s0s0s0'+ArgSeparator+'c0',ClassFileName,'file','y');
//PrintStringInfo('z0d0d0d0'+ArgSeparator+'z0',ClassFileName,'file','y');

//PrintStringInfo('s2s0s0s0'+ArgSeparator+'s2',ClassFileName,'file','y');
//PrintStringInfo('d2d0d0d0'+ArgSeparator+'d2',ClassFileName,'file','y');
//PrintStringInfo('s2s0s0s0'+ArgSeparator+'c2',ClassFileName,'file','y');
//PrintStringInfo('d2d0d0d0'+ArgSeparator+'z2',ClassFileName,'file','y');
//PrintStringInfo('c2s0s0s0'+ArgSeparator+'c2',ClassFileName,'file','y');
//PrintStringInfo('z2d0d0d0'+ArgSeparator+'z2',ClassFileName,'file','y');


//NUT non metto tutte le combinazioni ma prima cerco di capire cosa mi offre INRIA
//NUT come libreria a disposizione.
// --- Annotation Function And Function List Function. ---
FunctionName = 'fft';  // AS : Done   AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// -----------------------
// --- Class FFTShift. ---
// -----------------------
ClassName = 'FFTShift';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= IN(1).SZ(1)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= IN(1).SZ(2)',ClassFileName,'file','y');
PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= IN(1).SZ(1)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= IN(1).SZ(2)',ClassFileName,'file','y');

// --- Function List Class. ---
//NUT: no mixed data types
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z0'+ArgSeparator+'z0',ClassFileName,'file','y');

PrintStringInfo('s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('s0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('s0g2'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('d0g2'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0s0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('c0g2'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z0d0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('z0g2'+ArgSeparator+'z0',ClassFileName,'file','y');

PrintStringInfo('s2s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('s2g2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2d0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('d2g2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c2s0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2g2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2d0'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2g2'+ArgSeparator+'z2',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'fftshift'; // AS : Done   AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);


// --------------------
// --- Class Meanf. ---
// --------------------
ClassName = 'Meanf';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_MAX(IN(1).TP, IN(2).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= ''1''',ClassFileName,'file','y');

PrintStringInfo('NIN=          3',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_MAX(IN(1).TP, IN(2).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= FA_SZ_SEL1(IN(1).SZ(1),IN(3).VAL)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= FA_SZ_SEL2(IN(1).SZ(2),IN(3).VAL)',ClassFileName,'file','y');

// --- Function List Class. ---
//NUT: no mixed data types
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0s0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z0d0'+ArgSeparator+'z0',ClassFileName,'file','y');

PrintStringInfo('s2s2'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d2d2'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c2s2'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z2d2'+ArgSeparator+'z0',ClassFileName,'file','y');

PrintStringInfo('s0s0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0d0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0s0s0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z0d0d0'+ArgSeparator+'z0',ClassFileName,'file','y');

PrintStringInfo('s2s2s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2d2d0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c2s2s0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2d2d0'+ArgSeparator+'z2',ClassFileName,'file','y');
//
PrintStringInfo('s0c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('d0z0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('c0c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z0z0'+ArgSeparator+'z0',ClassFileName,'file','y');

PrintStringInfo('s2c2'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('d2z2'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('c2c2'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z2z2'+ArgSeparator+'z0',ClassFileName,'file','y');

PrintStringInfo('s0c0s0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('d0z0d0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('c0c0s0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z0z0d0'+ArgSeparator+'z0',ClassFileName,'file','y');

PrintStringInfo('s2c2s0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d2z2d0'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('c2c2s0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2z2d0'+ArgSeparator+'z2',ClassFileName,'file','y');
//
PrintStringInfo('s0c0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0z0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0c0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('z0z0'+ArgSeparator+'d0',ClassFileName,'file','y');

PrintStringInfo('s2c2'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d2z2'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c2c2'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('z2z2'+ArgSeparator+'d0',ClassFileName,'file','y');

PrintStringInfo('s0c0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('d0z0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('c0c0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('z0z0d0'+ArgSeparator+'d0',ClassFileName,'file','y');

PrintStringInfo('s2c2s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2z2d0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c2c2s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('z2z2d0'+ArgSeparator+'d2',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'meanf'; // AS : Done   AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'variancef';
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

FunctionName = 'stdevf'; // AS : Done  AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// --------------------
// --- Class Frmag. ---
// --------------------
ClassName = 'Frmag';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_MAX(IN(1).TP,IN(2).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= IN(2).VAL',ClassFileName,'file','y');
PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         2',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_MAX(IN(1).TP,IN(2).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= IN(2).VAL',ClassFileName,'file','y');
PrintStringInfo('OUT(2).TP=    FA_TP_MAX(IN(1).TP,IN(2).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(2).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(2).SZ(2)= IN(2).VAL',ClassFileName,'file','y');

PrintStringInfo('NIN=          3',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_MAX(IN(1).TP,IN(2).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= IN(3).VAL',ClassFileName,'file','y');

PrintStringInfo('NIN=          3',ClassFileName,'file','y');
PrintStringInfo('NOUT=         2',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_MAX(IN(1).TP,IN(2).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= IN(3).VAL',ClassFileName,'file','y');
PrintStringInfo('OUT(2).TP=    FA_TP_MAX(IN(1).TP,IN(2).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(2).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(2).SZ(2)= IN(3).VAL',ClassFileName,'file','y');

// --- Function List Class. ---
//NUT: no mixed data types
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('d2d0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('d2d0'+ArgSeparator+'d2d2',ClassFileName,'file','y');
PrintStringInfo('d0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('d0d0'+ArgSeparator+'d0d0',ClassFileName,'file','y');
PrintStringInfo('d2d2d0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('d2d2d0'+ArgSeparator+'d2d2',ClassFileName,'file','y');
PrintStringInfo('d0d0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('d0d0d0'+ArgSeparator+'d0d0',ClassFileName,'file','y');

PrintStringInfo('s2s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('s2s0'+ArgSeparator+'s2s2',ClassFileName,'file','y');
PrintStringInfo('s0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('s0s0'+ArgSeparator+'s0s0',ClassFileName,'file','y');
PrintStringInfo('s2s2s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('s2s2s0'+ArgSeparator+'s2s2',ClassFileName,'file','y');
PrintStringInfo('s0s0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('s0s0s0'+ArgSeparator+'s0s0',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'frmag';
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// ------------------
// --- Class Lev. ---
// ------------------
ClassName = 'Lev';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= FA_SUB(FA_ADD(IN(1).SZ(1),IN(1).SZ(2)),''2'')',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= ''1''',ClassFileName,'file','y');

PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         2',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= FA_SUB(FA_ADD(IN(1).SZ(1),IN(1).SZ(2)),''2'')',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(2).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(2).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(2).SZ(2)= ''1''',ClassFileName,'file','y');

PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         3',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= FA_SUB(FA_ADD(IN(1).SZ(1),IN(1).SZ(2)),''2'')',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(2).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(2).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(2).SZ(2)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(3).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(3).SZ(1)= FA_SUB(FA_ADD(IN(1).SZ(1),IN(1).SZ(2)),''2'')',ClassFileName,'file','y');
PrintStringInfo('OUT(3).SZ(2)= ''1''',ClassFileName,'file','y');

// --- Function List Class. ---
//NUT: no mixed data types
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('s2'+ArgSeparator+'s2s0',ClassFileName,'file','y');
PrintStringInfo('s2'+ArgSeparator+'s2s0s2',ClassFileName,'file','y');

PrintStringInfo('d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'d2d0',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'d2d0d2',ClassFileName,'file','y');

PrintStringInfo('c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2'+ArgSeparator+'c2c0',ClassFileName,'file','y');
PrintStringInfo('c2'+ArgSeparator+'c2c0c2',ClassFileName,'file','y');

PrintStringInfo('z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2'+ArgSeparator+'z2z0',ClassFileName,'file','y');
PrintStringInfo('z2'+ArgSeparator+'z2z0z2',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'lev'; // AS : Done   AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// --------------------------
// --- Class OpBackSlash. ---
// --------------------------
ClassName = 'OpBackSlash';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_MAX(IN(1).TP,IN(2).TP) ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= FA_SZ_1(FA_SZ_OPBACKSLASH(IN(1).SZ,IN(2).SZ))',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= FA_SZ_2(FA_SZ_OPBACKSLASH(IN(1).SZ,IN(2).SZ))',ClassFileName,'file','y');

// --- Function List Class. ---
//NUT: no mixed data types
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);

PrintStringInfo('s0s0'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('s0s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('s2s0'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('s2s2'+ArgSeparator+'s2',ClassFileName,'file','y');

PrintStringInfo('d0d0'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('d0d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('d2d0'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('d2d2'+ArgSeparator+'d2',ClassFileName,'file','y');

PrintStringInfo('s0c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('c0s0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('c0c0'+ArgSeparator+'c0',ClassFileName,'file','y');

PrintStringInfo('c0s2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('s0c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c0c2'+ArgSeparator+'c2',ClassFileName,'file','y');

PrintStringInfo('c2s0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('s2c0'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2c0'+ArgSeparator+'c2',ClassFileName,'file','y');

PrintStringInfo('s2c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2s2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2c2'+ArgSeparator+'c2',ClassFileName,'file','y');

PrintStringInfo('d0z0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('z0d0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('z0z0'+ArgSeparator+'z0',ClassFileName,'file','y');

PrintStringInfo('z0d2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('d0z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z0z2'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('z2d0'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('d2z0'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2z0'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('d2z2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2d2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2z2'+ArgSeparator+'z2',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'OpBackSlash';
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// -----------------------
// --- Class Cepstrum. ---
// -----------------------
ClassName = 'Cepstrum';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_MAX(IN(1).TP,IN(2).TP) ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= FA_MUL(IN(1).SZ(1),IN(1).SZ(2))',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= ''1''',ClassFileName,'file','y');

// --- Function List Class. ---
//NUT: no mixed data types
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);

PrintStringInfo('s0s0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('d0d0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('s0c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('d0z0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('s2s2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d2d2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('s2c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d2z2'+ArgSeparator+'z2',ClassFileName,'file','y');


// --- Annotation Function And Function List Function. ---
FunctionName = 'Cepstrum';
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// -------------------
// --- Class Spec. ---
// -------------------
ClassName = 'Spec';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_COMPLEX(IN(1).TP)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= IN(1).SZ(1)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= ''1''',ClassFileName,'file','y');

PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         2',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    FA_TP_COMPLEX(IN(1).TP)',ClassFileName,'file','y'); //FOR INRIA FA_TP_MAX NEEDS 2 Input args
PrintStringInfo('OUT(1).SZ(1)= IN(1).SZ(1)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= IN(1).SZ(2)',ClassFileName,'file','y');
PrintStringInfo('OUT(2).TP=    FA_TP_COMPLEX(IN(1).TP)',ClassFileName,'file','y'); //FOR INRIA FA_TP_MAX NEEDS 2 Input args
PrintStringInfo('OUT(2).SZ(1)= IN(1).SZ(1)',ClassFileName,'file','y');
PrintStringInfo('OUT(2).SZ(2)= IN(1).SZ(2)',ClassFileName,'file','y');

// --- Function List Class. ---
//NUT: no mixed data types
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);

PrintStringInfo('s0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'z0',ClassFileName,'file','y');
PrintStringInfo('c0'+ArgSeparator+'c0',ClassFileName,'file','y');
PrintStringInfo('z0'+ArgSeparator+'z0',ClassFileName,'file','y');

PrintStringInfo('s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('s2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'z2',ClassFileName,'file','y');
PrintStringInfo('z2'+ArgSeparator+'z2',ClassFileName,'file','y');

PrintStringInfo('s0'+ArgSeparator+'c0c0',ClassFileName,'file','y');
PrintStringInfo('d0'+ArgSeparator+'z0z0',ClassFileName,'file','y');
PrintStringInfo('c0'+ArgSeparator+'c0c0',ClassFileName,'file','y');
PrintStringInfo('z0'+ArgSeparator+'z0z0',ClassFileName,'file','y');

PrintStringInfo('s2'+ArgSeparator+'s2s2',ClassFileName,'file','y');
PrintStringInfo('s2'+ArgSeparator+'c2c2',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'d2d2',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'z2z2',ClassFileName,'file','y');
PrintStringInfo('c2'+ArgSeparator+'c2c2',ClassFileName,'file','y');
PrintStringInfo('z2'+ArgSeparator+'z2z2',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'spec'; // AS : Done  AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// -------------------
// --- Class Part. ---
// -------------------
ClassName = 'Part';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    ''g''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= FA_SZ_RTMAX(IN(1).SZ(2))',ClassFileName,'file','y');

// --- Function List Class. ---
//NUT: no mixed data types
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);

PrintStringInfo('g2s0'+ArgSeparator+'g2',ClassFileName,'file','y');
PrintStringInfo('g2d0'+ArgSeparator+'g2',ClassFileName,'file','y');
PrintStringInfo('g2s2'+ArgSeparator+'g2',ClassFileName,'file','y');
PrintStringInfo('g2d2'+ArgSeparator+'g2',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'part'; // to code
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// -----------------------
// --- Class Strindex. ---
// -----------------------
ClassName = 'Strindex';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          2',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    ''g''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= FA_SZ_RTMAX(IN(1).SZ(2))',ClassFileName,'file','y');

// --- Function List Class. ---
//NUT: no mixed data types
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);

PrintStringInfo('g2g2'+ArgSeparator+'s0',ClassFileName,'file','y');
PrintStringInfo('g2g2'+ArgSeparator+'d0',ClassFileName,'file','y');
PrintStringInfo('g2g2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('g2g2'+ArgSeparator+'d2',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'strindex'; // to code
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);

// -----------------------
// --- Class StrSubSt. ---
// -----------------------
ClassName = 'StrSubSt';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          3',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    ''g''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= ''1''',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= FA_SZ_RTMAX(FA_MUL(IN(1).SZ(2),IN(3).SZ(2)))',ClassFileName,'file','y');

// --- Function List Class. ---
//NUT: no mixed data types
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);

PrintStringInfo('g2g2g2'+ArgSeparator+'g2',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'strsubst'; // to code
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);



// ------------------
// --- Class Expm. ---
// ------------------
ClassName = 'Expm';

// --- Class Annotation. ---
PrintStringInfo('   Adding Class: '+ClassName+'.',GeneralReport,'both','y');
ClassFileName = fullfile(SCI2CLibCAnnClsDir,ClassName+ExtensionCAnnCls);
PrintStringInfo('NIN=          1',ClassFileName,'file','y');
PrintStringInfo('NOUT=         1       ',ClassFileName,'file','y');
PrintStringInfo('OUT(1).TP=    IN(1).TP',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(1)= IN(1).SZ(1)',ClassFileName,'file','y');
PrintStringInfo('OUT(1).SZ(2)= IN(1).SZ(2)',ClassFileName,'file','y');

// --- Function List Class. ---
ClassFileName = fullfile(SCI2CLibCFLClsDir,ClassName+ExtensionCFuncListCls);
PrintStringInfo('s2'+ArgSeparator+'s2',ClassFileName,'file','y');
PrintStringInfo('d2'+ArgSeparator+'d2',ClassFileName,'file','y');
PrintStringInfo('c2'+ArgSeparator+'c2',ClassFileName,'file','y');
PrintStringInfo('z2'+ArgSeparator+'z2',ClassFileName,'file','y');

// --- Annotation Function And Function List Function. ---
FunctionName = 'expm'; // AS : Done AS : Float_Done
PrintStringInfo('      Adding Function: '+FunctionName+'.',GeneralReport,'both','y');
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCAnnFunDir,ClassName,GeneralReport,ExtensionCAnnFun);
INIT_GenAnnFLFunctions(FunctionName,SCI2CLibCFLFunDir,ClassName,GeneralReport,ExtensionCFuncListFun);




// ////////////////////////////////////////////
// /////PARTE INTRODOTTA DA ALBERTO MOREA 
// /////////////////////////////////////////////
// ///////////////////////////////////////////////

// // DOMAINS DEFINITION  FOR EVERY CLASS-LEADER IN SCI2C LIBRARY

// // In the  following  lets: 
// //	N,R ,C     are the  standard sets of integer,real ,complex numbers
// //	 I,J c R   are intervals of real numbers 
// //   {}  the empty set  
// //	  Sel={'r','c'}  or {0,1}   are the selection sets r=row,c=column 
// //	  B={T,F} is Boolean set
// //   F is the standard set of File IDentify  files  
// //	  G is the standard alphanumeric string domain

// // For every  sci2c function library class  we reported  
// // the domains , annotations only for the class-leader 

// //  Example    [y1,y2]=Fun(x1,x2,x3) : scilab library  function call

// // Domain RxNxSel->{R,R}   :the first input element  is real number ,the second is integer , 
// // the last is in the selection set
// // The first output and the second are real numbers

// // Notice : only   default (double) precision are considered for real and complex data 
// // The dimension of the input/output data are provided in the mapping schema 

// //    d0 =  double real  scalar 
// //    d2 =  double real vector or matrix
// //    z0=  double complex scalar 
// //    z2= double complex  vector or matrix

// // ---------------------
// // --- Class Global. ---
// // ---------------------

// // DOMAINS   G -> R  

// //////////////////////////////////


// // ---------------------
// // --- Class Float. ---
// // ---------------------

// // DOMAINS   R -> R   

// //////////////////////////////////


// // ---------------------
// // --- Class Double. ---
// // ---------------------

// // DOMAINS   R -> R   

// //////////////////////////////////

// // ----------------------------
// // --- Class FloatComplex. ---
// // ----------------------------

// // DOMAINS      1)R -> C         
// //              2)C -> C  

// /////////////////////////////////

// // ----------------------------
// // --- Class DoubleComplex. ---
// // ----------------------------

// // DOMAINS     1) R -> C         
// //             2) C -> C  

// /////////////////////////////////

// // ------------------
// // --- Class Sin. ---
// // ------------------

// // DOMAINS   1) IcR -> JcR       
// //           2) C -> C    

// /////////////////////////////////

// // -------------------
// // --- Class Atan. ---
// // -------------------

// // DOMAINS     1) IcR -> JcR  
// //             2) C -> C 
// //             3) RxR -> JcR
             
// /////////////////////////////////

// // -------------------
// // --- Class Sqrt. ---
// // -------------------

// // DOMAINS     1) IcR -> JcR  
// //             2) R -> R 
// //             3) C -> C
             
 // /////////////////////////////////

// // --------------------
// // --- Class Zeros. ---
// // --------------------

// // DOMAINS        1) {}->R 
// //                2) NxN -> R 
// //                3) R -> R  
// //                4) C -> R
                
// /////////////////////////////////

// // --------------------
// // --- Class Sum. ---
// // --------------------

// // DOMAINS      1) R -> R  
// //              2) C -> C
// //              3) RxSel -> R
// //              4) CxSel -> R
              
 // /////////////////////////////////

// // --------------------
// // --- Class Abs. ---
// // --------------------

// // DOMAINS    1) R -> R+  
// //            2) C -> R+

// /////////////////////////////////

// // ------------------------
// // --- Class OpDotStar. ---
// // ------------------------

// // DOMAINS        1) RxR -> R 
// //                2) CxC -> C 
// //                3) RxC -> C  
// //                4) CxR -> C

// /////////////////////////////////

// // ------------------------
// // --- Class OpDotHat. ---
// // ------------------------

// // DOMAINS        1) RxR -> R 
// //                2) CxC -> C 

// /////////////////////////////////

// // ----------------------
// // --- Class OpLogEq. ---
// // ----------------------

// // DOMAINS        1) RxR -> B 
// //                2) CxC -> B 
// //                3) GxG -> B

// /////////////////////////////////

// // ---------------------
// // --- Class OpStar. ---
// // ---------------------

// // DOMAINS        1) RxR -> R 
// //                2) CxC -> C 
// //                3) RxC -> C  
// //                4) CxR -> C

// /////////////////////////////////

// // ---------------------
// // --- Class OpApex. ---
// // ---------------------

// // DOMAINS        1) R -> R 
// //                2) C -> C

// /////////////////////////////////

// // ---------------------
// // --- Class OpPlus. ---
// // ---------------------

// // DOMAINS        1) R -> R 
// //                2) C -> C 
// //                3) RxR -> R  
// //                4) CxC -> C
// //                5) RxC -> C
// //                6) CxR -> C 
// //                7) GxG -> G 

// /////////////////////////////////

// // ----------------------
// // --- Class OpMinus. ---
// // ----------------------

// // DOMAINS        1) R -> R 
// //                2) C -> C 
// //                3) RxR -> R  
// //                4) CxC -> C
// //                5) RxC -> C
// //                6) CxR -> C 


// /////////////////////////////////

// // -------------------
// // --- Class OpRc. ---
// // -------------------

// // DOMAINS        1) RxR -> R 
// //                2) CxC -> C 

// /////////////////////////////////

// // -------------------
// // --- Class OpCc. ---
// // -------------------

// // DOMAINS        1) RxR -> R 
// //                2) CxC -> C 

// /////////////////////////////////

// // -------------------
// // --- Class Find. ---
// // -------------------

// // DOMAINS        1) R -> N 
// //                2) R -> {N,N} 
// //                3) RxN -> N  
// //                4) RxN ->{N,N}

// /////////////////////////////////

// // ---------------------
// // --- Class Length. ---
// // ---------------------

// // DOMAINS        1) R -> N 
// //                2) C -> N 
// //                3) G -> N 

// /////////////////////////////////

// // -------------------
// // --- Class Size. ---
// // -------------------

// // DOMAINS        1) R -> {N,N} 
// //                2) C -> {N,N} 
// //                3) RxSel -> {N,N} 
// //                4) CxSel -> {N,N}

// /////////////////////////////////

// // ---------------------
// // --- Class Return. ---
// // ---------------------
// // DOMAINS        1) {} -> {} 

// /////////////////////////////////

// // ----------------------
// // --- Class OpColon. ---
// // ----------------------

// // DOMAINS        1) RxR -> R 
// //                2) RxRxR -> R

// /////////////////////////////////

// // ----------------------
// // --- Class IsEmpty. ---
// // ----------------------

// // DOMAINS        1) R -> B 
// //                2) C -> B

// /////////////////////////////////

// // ----------------------
// // --- Class Trace. ---
// // ----------------------

// // DOMAINS        1) R -> R 
// //                2) C -> C

// ////////////////////////////////

// // --------------------
// // --- Class OpIns. ---
// // --------------------

// // DOMAINS        1) RxRxR   -> {}
// //                2) RxRxRxR -> {}
// //                3) CxRxR   -> {}
// //                4) CxRxRxC -> {}
// //                5) CxRxC   -> {}

// ////////////////////////////////

// // --------------------
// // --- Class OpExt. ---
// // --------------------

// // DOMAINS        1) RxR   -> R  
// //                2) CxR   -> C
// //                3) CxRxR -> C

// ////////////////////////////////

// // -------------------
// // --- Class Disp. ---
// // -------------------

// // DOMAINS        1) R -> R  
// //                2) C -> R
// //                3) G -> R

// ////////////////////////////////

// // ----------------------
// // --- Class OpEqual. ---
// // ----------------------

// // DOMAINS        1) R -> R  
// //                2) C -> C
// //                3) G -> G

// ////////////////////////////////

// // --------------------
// // --- Class Mopen. ---
// // --------------------

// // DOMAINS        1) G     -> F
// //                2) GxG   -> F
// //                3) GxGxR -> {F,R}
// //                4) G     -> {F,R}
// //                5) GxG   -> {F,R}

// ////////////////////////////////

// // -------------------
// // --- Class Mput. ---
// // -------------------

// // DOMAINS        1) RxGxF -> {}
// //                2) RxGxF -> N

// ////////////////////////////////

// // -------------------
// // --- Class Mget. ---
// // -------------------

// // DOMAINS        1) RxGxF -> N

// ////////////////////////////////

// // ---------------------
// // --- Class Mclose. ---
// // ---------------------

// // DOMAINS        1) {} -> N
// //                2) F -> N

// ////////////////////////////////

// // ---------------------
// // --- Class Mseek. ---
// // ---------------------

// // DOMAINS        1) R     -> {}
// //                2) RxF   -> {}
// //                3) RxFxG -> {}

// ////////////////////////////////

// // ---------------------
// // --- Class Convol. ---
// // ---------------------

// // DOMAINS         1) RxR->R   
                   // 2) CxC->C  
                   // 3) RxC->C  
                   // 4) CxR->C
                   // 5) RxR->{R,R}   
                   // 6) CxC->{C,C}  
                   // 7) RxC->{C,C} 
                   // 8) CxR->{C,C}  
                   // 9) CxC->{C,C}
                  // 10) RxRxR->{R,R} 
                  // 11) RxCxC->{C,C}   
                  // and so on …..
                  
 
// ////////////////////////////////


// // -------------------
// // --- Class IFFT. ---
// // -------------------

// // DOMAINS         1) R->C   
                   // 2) C->C  
                   
 // ////////////////////////////////

// // ------------------
// // --- Class FFT. ---
// // ------------------

// // DOMAINS         1) R->C   
                   // 2) RxN->C  
                   // 3) C->C  
                   // 4) CxN->C
                   // 5) RxNxN->C   
                   // 6) CxNxN->C  
                   // 7) RxNxNxN->C
                   // 8) CxNxNxN->C
                   
// ////////////////////////////////

// // -----------------------
// // --- Class FFTShift. ---
// // -----------------------

// // DOMAINS         1) R->R  
                   // 2) C->C  
                   // 3) RxR->R  
                   // 4) RxG->R
                   // 5) CxR->C   
                   // 6) CxG->C 
                   
// ////////////////////////////////                  
                   
// // --------------------
// // --- Class Meanf. ---
// // --------------------

// // DOMAINS         1) R->R  
                   // 2) C->C  
                   // 3) CxSel->C  
                   // 4) RxSel->R
                   
// ////////////////////////////////

// // --------------------
// // --- Class Frmag. ---
// // --------------------

// // DOMAINS         1) RxN->R  
                   // 2) RxN->{R,R}   
                   // 3) RxRxN->R  
                   // 4) RxRxN->{R,R} 

// ////////////////////////////////

// // ------------------
// // --- Class Lev. ---
// // ------------------

// // DOMAINS         1) R->R  
                   // 2) R->{R,R}   
                   // 3) R->{R,R,R}
                   // 4) C->C                   
                   // 5) C->{C,C} 
                   // 6) C->{C,C,C} 
                   
// ////////////////////////////////

// // --------------------------
// // --- Class OpBackSlash. ---
// // --------------------------

// // DOMAINS         1) RxR->R   
                   // 2) CxC->C  
                   // 3) RxC->C  
                   // 4) CxR->C
                   
// ////////////////////////////////

// // -----------------------
// // --- Class Cepstrum. ---
// // -----------------------

// // DOMAINS         1) RxR->C  
                   // 2) RxC->C 
                   
// ////////////////////////////////

// // -----------------------
// // --- Class Spec. ---
// // -----------------------

// // DOMAINS         1) R->R   
                   // 2) C->C  
                   // 3) R->{R,R}
                   // 4) C->{C,C}

// ////////////////////////////////

// // -------------------
// // --- Class Part. ---
// // -------------------

// // DOMAINS         1) GxN->G

// ////////////////////////////////

// // -----------------------
// // --- Class Strindex. ---
// // -----------------------

// // DOMAINS         1) GxG->N

// ////////////////////////////////

// // -----------------------
// // --- Class StrSubSt. ---
// // -----------------------

// // DOMAINS         1) GxGxG->G

// ////////////////////////////////


// ////////////////////////////////
// ////////////////////////////////
// //  fine parte introdotta alberto
// ////////////////////////////////
// ////////////////////////////////

// //RNU se puoi sistema meglio la parte introdotta da al.
endfunction

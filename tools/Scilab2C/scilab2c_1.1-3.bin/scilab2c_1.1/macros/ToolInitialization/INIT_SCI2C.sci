function [FileInfoDatFile,SharedInfoDatFile] = ...
    INIT_SCI2C(UserScilabMainFile, UserSciFilesPaths, SCI2COutputDir, RunMode)
// function [FileInfoDatFile,SharedInfoDatFile] = INIT_SCI2C(SCI2CInputPrmFile)
// -----------------------------------------------------------------
// #RNU_RES_B
// This function initializes the SCI2C tool according
// to the input parameters recorded in the SCI2CParameters.
// All info will be stored into FileInfoDatFile.
//
// Input data:
// SCI2CInputPrmFile: name of the .sce file containing input parameters.
//
// Output data:
// FileInfoDatFile: name of the .dat file containing the FileInfo structure.
// SharedInfoDatFile: it is a buffer containing parameters that are exchanged by the
//                    functions of the SCI2C tool.
// #RNU_RES_E
// Status:
// 13-Apr-2007 -- Raffaele Nutricato: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// #RNU_RES_B
//NUT: questo file e' da rivedere quando il tool funzionera al 50%
// #RNU_RES_E
// ------------------------------
// --- Check input arguments. ---
// ------------------------------
//SCI2CNInArgCheck(argn(2),1,1);

// ------------------------------
// --- Read Input Parameters. ---
// ------------------------------
//exec(SCI2CInputPrmFile);
// #RNU_RES_B
//NUT: queste variabili sono per usi futuri.
//NUT: e saranno introdotti nel parameter file.
// #RNU_RES_E
WorkAreaSizeBytes = 2000*8; // 2000 locations of double
// #RNU_RES_B
// Maximum number of temporary scalar variables that can be used.
// #RNU_RES_E
TotTempScalarVars = 20;
EnableTempVarsReuse = 0; // 0 = Disable; 1 = Enable.


// #RNU_RES_B
//NUT: I prefer to don't show this parameters to the user.
// --- Directory where all the products of the SCI2C tool will be stored. ---
// #RNU_RES_E
//-- [SCI2CResultDir,tmpfile,tmpext] = fileparts(SCI2CInputPrmFile);
SCI2CResultDir = SCI2COutputDir;


WorkingDir = fullfile(SCI2CResultDir,'SCI2CTmpResultsReports');
// #RNU_RES_B
// --- Directory where the generated C code will be stored. ---
// #RNU_RES_E
OutCCCodeDir = SCI2CResultDir;

// ------------------------------
// --- Initialize SharedInfo. ---
// ------------------------------
//SharedInfo = INIT_GenSharedInfo(WorkingDir,OutCCCodeDir,UserSciFilesPaths,...
//   RunMode,UserScilabMainFile,TotTempScalarVars,EnableTempVarsReuse,Sci2CLibMainHeaderFName);

//-- FIXME : MainLibHeader and Verbose mode are (?) configurable
SharedInfo = INIT_GenSharedInfo(RunMode,UserScilabMainFile, ...
				TotTempScalarVars,EnableTempVarsReuse,"sci2clib.h", %t);

// ----------------------------
// --- Initialize FileInfo. ---
// ----------------------------
FileInfo = INIT_GenFileInfo(WorkingDir,OutCCCodeDir,UserSciFilesPaths);
PrintStepInfo('SCI2C hArtes/POLIBA Tool!!!',FileInfo.GeneralReport,'stdout');

// ----------------------------------------------------
// --- Remove previous versions of SCI2C files/dir. ---
// ----------------------------------------------------
INIT_RemoveDirs(FileInfo,SharedInfo.RunMode);

// ---------------------------
// --- Create Directories. ---
// ---------------------------
INIT_CreateDirs(FileInfo);
PrintStepInfo('SCI2C hArtes/POLIBA Tool!!!',FileInfo.GeneralReport,'file');

// ------------------------------
// --- Initialize GlobalVars. ---
// ------------------------------
GlobalVars = [];
save(FileInfo.GlobalVarFileName,GlobalVars);

// ----------------------------------
// --- Initialize Main .dat file. ---
// ----------------------------------
//NUT: qui va sistemata.
clear FunInfo
//NUT: qua conviene fare una unica funzione.
FunInfo.SCIFunctionName     = SharedInfo.NextSCIFunName;
FunInfo.CFunctionName       = SharedInfo.NextCFunName;
FunInfo.FunPrecSpecifier    = ''; //NUT: si riferiscono al main verifica se sono corrette
FunInfo.FunTypeAnnot        = ''; //NUT: si riferiscono al main verifica se sono corrette
FunInfo.FunSizeAnnot        = ''; //NUT: si riferiscono al main verifica se sono corrette
FunInfo.NInArg              = 0;//NUT: si riferiscono al main verifica se sono corrette
FunInfo.InArg(1).Name       = '';//NUT: si riferiscono al main verifica se sono corrette
FunInfo.InArg(1).Type       = '';//NUT: si riferiscono al main verifica se sono corrette
FunInfo.InArg(1).Value      = %nan;//NUT: si riferiscono al main verifica se sono corrette
FunInfo.InArg(1).Size(1)    = '';//NUT: si riferiscono al main verifica se sono corrette
FunInfo.InArg(1).Dimension  = '';//NUT: si riferiscono al main verifica se sono corrette
FunInfo.InArg(2).Size(2)    = '';//NUT: si riferiscono al main verifica se sono corrette
FunInfo.NOutArg             = 0;//NUT: si riferiscono al main verifica se sono corrette
FunInfo.OutArg(1).Name      = '';
FunInfo.OutArg(1).Type      = '';
FunInfo.OutArg(1).Size(1)   = '';
FunInfo.OutArg(1).Size(2)   = '';
FunInfo.OutArg(1).Dimension = '';
FunInfo.PosFirstOutScalar   = 0;
FunInfo.LibTypeInfo         = 'USER2C';
save(fullfile(FileInfo.FunctionList.FunInfoDatDir,FunInfo.CFunctionName+'.dat'),FunInfo);
clear FunInfo

// -------------------------------------
// --- Initialize ASTStack.dat file. ---
// -------------------------------------
//NUT: questa struttura deve sostituire le variabili global usate per lo stack
clear ASTStack
ASTStack.SCI2CSTACK    = 'EMPTYSTACK';
ASTStack.StackPosition = 1;
ASTStack.STACKDEDUG    = 0;
save(FileInfo.ASTStackDataFile,ASTStack);
clear ASTStack

// ---------------------------------------------
// --- Generate new versions of SCI2C files. ---
// ---------------------------------------------
save(FileInfo.FileInfoDatFile,FileInfo);
save(FileInfo.SharedInfoDatFile,SharedInfo);
FileInfoDatFile = FileInfo.FileInfoDatFile;
SharedInfoDatFile = FileInfo.SharedInfoDatFile;

global anscounter; //NUT: just to fix problem with ans variables.
anscounter = 0;

endfunction
// #RNU_RES_B
//NUT: quando genero il c della funzione utente devo anche generare il corrispondente file delle includes.
//NUT: perche' la main non la devo annotare, secondo me occorre annotarla.?
// #RNU_RES_E

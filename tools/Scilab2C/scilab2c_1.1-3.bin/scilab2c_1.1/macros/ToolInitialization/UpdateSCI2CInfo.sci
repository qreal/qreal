function UpdateSCI2CInfo(FileInfoDatFile)
// function UpdateSCI2CInfo(FileInfoDatFile)
// -----------------------------------------------------------------
// #RNU_RES_B
// Updates the FileInfo struct according to the new scilab function
// to be converted in C.
//
// Input data:
// FileInfoDatFile: name of the .dat file containing the FileInfo structure.
//
// Output data:
// ---
//
// #RNU_RES_E
// Status:
// 13-Apr-2007 -- Raffaele Nutricato: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),1,1);

// ---------------------------------
// --- Load File Info Structure. ---
// ---------------------------------
clear FileInfo
load(FileInfoDatFile,'FileInfo');

// -----------------------------------
// --- Load Shared Info Structure. ---
// -----------------------------------
clear SharedInfo
load(FileInfo.SharedInfoDatFile,'SharedInfo');

// ---------------------------------------------------
// --- Extraction of the function name and number. ---
// ---------------------------------------------------
funname   = SharedInfo.NextSCIFunName;
funnumber = SharedInfo.NextSCIFunNumber;
// #RNU_RES_B
//NUT: sicuro che mi serve questa struttura? SharedInfo.NextSCIFunNumber cioe' il numero della funzione a che serve?
// #RNU_RES_E
PrintStepInfo('Start translation of function ""'+funname+'""',...
   FileInfo.GeneralReport,'both');

// -----------------------------------
// --- Update File Info structure. ---
// -----------------------------------
FileInfo.Funct(funnumber).Name                   = funname;
FileInfo.Funct(funnumber).SCIFileName            = SharedInfo.NextSCIFileName;
FileInfo.Funct(funnumber).ASTFileName            = fullfile(FileInfo.WorkingDir,funname,funname+'.ast');
FileInfo.Funct(funnumber).CPass1FileName         = fullfile(FileInfo.WorkingDir,funname,SharedInfo.NextCFunName+'_pass1.c');
FileInfo.Funct(funnumber).PfxP1ForProlFileName   = fullfile(FileInfo.WorkingDir,funname,SharedInfo.NextCFunName+'_pass1ProlFor');
FileInfo.Funct(funnumber).PfxP1ForEpilFileName   = fullfile(FileInfo.WorkingDir,funname,SharedInfo.NextCFunName+'_pass1EpilFor');
FileInfo.Funct(funnumber).PfxP1WhileProlFileName = fullfile(FileInfo.WorkingDir,funname,SharedInfo.NextCFunName+'_pass1ProlWhile');
FileInfo.Funct(funnumber).PfxP1WhileEpilFileName = fullfile(FileInfo.WorkingDir,funname,SharedInfo.NextCFunName+'_pass1EpilWhile');
FileInfo.Funct(funnumber).CPass1FreeFileName     = fullfile(FileInfo.WorkingDir,funname,SharedInfo.NextCFunName+'_pass1free.c');
FileInfo.Funct(funnumber).CPass2FileName         = fullfile(FileInfo.WorkingDir,funname,SharedInfo.NextCFunName+'_pass2.c');
FileInfo.Funct(funnumber).Pass1HeaderFileName    = fullfile(FileInfo.WorkingDir,funname,SharedInfo.NextCFunName+'.h');
FileInfo.Funct(funnumber).FinalCFileName         = fullfile(FileInfo.OutCCCodeDir,SharedInfo.NextCFunName+'.c');
FileInfo.Funct(funnumber).FinalHeaderFileName    = fullfile(FileInfo.OutCCCodeDir,SharedInfo.NextCFunName+'.h');
FileInfo.Funct(funnumber).CInitVarsFileName      = fullfile(FileInfo.WorkingDir,funname,SharedInfo.NextCFunName+'_initvars.c');
FileInfo.Funct(funnumber).CDeclarationFileName   = fullfile(FileInfo.WorkingDir,funname,SharedInfo.NextCFunName+'_declarations.c');
FileInfo.Funct(funnumber).CGblDeclarFileName     = fullfile(FileInfo.WorkingDir,funname,SharedInfo.NextCFunName+'_globaldeclarations.c');
FileInfo.Funct(funnumber).ReportFileName         = fullfile(FileInfo.WorkingDir,funname,SharedInfo.NextCFunName+'.rpt');
FileInfo.Funct(funnumber).LocalVarFileName       = fullfile(FileInfo.WorkingDir,funname,SharedInfo.NextCFunName+'_LOCVAR.dat');
FileInfo.Funct(funnumber).TempVarFileName        = fullfile(FileInfo.WorkingDir,funname,SharedInfo.NextCFunName+'_TMPVAR.dat');
FileInfo.Funct(funnumber).SCICopyFileName        = fullfile(FileInfo.WorkingDir,funname,funname+'_copy.sci');


// -------------------------------------
// --- Update Shared Info structure. ---
// -------------------------------------
SharedInfo.NIndent                = 0; // Indentation Level. Useful to produce indentated C code.
SharedInfo.SkipNextEqual          = 0; // 1 = the next equal in the AST will not produce C code.
SharedInfo.SkipNextPrec           = 0; // 1 = the next precision specifier in the AST will not produce C code.
SharedInfo.SkipNextFun            = 0; // 1 = the next function in the AST will not produce C code.
SharedInfo.ASTReader.fidAST       = -1;
SharedInfo.CountNestedIf          = 0;
SharedInfo.CountForTempVars       = 0;
SharedInfo.For.Level              = 0;
SharedInfo.ForExpr.OnExec         = 0;
SharedInfo.ForExpr.IntCntArg      = [];
SharedInfo.ForExpr.MtxValCntArg   = [];
SharedInfo.ForExpr.SclValCntArg   = [];
SharedInfo.ForExpr.OpColonInfoIn1 = '';
SharedInfo.ForExpr.OpColonInfoIn2 = '';
SharedInfo.ForExpr.OpColonInfoIn3 = '';
SharedInfo.ForExpr.AssignmentFun  = 0;

SharedInfo.WhileExpr.OnExec     = 0;
SharedInfo.WhileExpr.CondVar    = '';
SharedInfo.WhileExpr.DimCondVar = -1;
SharedInfo.While.Level      = 0;
//NUT: anche questa sarebbe da inizializzare con una bella funzione.


SharedInfo.CFunId.OpColon   = 3;
SharedInfo.CFunId.EqScalar  = 4;
SharedInfo.CFunId.EqMatrix  = 5;
SharedInfo.CFunId.GenFunMtx = 6; // (scalar functions are fall in the scalar equal category.)

SharedInfo = INIT_SharedInfoEqual(SharedInfo);

// Contains the list of the C calls calls made in the current .sci file.
SharedInfo.CFunctsAlreadyCalled = '_____________'; // Initialization with a dummy name

// ---------------------------------------
// --- Update Converted Function List. ---
// ---------------------------------------
Converted = FL_UpdateConverted(SharedInfo.NFilesToTranslate,FileInfo.FunctionList.ConvertedDat);

// --------------------------------------
// --- Create the function directory. ---
// --------------------------------------
rmdir(fullfile(FileInfo.WorkingDir,funname),'s');
mkdir(FileInfo.WorkingDir,funname);

// -----------------------------------------
// --- Initialize Other FileInfo fields. ---
// -----------------------------------------
PrintStringInfo(' ',FileInfo.Funct(funnumber).SCICopyFileName,'file','y'); // Cannot use copyfile when the directory is empty!.
SCI2Ccopyfile(FileInfo.Funct(funnumber).SCIFileName,FileInfo.Funct(funnumber).SCICopyFileName,'overwrite');

FileInfo.Funct(funnumber).SCICopyFileFid = SCI2COpenFileRead(FileInfo.Funct(funnumber).SCICopyFileName);
// Perform a dummy reading up to the function.
//NUT: mettimi in una funzione.
scicopyfid       = FileInfo.Funct(funnumber).SCICopyFileFid;
CPass1FileName   = FileInfo.Funct(funnumber).CPass1FileName;
IndentLevel      = SharedInfo.NIndent;
FoundFunctionKey = 0;
PrintStringInfo(C_IndentBlanks(IndentLevel)+'/*',CPass1FileName,'file','y');
PrintStringInfo(C_IndentBlanks(IndentLevel)+'  SCI2C: ------------------------------------------------------------------',CPass1FileName,'file','y');
while (~meof(scicopyfid) & (FoundFunctionKey==0))
   // Read a line from the scilab file
   sciline = mgetl(scicopyfid,1);
   noblkssciline = stripblanks(sciline);
   if (SCI2Cstrncmps1size('function',noblkssciline))
      FoundFunctionKey = 1;
   end
   PrintStringInfo(C_IndentBlanks(IndentLevel)+'  SCI2C: '+sciline,CPass1FileName,'file','y');
end
PrintStringInfo(C_IndentBlanks(IndentLevel)+'  SCI2C: ------------------------------------------------------------------',CPass1FileName,'file','y');
PrintStringInfo(C_IndentBlanks(IndentLevel)+'*/',CPass1FileName,'file','y');

// -------------------------
// --- Initialize Files. ---
// -------------------------
PrintStringInfo(' ',FileInfo.Funct(funnumber).CDeclarationFileName,'file','y');
PrintStringInfo(' ',FileInfo.Funct(funnumber).CGblDeclarFileName,'file','y');
PrintStringInfo(' ',FileInfo.Funct(funnumber).CInitVarsFileName,'file','y');
CPass1FreeFileName = FileInfo.Funct(funnumber).CPass1FreeFileName;
PrintStringInfo(' ',CPass1FreeFileName,'file','y');
PrintStringInfo(C_IndentBlanks(1)+'/*',CPass1FreeFileName,'file','y');
PrintStringInfo(C_IndentBlanks(1)+'** --------------------- ',CPass1FreeFileName,'file','y');
PrintStringInfo(C_IndentBlanks(1)+'** --- Free Section. --- ',CPass1FreeFileName,'file','y');
PrintStringInfo(C_IndentBlanks(1)+'** --------------------- ',CPass1FreeFileName,'file','y');
PrintStringInfo(C_IndentBlanks(1)+'*/',CPass1FreeFileName,'file','y');

// -----------------------------------
// --- Initialize Local/Temp Vars. ---
// -----------------------------------
LocalVars = [];
TempVars = [];

// ------------------------------------
// --- Determine Default Precision. ---
// ------------------------------------
// For the current release only the following approaches are available:
// 'NO_RESIZE'
// 'REALLOC_ALL_RESIZE_ALL'
SharedInfo.DefaultPrecision = ...
   FA_GetDefaultPrecision(FileInfo.Funct(funnumber).SCICopyFileName,FileInfo.Funct(funnumber).ReportFileName);

// ----------------------------------
// --- Determine Resize Approach. ---
// ----------------------------------
SharedInfo.ResizeApproach = FA_GetResizeApproach(FileInfo.Funct(funnumber).SCICopyFileName,FileInfo.Funct(funnumber).ReportFileName);

// ---------------------
// --- Save section. ---
// ---------------------
// --- Save File Info Structure. ---
save(FileInfoDatFile,FileInfo);

// --- Save File Info Structure. ---
save(FileInfo.SharedInfoDatFile,SharedInfo);

// --- Save Local/Temp Vars. ---
save(FileInfo.Funct(funnumber).LocalVarFileName,LocalVars);
save(FileInfo.Funct(funnumber).TempVarFileName,TempVars);

// --- Save Converted .dat file. ---
save(FileInfo.FunctionList.ConvertedDat,Converted);
// -------------------------
// --- End save section. ---
// -------------------------

endfunction

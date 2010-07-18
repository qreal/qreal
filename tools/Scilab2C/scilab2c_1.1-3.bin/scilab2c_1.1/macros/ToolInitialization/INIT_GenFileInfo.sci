function FileInfo = INIT_GenFileInfo(WorkingDir,OutCCCodeDir,UserSciFilesPaths)
// function FileInfo = INIT_GenFileInfo(WorkingDir,OutCCCodeDir,UserSciFilesPaths)
// -----------------------------------------------------------------
// #RNU_RES_B
// This function creates and initializes FileInfo structure.
//
// Input data:
// WorkingDir: see description in the SCI2CInputParameters.sce file.
// OutCCCodeDir: see description in the SCI2CInputParameters.sce file.
// UserSciFilesPaths: see description in the SCI2CInputParameters.sce file.
//
// Output data:
// FileInfo: structure containing all info about SCI2C files.
//
// #RNU_RES_E
// Status:
// 03-Jan-2008 -- Raffaele Nutricato: Author.
//
// Copyright 2008 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),3,3);


// -------------------------
// --- Main directories. ---
// -------------------------
FileInfo.SCI2CMainDir      = pwd();
FileInfo.WorkingDir        = WorkingDir;
FileInfo.OutCCCodeDir      = OutCCCodeDir;
FileInfo.UserSciFilesPaths = UserSciFilesPaths;

// -------------------
// --- .dat Files. ---
// -------------------
FileInfo.FileInfoDatFile       = fullfile(FileInfo.WorkingDir,'FileInfo.dat');
FileInfo.SharedInfoDatFile     = fullfile(FileInfo.WorkingDir,'SharedInfo.dat');
FileInfo.GlobalVarFileName     = fullfile(FileInfo.WorkingDir,'GBLVAR.dat');
FileInfo.ASTStackDataFile      = fullfile(FileInfo.WorkingDir,'ASTStack.dat');

// ----------------------
// --- SCI2C Library. ---
// ----------------------
FileInfo.SCI2CLibDir           = fullfile(FileInfo.WorkingDir,'SCI2CLib');

FileInfo.SCI2CLibSCIAnnDir     = fullfile(FileInfo.SCI2CLibDir,'SCIAnnotations');
FileInfo.SCI2CLibSCIAnnFun     = fullfile(FileInfo.SCI2CLibSCIAnnDir,'Functions');
FileInfo.SCI2CLibSCIAnnCls     = fullfile(FileInfo.SCI2CLibSCIAnnDir,'Classes');

FileInfo.SCI2CLibSCIFunListDir = fullfile(FileInfo.SCI2CLibDir,'SCIFunctionList');
FileInfo.SCI2CLibSCIFLFun      = fullfile(FileInfo.SCI2CLibSCIFunListDir,'Functions');
FileInfo.SCI2CLibSCIFLCls      = fullfile(FileInfo.SCI2CLibSCIFunListDir,'Classes');

FileInfo.SCI2CLibCAnnDir       = fullfile(FileInfo.SCI2CLibDir,'CAnnotations');
FileInfo.SCI2CLibCAnnFun       = fullfile(FileInfo.SCI2CLibCAnnDir,'Functions');
FileInfo.SCI2CLibCAnnCls       = fullfile(FileInfo.SCI2CLibCAnnDir,'Classes');

FileInfo.SCI2CLibCFunListDir   = fullfile(FileInfo.SCI2CLibDir,'CFunctionList');
FileInfo.SCI2CLibCFLFun        = fullfile(FileInfo.SCI2CLibCFunListDir,'Functions');
FileInfo.SCI2CLibCFLCls        = fullfile(FileInfo.SCI2CLibCFunListDir,'Classes');


// -----------------------
// --- USER2C Library. ---
// -----------------------
FileInfo.USER2CLibDir           = fullfile(FileInfo.WorkingDir,'USER2CLib');

FileInfo.USER2CLibSCIAnnDir     = fullfile(FileInfo.USER2CLibDir,'SCIAnnotations');
FileInfo.USER2CLibSCIAnnFun     = fullfile(FileInfo.USER2CLibSCIAnnDir,'Functions');
FileInfo.USER2CLibSCIAnnCls     = fullfile(FileInfo.USER2CLibSCIAnnDir,'Classes');

FileInfo.USER2CLibSCIFunListDir = fullfile(FileInfo.USER2CLibDir,'SCIFunctionList');
FileInfo.USER2CLibSCIFLFun      = fullfile(FileInfo.USER2CLibSCIFunListDir,'Functions');
FileInfo.USER2CLibSCIFLCls      = fullfile(FileInfo.USER2CLibSCIFunListDir,'Classes');

FileInfo.USER2CLibCAnnDir       = fullfile(FileInfo.USER2CLibDir,'CAnnotations');
FileInfo.USER2CLibCAnnFun       = fullfile(FileInfo.USER2CLibCAnnDir,'Functions');
FileInfo.USER2CLibCAnnCls       = fullfile(FileInfo.USER2CLibCAnnDir,'Classes');

FileInfo.USER2CLibCFunListDir   = fullfile(FileInfo.USER2CLibDir,'CFunctionList');
FileInfo.USER2CLibCFLFun        = fullfile(FileInfo.USER2CLibCFunListDir,'Functions');
FileInfo.USER2CLibCFLCls        = fullfile(FileInfo.USER2CLibCFunListDir,'Classes');

// ----------------------
// --- Function List. ---
// ----------------------
FileInfo.FunctionList.MainDir             = fullfile(FileInfo.WorkingDir,'FunctionList');
FileInfo.FunctionList.SCI2CAvailableCDat  = fullfile(FileInfo.FunctionList.MainDir,'SCI2CAvailableC.dat');
FileInfo.FunctionList.USER2CAvailableCDat = fullfile(FileInfo.FunctionList.MainDir,'USER2CAvailableC.dat');
FileInfo.FunctionList.ConvertedDat        = fullfile(FileInfo.FunctionList.MainDir,'Converted.dat');
FileInfo.FunctionList.ToBeConvertedDat    = fullfile(FileInfo.FunctionList.MainDir,'ToBeConverted.dat');
FileInfo.FunctionList.FunInfoDatDir       = fullfile(FileInfo.FunctionList.MainDir,'FunInfoDatFiles');

// --------------------
// --- Other Files. ---
// --------------------
FileInfo.GeneralReport = fullfile(FileInfo.WorkingDir,'SCI2CGeneralReport.txt');

// -----------------------------------
// --- C-Style paths and Makefile. ---
// -----------------------------------
//-- FileInfo.CStyleSCI2CMainDir = ConvertPathMat2C(FileInfo.SCI2CMainDir,SharedInfo.CCompilerPathStyle);
//-- FileInfo.CStyleOutCCCodeDir = ConvertPathMat2C(OutCCCodeDir,SharedInfo.CCompilerPathStyle);
FileInfo.CStyleSCI2CMainDir = pathconvert(FileInfo.SCI2CMainDir, %f, %f, 'u');
FileInfo.CStyleOutCCCodeDir = pathconvert(OutCCCodeDir, %f, %f, 'u');
FileInfo.MakefileFilename   = fullfile(FileInfo.CStyleOutCCCodeDir,'Makefile');
FileInfo.MakefileTemplate   = fullfile(SCI+'/contrib/scilab2c/macros/CCodeGeneration','SCI2CMakefileTemplate.rc');
endfunction

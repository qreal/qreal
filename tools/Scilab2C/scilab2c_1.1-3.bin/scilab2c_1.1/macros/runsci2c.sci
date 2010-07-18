function runsci2c(UserScilabMainFile, UserSciFilesPaths, SCI2COutputPath, Runmode)
// function runsci2c(SCI2CInputPrmFile)
// -----------------------------------------------------------------
// ===         hArtes/PoliBa/GAP SCI2C tool                      ===
// ===         Authors:                                          ===
// ===            Raffaele Nutricato                             ===
// ===            raffaele.nutricato@tiscali.it                  ===
// ===            Alberto Morea                                  ===
//
// This is the main function of SCI2C.
//
// Input data:
// SCI2CInputPrmFile: path+filename of the input parameters file.
//
// Output data:
// ---
//
// Status:
// 11-Apr-2007 -- Raffaele Nutricato: Author.
// 11-Apr-2007 -- Alberto Morea: Tests.
//
// Copyright 2007 Raffaele Nutricato & Alberto Morea.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// -------------------
// --- Soft reset. ---
// -------------------
//mode(-1);
//clc;
// -----------------------
// --- End Soft reset. ---
// -----------------------

// -------------------------
// --- Input Parameters. ---
// -------------------------
RunSci2CMainDir = pwd();
// -----------------------------
// --- End input Parameters. ---
// -----------------------------

// -------------------------------
// --- Perform Intializations. ---
// -------------------------------
// --- Load SCI2C directories and files. ---
//cd(fullfile(RunSci2CMainDir,'ToolInitialization'));
//exec('INIT_SCI2CLoader.sce');
//cd(RunSci2CMainDir);

// --- Initialize the SCI2C tool directories and files. ---
[FileInfoDatFile,SharedInfoDatFile] = INIT_SCI2C(UserScilabMainFile, ...
						 UserSciFilesPaths, SCI2COutputPath, RunMode);

// -- Load FileInfo and SharedInfo
load(SharedInfoDatFile,'SharedInfo');
load(FileInfoDatFile,'FileInfo');

RunMode = SharedInfo.RunMode;

// --- Generation of the library structure. ---
if (RunMode == 'GenLibraryStructure' | RunMode == 'All')
   INIT_GenLibraries(FileInfoDatFile);
end

// --- Load Library Info. ---
INIT_LoadLibraries(FileInfoDatFile);

// -----------------------------------
// --- End Perform Intializations. ---
// -----------------------------------

// ----------------------------------
// --- Perform SCI2C Translation. ---
// ----------------------------------
if (RunMode == 'All' | RunMode == 'Translate')
   FlagContinueTranslation = 1;
   while(FlagContinueTranslation == 1)
      UpdateSCI2CInfo(FileInfoDatFile);
      AST_GetASTFile(FileInfoDatFile);
      AST2Ccode(FileInfoDatFile);
      JoinDeclarAndCcode(FileInfoDatFile);
      FlagContinueTranslation = ManageNextConversion(FileInfoDatFile);
   end
end



// ---------------------------
// --- Copy library files. ---
// ---------------------------
sci2cVersion = "1.1"
sci2cIntallDir = SCI + "/contrib/scilab2c/" + sci2cVersion + "/";
allSources = sci2cIntallDir + getAllSources();
allHeaders = sci2cIntallDir + getAllHeaders();
allInterfaces = sci2cIntallDir + getAllInterfaces();

mkdir(SCI2COutputPath+"/src/");
mkdir(SCI2COutputPath+"/src/c/");
mkdir(SCI2COutputPath+"/includes/");
mkdir(SCI2COutputPath+"/interfaces/");

// -- Sources
PrintStepInfo('Copying sources needed in ' + SCI2COutputPath + ...
	      "/src/c/", FileInfo.GeneralReport,'both');
for i = 1:size(allSources, "*")
  disp("Copying "+allSources(i)+" in "+SCI2COutputPath+"/src/c/");
  copyfile(allSources(i), SCI2COutputPath+"/src/c/");
end

// -- Includes
PrintStepInfo('Copying headers needed in ' + SCI2COutputPath + ...
	      "/includes/", FileInfo.GeneralReport,'both');
for i = 1:size(allHeaders, "*")
  disp("Copying "+allHeaders(i)+" in "+SCI2COutputPath+"/includes/");
  copyfile(allHeaders(i), SCI2COutputPath+"/includes/");
end

// -- Interfaces
PrintStepInfo('Copying interfaces needed in ' + SCI2COutputPath + ...
	      "/interfaces/", FileInfo.GeneralReport,'both');
for i = 1:size(allInterfaces, "*")
  disp("Copying "+allInterfaces(i)+" in "+SCI2COutputPath+"/interfaces/");
  copyfile(allInterfaces(i), SCI2COutputPath+"/interfaces/");
end

// --------------------------
// --- Generate Makefile. ---
// --------------------------
C_GenerateMakefile(FileInfo,SharedInfo);

// -----------------
// --- Epilogue. ---
// -----------------
if (RunMode == 'All' | RunMode == 'Translate')
   PrintStepInfo('Translation Successfully Completed!!!',FileInfo.GeneralReport,'both');
elseif (RunMode == 'GenLibraryStructure')
   PrintStepInfo('Library Structure Successfully Created!!!',FileInfo.GeneralReport,'both');
end
endfunction

function AST_GetASTFile(FileInfoDatFile)
// function AST_GetASTFile(FileInfoDatFile)
// -----------------------------------------------------------------
// Generates the AST file starting from the .sci file specified
// in SharedInfo.NextSCIFileName.
//
// Input data:
// FileInfoDatFile: name of the .dat file containing the FileInfo structure.
//
// Output data:
// ---
//
// Status:
// 11-Apr-2007 -- Raffaele Nutricato: Author.
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

PrintStepInfo('Generate the AST in '+FileInfo.Funct(funnumber).ASTFileName,...
   FileInfo.GeneralReport,'both');

// --- Generation of the AST file. ---
SciFile2ASTFile(FileInfo.Funct(funnumber).SCIFileName,...
   FileInfo.Funct(funnumber).ASTFileName);

// ---------------------
// --- Save section. ---
// ---------------------
// --- Save File Info Structure. ---
// save(FileInfoDatFile,FileInfo);
// -------------------------
// --- End save section. ---
// -------------------------

endfunction

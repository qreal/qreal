function INIT_GenLibraries(FileInfoDatFile)
// function INIT_GenLibraries(FileInfoDatFile)
// -----------------------------------------------------------------
// #RNU_RES_B
// This function initializes the SCI2C and USER libraries.
// For each Scilab function a .ann file is created where the function
// annotations are listed into it.
// #RNU_RES_E
//
// Input data:
// FileInfoDatFile: name of the .dat file containing the FileInfo structure.
//
// Output data:
// ---
//
// Status:
// 12-Jun-2007 -- Nutricato Raffaele: Author.
// 03-Jan-2008 -- Nutricato Raffaele: Changed directory structure.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),1,1);

// -----------------------
// --- Initialization. ---
// -----------------------
// --- Load File Info Structure. ---
clear FileInfo
load(FileInfoDatFile,'FileInfo');

// --- Load Shared Info Structure. ---
clear SharedInfo
load(FileInfo.SharedInfoDatFile,'SharedInfo');

PrintStepInfo('Initialize SCI2C and USER2C Libraries.',...
   FileInfo.GeneralReport,'both');
// ---------------------------
// --- End Initialization. ---
// ---------------------------

// -----------------------------------------------------------
// --- Fills SCI2C and USER2C libs with appropriate files. ---
// -----------------------------------------------------------
INIT_FillSCI2LibCDirs(FileInfo,SharedInfo.Extension);

// #RNU_RES_B
//NUT the following functions will be useful in next release
//NUT for advanced use of SCI2C
//INIT_FillSCI2LibSCIDirs(FileInfo,SharedInfo.Extension);
//INIT_FillUSER2LibCDirs(FileInfo,SharedInfo.Extension);
//INIT_FillUSER2LibSCIDirs(FileInfo,SharedInfo.Extension);
// ---------------------------------------------------------------
// --- End Fills SCI2C and USER2C libs with appropriate files. ---
// ---------------------------------------------------------------
// #RNU_RES_E
endfunction

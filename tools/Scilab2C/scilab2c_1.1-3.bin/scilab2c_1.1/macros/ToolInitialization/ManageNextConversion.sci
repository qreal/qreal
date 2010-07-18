function FlagContinueTranslation = ManageNextConversion(FileInfoDatFile)
// function FlagContinueTranslation = ManageNextConversion(FileInfoDatFile)
// -----------------------------------------------------------------
// //NUT: add description here
//
// Input data:
// //NUT: add description here
//
// Output data:
// //NUT: add description here
//
// Status:
// 27-Oct-2007 -- Raffaele Nutricato: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

//NUT: verifica se update e managenexconversion possono essere integrate in un'unica funzione.

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),1,1);

// ---------------------
// --- Load section. ---
// ---------------------
// --- Load File Info Structure. ---
load(FileInfoDatFile,'FileInfo');

// --- Load Shared Info Structure. ---
load(FileInfo.SharedInfoDatFile,'SharedInfo');

// --- Load ToBeConverted .dat file. ---
load(FileInfo.FunctionList.ToBeConvertedDat,'ToBeConverted');
// -------------------------
// --- End load section. ---
// -------------------------

FlagContinueTranslation = 0;

// ------------------------------------
// --- Finalize the current C code. ---
// ------------------------------------
C_FinalizeCode(FileInfo,SharedInfo);

// ------------------------------------------------
// --- Identify the next function to translate. ---
// ------------------------------------------------
SharedInfo.NFilesToTranslate = SharedInfo.NFilesToTranslate - 1;
if (SharedInfo.NFilesToTranslate >= 1)
   // Remove the translated C function from the ToBeConverted list
   ToBeConverted(1) = [];
   FlagContinueTranslation = 1;
   SharedInfo.NextSCIFunName   = ToBeConverted(1).SCIFunctionName;
   SharedInfo.NextCFunName     = ToBeConverted(1).CFunctionName; 
   SharedInfo.NextSCIFunNumber = SharedInfo.NextSCIFunNumber + 1;  
   [FlagFound,SharedInfo.NextSCIFileName] = ...
      SCI2CFindFile(FileInfo.UserSciFilesPaths,SharedInfo.NextSCIFunName+'.sci');
   if (FlagFound == 0)
      SCI2CerrorFile('Cannot find a scilab file to generate ""'+SharedInfo.NextCFunName+'"".',...
         FileInfo.GeneralReport);
   end
end
// ----------------------------------------------------
// --- End Identify the next function to translate. ---
// ----------------------------------------------------


// ---------------------
// --- Save section. ---
// ---------------------
// --- Save Shared Info Structure. ---
save(FileInfo.SharedInfoDatFile,SharedInfo);
clear SharedInfo

// --- Save ToBeConverted .dat file. ---
save(FileInfo.FunctionList.ToBeConvertedDat,ToBeConverted);
clear ToBeConverted

clear FileInfo
// -------------------------
// --- End Save section. ---
// -------------------------

endfunction

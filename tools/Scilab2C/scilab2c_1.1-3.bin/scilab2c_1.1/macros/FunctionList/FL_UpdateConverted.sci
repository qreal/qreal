function Converted = FL_UpdateConverted(NFilesToTranslate,ConvertedDatFile)
// function Converted = FL_UpdateConverted(NFilesToTranslate,ConvertedDatFile)
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

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),2,2);

// -----------------------
// --- Initialization. ---
// -----------------------
// --- Load Converted .dat file. ---
load(ConvertedDatFile,'Converted');
// ---------------------------
// --- End Initialization. ---
// ---------------------------


if (NFilesToTranslate >= 1)
   // ---------------------------------------
   // --- Update Converted Function List. ---
   // ---------------------------------------
   // --- Insert the current function into the converted function list. ---
   NConvP1 = size(Converted,1)+1;
   Converted(NConvP1) = SharedInfo.NextCFunName;
   // -------------------------------------------
   // --- End Update Converted Function List. ---
   // -------------------------------------------
end

endfunction

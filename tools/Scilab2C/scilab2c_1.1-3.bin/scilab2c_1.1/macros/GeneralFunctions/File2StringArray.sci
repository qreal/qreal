function [String_Array,N_Strings] = File2StringArray(InFileName)
// function [String_Array,N_Strings] = File2StringArray(InFileName)
// -----------------------------------------------------------------
// #RNU_RES_B
// Reads a text file and stores every line into a string array.
// #RNU_RES_E
//
// Input data:
// InFileName: path+filename of the input file.
//
// Output data:
// String_Array: array of strings containing the lines of the input
//               text file.
// N_Strings: number of strings stored in String_Array.
//
// Status:
// 10-Nov-2007 -- Raffaele Nutricato: Author.
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
N_Strings    = 0;
String_Array = '';
// ---------------------------
// --- End Initialization. ---
// ---------------------------

// --------------------
// --- Open C file. ---
// --------------------
fidfile = SCI2COpenFileRead(InFileName);

// -------------------
// --- Read lines. ---
// -------------------
tmpline = mgetl(fidfile,1);
while (meof(fidfile) == 0)
   N_Strings = N_Strings + 1;
   String_Array(N_Strings) = tmpline;
   tmpline = mgetl(fidfile,1);
end

mclose(fidfile);
endfunction

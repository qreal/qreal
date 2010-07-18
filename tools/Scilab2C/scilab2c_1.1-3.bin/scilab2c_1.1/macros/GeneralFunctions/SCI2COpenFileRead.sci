function fidnumber = SCI2COpenFileRead(filename)
// function fidnumber = SCI2COpenFileRead(filename)
// --------------------------------------------------------------------------------
// Open a file in read mode.
//
// Input data:
// filename: path + name of the file to read.
//
// Output data:
// fidnumber: file identifier.
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
SCI2CNInArgCheck(argn(2),1,1);

// --- Open the .sci file (read only). ---
[fidnumber,fiderror] = mopen(filename,'r'); 
if (fiderror < 0)
   SCI2Cerror(['Cannot open (in read mode): '+filename]);
end

endfunction

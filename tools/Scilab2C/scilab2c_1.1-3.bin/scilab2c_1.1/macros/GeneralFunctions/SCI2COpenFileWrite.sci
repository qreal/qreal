function fidnumber = SCI2COpenFileWrite(filename)
// function fidnumber = SCI2COpenFileWrite(filename)
// --------------------------------------------------------------------------------
// Open a file in write mode.
//
// Input data:
// filename: path + name of the file to be written.
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

// --- Open the .sci file (write mode). ---
[fidnumber,fiderror] = mopen(filename,'w');
if (fiderror < 0)
   SCI2Cerror(['Cannot open (in write mode): '+filename]);
end

endfunction

function [StringArray] = Array2String(InArray);
// function [StringArray] = Array2String(InArray);
// -----------------------------------------------------------------
// #RNU_RES_B
// Converts an input array into a string. Maximum 2D array are allowed.
// Ex.: InArray = [10, 4];
//      StringArray = "[10, 4]";
// #RNU_RES_E
//
// Input data:
// InArray: Input array.
//
// Output data:
// StringArray: array converted into a string.
//
// Status:
// 13-May-2007 -- Nutricato Raffaele: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),1,1);

[Nrows,Ncols] = size(InArray);

StringArray = '[';
for counterrows = 1:Nrows
   for countercols = 1:Ncols
      StringArray = StringArray + string(InArray(counterrows,countercols)) + ',';
   end
   StringArray = part(StringArray,1:(length(StringArray)-1)); // Remove the last ','
   StringArray = StringArray+';';
end
StringArray = part(StringArray,1:(length(StringArray)-1)); // Remove the last ';'
StringArray = StringArray+']';
endfunction

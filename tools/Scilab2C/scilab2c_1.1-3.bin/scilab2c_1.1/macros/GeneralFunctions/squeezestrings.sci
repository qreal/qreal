function OutString = squeezestrings(InStringArray)
// function OutString = squeezestrings(InStringArray)
// -----------------------------------------------------------------
// #RNU_RES_B
// Converts an array of strings into a single string.
//
// Input data:
// InStringArray: Array of strings.
//
// Output data:
// OutString: Output string.
//
// #RNU_RES_E
// Status:
// 12-Apr-2007 -- Nutricato Raffaele: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),1,1);

OutString = [];
for counterstrings = 1:max(size(InStringArray))
   OutString = OutString+InStringArray(counterstrings);
end

endfunction

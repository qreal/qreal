function OutputData = SCI2Cflipud(InputData)
// function OutputData = SCI2Cflipud(InputData)
// -----------------------------------------------------------------
// #RNU_RES_B
// Inverts (flips) the position of the arguments of InputData.
// Input data can be a struct or an array.
// Ex.:
// A(1) = 'one';
// A(2) = 'two';
// A(3) = 'three';
// B = SCI2Cflipud(A);
// B(1) = 'three';
// B(2) = 'two';
// B(3) = 'one';
//
// Input data:
// InputData: input array or structure.
//
// Output data:
// OutputData: flipped version of the input array.
//
// #RNU_RES_E
// Status:
// 12-May-2007 -- Nutricato Raffaele: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),1,1);

NInputs = size(InputData,1);
OutputData = InputData; // To be sure that they will have the same structure.
for cnt = 1:NInputs
   OutputData(cnt) = InputData(NInputs-cnt+1);
end
endfunction

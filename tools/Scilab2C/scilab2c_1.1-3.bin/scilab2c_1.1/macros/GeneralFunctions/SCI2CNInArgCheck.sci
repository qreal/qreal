function SCI2CNInArgCheck(NInArgs,MinNArgs,MaxNArgs)
// function SCI2CNInArgCheck(NInArgs,MinNArgs,MaxNArgs)
// -----------------------------------------------------------------
// #RNU_RES_B
// Check that NInArgs is in the range specified by MinNArgs and 
// MaxNArgs.
//
// Input data:
// NInArgs: number of input arguments of the function under test.
// MinNArgs: minimum number of input arguments allowed.
// MaxNArgs: maximum number of input arguments allowed.
//
// Output data:
// ---
// #RNU_RES_E
//
// Status:
// 23-Nov-2007 -- Raffaele Nutricato: Author.
//
// Copyright 2007 Raffaele Nutricato
// -----------------------------------------------------------------

if ((NInArgs < MinNArgs) | (NInArgs > MaxNArgs))
   SCI2Cerror('Incorrect number of input arguments.');
end


endfunction

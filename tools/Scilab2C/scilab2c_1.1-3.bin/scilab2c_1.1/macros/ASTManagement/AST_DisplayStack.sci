function AST_DisplayStack()
// function AST_DisplayStack()
// -----------------------------------------------------------------
// Displays the AST stack content. The AST stack is used to read the
// AST.
//
// Input data:
// ---
//
// Output data:
// //NUT: add description here
//
// Status:
// 11-Apr-2007 -- Raffaele Nutricato: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------


// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),0,0);

global SCI2CSTACK 
global StackPosition;
global STACKDEDUG
   
disp('*********************')
disp('*********************')

if (STACKDEDUG == 1)
   for counterposition = 1:StackPosition
      disp(SCI2CSTACK(counterposition,1))
   end
end
disp('---------------------')
disp('---------------------')
disp(' ');disp(' ');disp('Press return to continue'); halt;

endfunction

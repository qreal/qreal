function AST_PushASTStack(stackelement)
// function AST_PushASTStack(stackelement)
// -----------------------------------------------------------------
// Push the AST stack.
//
// Input data:
// ---
//
// Output data:
// //NUT: add description here
//
// Status:
// 11-Aug-2007 -- Raffaele Nutricato: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),1,1);


global SCI2CSTACK 
global StackPosition;
global STACKDEDUG

StackPosition = StackPosition + 1;
SCI2CSTACK(StackPosition,1) = stackelement;

if (STACKDEDUG == 1)
   AST_DisplayStack();
end

endfunction

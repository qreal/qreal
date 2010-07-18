function stackelement = AST_PopASTStack()
// function stackelement = AST_PopASTStack()
// -----------------------------------------------------------------
// Pop the AST stack.
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
SCI2CNInArgCheck(argn(2),0,0);

global SCI2CSTACK; 
global StackPosition;
global STACKDEDUG;
	
if StackPosition == 1
   SCI2Cerror('Stack empty. Cannot pop from stack.');
end
   
stackelement = SCI2CSTACK(StackPosition,1);
SCI2CSTACK = SCI2CSTACK(1:StackPosition-1);
StackPosition = StackPosition - 1;

if (STACKDEDUG == 1)
   AST_DisplayStack();
end
   
endfunction

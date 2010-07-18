function C_SCI2CHeader(FileName)
// function C_SCI2CHeader(FileName)
// -----------------------------------------------------------------
// //NUT: add description here
//
// Input data:
// //NUT: add description here
//
// Output data:
// //NUT: add description here
//
// Status:
// 21-Dec-2007 -- Raffaele Nutricato: Author.
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
// ---------------------------
// --- End Initialization. ---
// ---------------------------

      
PrintStringInfo('/*',FileName,'file','y');
PrintStringInfo('** ************************************************',FileName,'file','y');
PrintStringInfo('** hArtes/POLIBA SCILAB2C',FileName,'file','y');
PrintStringInfo('** Contact: raffaele.nutricato@tiscali.it',FileName,'file','y');
PrintStringInfo('** ************************************************',FileName,'file','y');
PrintStringInfo('*/',FileName,'file','y');
PrintStringInfo(' ',FileName,'file','y');
PrintStringInfo(' ',FileName,'file','y');

endfunction

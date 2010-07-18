function C_InitHeader(C_Prototype,HeaderFileName,Sci2CLibMainHeaderFName)
// function C_InitHeader(C_Prototype,HeaderFileName,Sci2CLibMainHeaderFName)
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
// 27-Oct-2007 -- Raffaele Nutricato: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),3,3);

// -----------------------
// --- Initialization. ---
// -----------------------
// ---------------------------
// --- End Initialization. ---
// ---------------------------

      
C_SCI2CHeader(HeaderFileName);
[tmppath,tmpfname,tmpextension]=fileparts(HeaderFileName);
PrintStringInfo('#ifndef '+tmpfname+'_h',HeaderFileName,'file','y');
PrintStringInfo('#define '+tmpfname+'_h',HeaderFileName,'file','y');
PrintStringInfo('/*',HeaderFileName,'file','y');
PrintStringInfo('** ----------------------- ',HeaderFileName,'file','y');
PrintStringInfo('** --- SCI2C Includes. --- ',HeaderFileName,'file','y');
PrintStringInfo('** ----------------------- ',HeaderFileName,'file','y');
PrintStringInfo('*/',HeaderFileName,'file','y');
PrintStringInfo('#include ""'+Sci2CLibMainHeaderFName+'""',HeaderFileName,'file','y');
PrintStringInfo('/*',HeaderFileName,'file','y');
PrintStringInfo('** --------------------------- ',HeaderFileName,'file','y');
PrintStringInfo('** --- End SCI2C Includes. --- ',HeaderFileName,'file','y');
PrintStringInfo('** --------------------------- ',HeaderFileName,'file','y');
PrintStringInfo('*/',HeaderFileName,'file','y');
PrintStringInfo(' ',HeaderFileName,'file','y');
PrintStringInfo(' ',HeaderFileName,'file','y');
PrintStringInfo('/*',HeaderFileName,'file','y');
PrintStringInfo('** ------------------- ',HeaderFileName,'file','y');
PrintStringInfo('** --- Prototypes. --- ',HeaderFileName,'file','y');
PrintStringInfo('** ------------------- ',HeaderFileName,'file','y');
PrintStringInfo('*/',HeaderFileName,'file','y');
PrintStringInfo(C_IndentBlanks(0)+C_Prototype,HeaderFileName,'file','y');
PrintStringInfo('/*',HeaderFileName,'file','y');
PrintStringInfo('** ----------------------- ',HeaderFileName,'file','y');
PrintStringInfo('** --- End Prototypes. --- ',HeaderFileName,'file','y');
PrintStringInfo('** ----------------------- ',HeaderFileName,'file','y');
PrintStringInfo('*/',HeaderFileName,'file','y');
PrintStringInfo(' ',HeaderFileName,'file','y');
PrintStringInfo(' ',HeaderFileName,'file','y');
PrintStringInfo('/*',HeaderFileName,'file','y');
PrintStringInfo('** ------------------------ ',HeaderFileName,'file','y');
PrintStringInfo('** --- USER2C Includes. --- ',HeaderFileName,'file','y');
PrintStringInfo('** ------------------------ ',HeaderFileName,'file','y');
PrintStringInfo('*/',HeaderFileName,'file','y');

endfunction

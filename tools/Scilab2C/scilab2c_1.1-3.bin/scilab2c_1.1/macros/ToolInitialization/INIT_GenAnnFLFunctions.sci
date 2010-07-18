function INIT_GenAnnFLFunctions(FunctionName,FunctionsOutDir,ClassName,ReportFile,ExtensionCAnnFun)
// function INIT_GenAnnFLFunctions(FunctionName,FunctionsOutDir,ClassName,ReportFile,ExtensionCAnnFun)
// -----------------------------------------------------------------
// Generates annotation or file list files. 
//
// Input data:
// //NUT: add description here
//
// Output data:
// //NUT: add description here
//
// Status:
// 17-Jun-2007 -- Raffaele Nutricato: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),5,5);

FunctionExtension = ExtensionCAnnFun;
FunctionFileName = fullfile(FunctionsOutDir,FunctionName+FunctionExtension);
PrintStringInfo('CLASS: '+ClassName,FunctionFileName,'file','y');

endfunction

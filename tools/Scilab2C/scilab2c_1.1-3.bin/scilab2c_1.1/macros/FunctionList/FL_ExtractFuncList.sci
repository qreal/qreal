function [CFuncList,NElements] = FL_ExtractFuncList(FunctionDir,ClassDir,SCI2CClassSpecifier,ExtFLCls,ReportFileName)
// function [CFuncList,NElements] = FL_ExtractFuncList(FunctionDir,ClassDir,SCI2CClassSpecifier,ExtFLCls,ReportFileName)
// -----------------------------------------------------------------
// #RNU_RES_B
// Extracts the list of the C functions available. To do that
// this function enters in the directories where the .clst and 
// .lst files are stored.
// #RNU_RES_E
//
// Input data:
// //NUT: add description here
//
// Output data:
// //NUT: add description here
//
// Status:
// 05-Jan-2008 -- Nutricato Raffaele: Author.
//
// Copyright 2008 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------


// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),5,5);

// #RNU_RES_B
// ---------------------------------------------------------
// --- Extract the list of files in Functions directory. ---
// ---------------------------------------------------------
// #RNU_RES_E
tmppwd = pwd();
cd(FunctionDir);
// funfiles = ls();
funfiles = listfiles();
cd(tmppwd);
NFunFiles = size(funfiles,1);

// #RNU_RES_B
// -----------------------------------------------------------
// --- Extract the C function list from Classes directory. ---
// -----------------------------------------------------------
// #RNU_RES_E
CFuncList = '';
NElements = 0;

for cntfun = 1:NFunFiles
   FunFileName        = fullfile(FunctionDir,funfiles(cntfun));
   ClassName          = FL_GetFunctionClass(FunFileName,SCI2CClassSpecifier,ReportFileName);
   ClassFileName      = fullfile(ClassDir,ClassName);
   [tmpfunlist,tmpnelem] = File2StringArray(ClassFileName+ExtFLCls);
   [tmppath,tmpfunname,tmpext] = fileparts(FunFileName);
   tmpfunlist = FL_InOutArgs2CFunNames(tmpfunname,tmpfunlist,tmpnelem);
   for cnttmpfun = 1:tmpnelem
      NElements = NElements + 1;
      CFuncList(NElements) = tmpfunlist(cnttmpfun);
   end
end

endfunction

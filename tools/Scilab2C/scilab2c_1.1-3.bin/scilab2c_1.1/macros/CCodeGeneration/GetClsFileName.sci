function SCI2CClassFileName = GetClsFileName(FunName,FileInfo,SharedInfo)
// function SCI2CClassFileName = GetClsFileName(FunName,FileInfo,SharedInfo)
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
// 11-Jul-2007 -- Nutricato Raffaele: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),3,3);

// //NUT: verifica che il nome sia accettabile e che non
// //NUT: occorra spezzettarla in piu funzioni.

// --- Extraction of the function name and number. ---
nxtscifunname   = SharedInfo.NextSCIFunName;
nxtscifunnumber = SharedInfo.NextSCIFunNumber;

ReportFileName = FileInfo.Funct(nxtscifunnumber).ReportFileName;

// --- Initialization. ---
tmpannfilename   = FunName+'.ann';
tmpscifilename   = FunName+'.sci';
AnnFileName      = '';
ClsFileName      = ''

SCI2CClassSpecifier = SharedInfo.Annotations.FUNCLASS;
FlagFoundAnnFile = 0;
// #RNU_RES_B
//NUT: qui e' presente la lista delle priorita' di accesso alle annotazioni.
// #RNU_RES_E
if SCI2Cfileexist(FileInfo.USER2CLibCAnnFun,tmpannfilename)
   // #RNU_RES_B
   // It is a C function of the USER2C library.
   // #RNU_RES_E
   FlagFoundAnnFile   = 1;
   AnnFileName        = fullfile(FileInfo.USER2CLibCAnnFun,tmpannfilename);
   SCI2CClassName     = FL_GetFunctionClass(AnnFileName,SCI2CClassSpecifier,ReportFileName);
   SCI2CClassFileName = fullfile(FileInfo.USER2CLibCAnnCls,SCI2CClassName+'.acls');
elseif SCI2Cfileexist(FileInfo.USER2CLibSCIAnnFun,tmpannfilename)
   // #RNU_RES_B
   // It is a scilab function of the USER2C library.
   // #RNU_RES_E
   FlagFoundAnnFile   = 1;
   AnnFileName        = fullfile(FileInfo.USER2CLibSCIAnnFun,tmpannfilename);
   SCI2CClassName     = FL_GetFunctionClass(AnnFileName,SCI2CClassSpecifier,ReportFileName);
   SCI2CClassFileName = fullfile(FileInfo.USER2CLibSCIAnnCls,SCI2CClassName+'.acls');
elseif (SCI2Cfileexist(FileInfo.SCI2CLibCAnnFun,tmpannfilename))
   // #RNU_RES_B
   // It is a C function of the SCI2C library.
   // #RNU_RES_E
   FlagFoundAnnFile   = 1;
   AnnFileName        = fullfile(FileInfo.SCI2CLibCAnnFun,tmpannfilename);
   SCI2CClassName     = FL_GetFunctionClass(AnnFileName,SCI2CClassSpecifier,ReportFileName);
   SCI2CClassFileName = fullfile(FileInfo.SCI2CLibCAnnCls,SCI2CClassName+'.acls');
elseif (SCI2Cfileexist(FileInfo.SCI2CLibSCIAnnFun,tmpannfilename))
   // #RNU_RES_B
   // It is a scilab function of the SCI2C library.
   // #RNU_RES_E
   FlagFoundAnnFile   = 1;
   AnnFileName        = fullfile(FileInfo.SCI2CLibSCIAnnFun,tmpannfilename);
   SCI2CClassName     = FL_GetFunctionClass(AnnFileName,SCI2CClassSpecifier,ReportFileName);
   SCI2CClassFileName = fullfile(FileInfo.SCI2CLibSCIAnnCls,SCI2CClassName+'.acls');
end

if (FlagFoundAnnFile == 0)
   [FlagFoundAnnFile,fullpathscifilename] = SCI2CFindFile(FileInfo.UserSciFilesPaths,FunName+'.sci');
   if (FlagFoundAnnFile == 0)
      // #RNU_RES_B
      PrintStringInfo(' ',ReportFileName,'both','y');
      PrintStringInfo('SCI2CERROR: Missing function annotation. Could not find',ReportFileName,'both','y');
      PrintStringInfo('SCI2CERROR: an associated .sci or .ann file for function: '+FunName,ReportFileName,'both','y');
      PrintStringInfo(' ',ReportFileName,'both','y');
      // #RNU_RES_E
      SCI2Cerror(' ');
   end
   AnnFileName        = fullfile(FileInfo.USER2CLibSCIAnnFun,tmpannfilename);
   SCI2CClassName     = FunName;
   SCI2CClassFileName = fullfile(FileInfo.USER2CLibSCIAnnCls,SCI2CClassName+'.acls');
   Sci2AnnotationFile(fullpathscifilename,SCI2CClassFileName,AnnFileName,...
      SharedInfo.Annotations.USERFUN,ReportFileName);
end
endfunction

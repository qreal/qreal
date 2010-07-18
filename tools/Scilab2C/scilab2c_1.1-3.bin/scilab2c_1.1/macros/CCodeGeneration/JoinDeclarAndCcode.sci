function JoinDeclarAndCcode(FileInfoDatFile)
// function JoinDeclarAndCcode(FileInfoDatFile)
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
// 07-Nov-2007 -- Raffaele Nutricato: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),1,1);

// ---------------------
// --- Load section. ---
// ---------------------
// --- Load File Info Structure. ---
load(FileInfoDatFile,'FileInfo');

// --- Load Shared Info Structure. ---
load(FileInfo.SharedInfoDatFile,'SharedInfo');
// -------------------------
// --- End load section. ---
// -------------------------

// -----------------------
// --- Initialization. ---
// -----------------------
nxtscifunname   = SharedInfo.NextSCIFunName;
funnumber = SharedInfo.NextSCIFunNumber;

CPass1FileName       = FileInfo.Funct(funnumber).CPass1FileName;
CPass2FileName       = FileInfo.Funct(funnumber).CPass2FileName;
CDeclarationFileName = FileInfo.Funct(funnumber).CDeclarationFileName;
CGblDeclarFileName   = FileInfo.Funct(funnumber).CGblDeclarFileName;
CInitVarsFileName    = FileInfo.Funct(funnumber).CInitVarsFileName;
ReportFileName       = FileInfo.Funct(funnumber).ReportFileName;

CPass1V1FileFid      = SCI2COpenFileRead(CPass1FileName);
CDeclarationFileFid  = SCI2COpenFileRead(CDeclarationFileName);
CGblDeclarFileFid    = SCI2COpenFileRead(CGblDeclarFileName);
CInitVarsFileFid     = SCI2COpenFileRead(CInitVarsFileName);
// ---------------------------
// --- End Initialization. ---
// ---------------------------

PrintStepInfo('Joining declaration and C-call files',ReportFileName,'file');

PrintStringInfo('/*',CPass2FileName,'file','y');
PrintStringInfo('** -------------------------------------',CPass2FileName,'file','y');
PrintStringInfo('** --- Global Variables Declaration. ---',CPass2FileName,'file','y');
PrintStringInfo('** -------------------------------------',CPass2FileName,'file','y');
PrintStringInfo('*/',CPass2FileName,'file','y');
// #RNU_RES_B
// --- Copy in V2 the global declaration file. ---
// #RNU_RES_E
while (~meof(CGblDeclarFileFid))
   // Read a line from C Global Declaration file.
   tmpcline = mgetl(CGblDeclarFileFid,1);
   if (length(tmpcline) == 0)
      tmpcline = ' ';
   end

   noblkstmpcline = stripblanks(tmpcline);
   PrintStringInfo(tmpcline,CPass2FileName,'file','y');
end
PrintStringInfo('/*',CPass2FileName,'file','y');
PrintStringInfo('** -----------------------------------------',CPass2FileName,'file','y');
PrintStringInfo('** --- End Global Variables Declaration. ---',CPass2FileName,'file','y');
PrintStringInfo('** -----------------------------------------',CPass2FileName,'file','y');
PrintStringInfo('*/',CPass2FileName,'file','y');
PrintStringInfo(' ',CPass2FileName,'file','y');

// #RNU_RES_B
// --- Copy in V2 the first part of V1 up to "{". ---
// #RNU_RES_E
FoundCurlyBracket = 0;
while ((~meof(CPass1V1FileFid)) & (FoundCurlyBracket == 0))
   // Read a line from C Pass1 file.
   tmpcline = mgetl(CPass1V1FileFid,1);
   noblkstmpcline = stripblanks(tmpcline);
   if (length(noblkstmpcline) > 0)
      if (SCI2Cstrncmps1size('{',noblkstmpcline))
         FoundCurlyBracket = 1;
      end
   else
      tmpcline = ' ';
   end
   PrintStringInfo(tmpcline,CPass2FileName,'file','y');
end

if (FoundCurlyBracket == 0)
   SCI2CerrorFile('""{"" char not found in:'+CPass1FileName,ReportFileName);
end

PrintStringInfo('/*',CPass2FileName,'file','y');
PrintStringInfo('** -----------------------------',CPass2FileName,'file','y');
PrintStringInfo('** --- Variable Declaration. ---',CPass2FileName,'file','y');
PrintStringInfo('** -----------------------------',CPass2FileName,'file','y');
PrintStringInfo('*/',CPass2FileName,'file','y');
// --- Copy in V2 the declaration file. ---
while (~meof(CDeclarationFileFid))
   // Read a line from C Declaration file.
   tmpcline = mgetl(CDeclarationFileFid,1);
   if (length(tmpcline) == 0)
      tmpcline = ' ';
   end

   noblkstmpcline = stripblanks(tmpcline);
   PrintStringInfo(tmpcline,CPass2FileName,'file','y');
end

// #RNU_RES_B
// --- Copy in V2 the variable initialization file. ---
// #RNU_RES_E
while (~meof(CInitVarsFileFid))
   // Read a line from C Declaration file.
   tmpcline = mgetl(CInitVarsFileFid,1);
   if (length(tmpcline) == 0)
      tmpcline = ' ';
   end

   noblkstmpcline = stripblanks(tmpcline);
   PrintStringInfo(tmpcline,CPass2FileName,'file','y');
end
PrintStringInfo('/*',CPass2FileName,'file','y');
PrintStringInfo('** ---------------------------------',CPass2FileName,'file','y');
PrintStringInfo('** --- End Variable Declaration. ---',CPass2FileName,'file','y');
PrintStringInfo('** ---------------------------------',CPass2FileName,'file','y');
PrintStringInfo('*/',CPass2FileName,'file','y');


PrintStringInfo('/*',CPass2FileName,'file','y');
PrintStringInfo('** ---------------',CPass2FileName,'file','y');
PrintStringInfo('** --- C code. ---',CPass2FileName,'file','y');
PrintStringInfo('** ---------------',CPass2FileName,'file','y');
PrintStringInfo('*/',CPass2FileName,'file','y');
// --- Copy the remaining part of V1 in V2. ---
while (~meof(CPass1V1FileFid))
   // #RNU_RES_B
   // Read a line from C Pass1 file.
   // #RNU_RES_E
   tmpcline = mgetl(CPass1V1FileFid,1);
   if (length(tmpcline) == 0)
      tmpcline = ' ';
   end
   PrintStringInfo(tmpcline,CPass2FileName,'file','y');
end

// --------------------
// --- Close Files. ---
// --------------------
mclose(CPass1V1FileFid);
mclose(CDeclarationFileFid);
mclose(CGblDeclarFileFid);
mclose(CInitVarsFileFid);

endfunction

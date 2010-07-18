function AST_HandleEOL(FileInfo,SharedInfo)
// function AST_HandleEOL(FileInfo,SharedInfo)
// -----------------------------------------------------------------
// Handles the EOL tag of the AST.
//
// Input data:
// //NUT: add description here
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
SCI2CNInArgCheck(argn(2),2,2);


//#RNU_RES_B
//NUT: questa parte e' molto interessante perche' ti puo' aiutare per fare confronti
//NUT: incrociati tra le annotazioni della funzione e gli argomenti in uscita.
//NUT: in particolare una volta messi nella tabella dei simboli anche gli argomenti
//NUT: di uscita puoi benissimo verificare che li stai utilizzando bene nel corpo della funzione stessa.
//#RNU_RES_E

// -----------------------
// --- Initialization. ---
// -----------------------
nxtscifunname   = SharedInfo.NextSCIFunName;
nxtscifunnumber = SharedInfo.NextSCIFunNumber;
ReportFileName  = FileInfo.Funct(nxtscifunnumber).ReportFileName;
CPass1FileName  = FileInfo.Funct(nxtscifunnumber).CPass1FileName;
SciFileFid      = FileInfo.Funct(nxtscifunnumber).SCICopyFileFid;
IndentLevel     = SharedInfo.NIndent;

PrintStepInfo('Handling EOL',ReportFileName,'file');
sciline = mgetl(SciFileFid,1);
   
// #RNU_RES_B
PrintStringInfo(' ',ReportFileName,'file','y','n');
PrintStringInfo('##################'+'################'+'##################'+'##################'+'##################',ReportFileName,'file','y','n');
PrintStringInfo('##################'+'################'+'##################'+'##################'+'##################',ReportFileName,'file','y','n');
PrintStringInfo('### Scilab code: '+sciline+' ###',ReportFileName,'file','y','n');
PrintStringInfo('##################'+'################'+'##################'+'##################'+'##################',ReportFileName,'file','y','n');
PrintStringInfo('##################'+'################'+'##################'+'##################'+'##################',ReportFileName,'file','y','n');
// #RNU_RES_E
PrintStringInfo(' ',CPass1FileName,'file','y');
// RNU BRUNO modeprintstringinfo MUST BE AN EXTERNAL PARAMETER!
modeprintstringinfo = 'both';
if (SharedInfo.CopySciCodeIntoCCode == 1)
   modeprintstringinfo = 'both';
end
PrintStringInfo(C_IndentBlanks(IndentLevel)+'/*SCI2C: #############'+'############'+'##############'+'###############'+'############',CPass1FileName,modeprintstringinfo,'y','n');
PrintStringInfo(C_IndentBlanks(IndentLevel)+'  SCI2C: '+sciline,CPass1FileName,modeprintstringinfo,'y','n');
PrintStringInfo(C_IndentBlanks(IndentLevel)+'  SCI2C: #############'+'############'+'##############'+'###############'+'############*/',CPass1FileName,modeprintstringinfo,'y','n');

endfunction

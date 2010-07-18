function SharedInfo = AST_HandleEndProgram(FileInfo,SharedInfo)
// function SharedInfo = AST_HandleEndProgram(FileInfo,SharedInfo)
// -----------------------------------------------------------------
// Handles the EndProgram tag of the AST.
//
//
// Input data:
// //NUT: add description here
//
// Output data:
// //NUT: add description here
//
// Status:
// 12-Jun-2007 -- Raffaele Nutricato: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),2,2);

// -----------------------
// --- Initialization. ---
// -----------------------
nxtscifunname      = SharedInfo.NextSCIFunName;
nxtscifunnumber    = SharedInfo.NextSCIFunNumber;
ReportFileName     = FileInfo.Funct(nxtscifunnumber).ReportFileName;
CPass1FileName     = FileInfo.Funct(nxtscifunnumber).CPass1FileName

IndentLevel        = SharedInfo.NIndent;
CCall  = '';
PrintStepInfo('Handling EndProgram',ReportFileName,'file');
tmpposfirstscalar = SharedInfo.CurrentFunInfo.PosFirstOutScalar;

if (1==2)
   //NUT: disabled because at the moment I am able to decode the return instruction.
   if (SharedInfo.CurrentFunInfo.CFunctionName == SharedInfo.CMainFunName)
      CCall = CCall+'return(0);';
   else
      if (SharedInfo.CurrentFunInfo.PosFirstOutScalar > 0)
         CCall = CCall+'return('+SharedInfo.CurrentFunInfo.OutArg(tmpposfirstscalar).Name+');'
      end
   end
   
   PrintStringInfo('   '+CCall,ReportFileName,'file','y');
   PrintStringInfo(C_IndentBlanks(IndentLevel)+CCall,CPass1FileName,'file','y');
end


SharedInfo.NIndent = SharedInfo.NIndent - 1;
IndentLevel        = SharedInfo.NIndent;
PrintStringInfo('   }',ReportFileName,'file','y');
PrintStringInfo(C_IndentBlanks(IndentLevel)+'}',CPass1FileName,'file','y');

// --- Close the copy of the scilab file. ---
PrintStringInfo('   Closing: '+FileInfo.Funct(nxtscifunnumber).SCICopyFileName,ReportFileName,'file','y');
mclose(FileInfo.Funct(nxtscifunnumber).SCICopyFileFid);

endfunction

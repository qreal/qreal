function SharedInfo = C_ForExpression(FileInfo,SharedInfo)
// function SharedInfo = C_ForExpression(FileInfo,SharedInfo)
// -----------------------------------------------------------------
// Get function for a generic SCI2C table.
//
// Input data:
// //NUT: add description here
//
// Output data:
// //NUT: add description here
//
// Status:
// 15-Nov-2007 -- Raffaele Nutricato: Author.
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
nxtscifunname    = SharedInfo.NextSCIFunName;
nxtscifunnumber  = SharedInfo.NextSCIFunNumber;

ReportFileName        = FileInfo.Funct(nxtscifunnumber).ReportFileName;
CPass1FileName        = FileInfo.Funct(nxtscifunnumber).CPass1FileName;
CPass1ForProlFileName = FileInfo.Funct(nxtscifunnumber).CPass1ForProlFileName(SharedInfo.For.Level);
CDeclarationFileName  = FileInfo.Funct(nxtscifunnumber).CDeclarationFileName;
CPass1ForEpilFileName = FileInfo.Funct(nxtscifunnumber).CPass1ForEpilFileName(SharedInfo.For.Level);

// #RNU_RES_B
PrintStringInfo(' ',ReportFileName,'file','y');
PrintStringInfo('***Generating C code***',ReportFileName,'file','y','n');
// #RNU_RES_E
CCall ='';
// ---------------------------
// --- End Initialization. ---
// ---------------------------

// ----------------------------
// --- Generate the C call. ---
// ----------------------------

// -------------------------
// --- Manage all cases. ---
// -------------------------
if (SharedInfo.ForExpr.AssignmentFun == SharedInfo.CFunId.EqScalar)
   // #RNU_RES_B
   // ---------------
   // --- Case 1. ---
   // ---------------
   PrintStringInfo('   Handling For Expression with scalar equal.',ReportFileName,'file','y');

   // for a = 10 or  for a = sin(tan(b)) where b is scalar:
   // In this case non for loops are needed.--> Do nothing!

   // -------------------------------------------------------------------------------------
   // --- Generate Prologue and Epilogue -> Copy the first N-1 lines of the for.c code. ---
   // -------------------------------------------------------------------------------------
   // #RNU_RES_E
   [C_Strings,NumCStrings] = File2StringArray(FileInfo.Funct(nxtscifunnumber).CPass1ForProlFileName(SharedInfo.For.Level));
   C_Strings = stripblanks(C_Strings);
   
   for cntstr = 1:NumCStrings
      // #RNU_RES_B
      // Prologue
      // #RNU_RES_E
      PrintStringInfo(C_IndentBlanks(SharedInfo.NIndent)+C_Strings(cntstr),CPass1FileName,'file','y');
   end
   // #RNU_RES_B
   // Epilogue
   // #RNU_RES_E
   PrintStringInfo(' ',CPass1ForEpilFileName ,'file','y');

elseif (SharedInfo.ForExpr.AssignmentFun == SharedInfo.CFunId.OpColon)
   // #RNU_RES_B
   // ---------------
   // --- Case 2. ---
   // ---------------
   // for a = 1:10
   PrintStringInfo('   Handling For Expression with OpColon.',ReportFileName,'file','y');
   // #RNU_RES_E
   
   // #RNU_RES_B
   // -------------------------------------------------------------------------------------
   // --- Generate Prologue and Epilogue -> Copy the first N-1 lines of the for.c code. ---
   // -------------------------------------------------------------------------------------
   // #RNU_RES_E
   [C_Strings,NumCStrings] = File2StringArray(FileInfo.Funct(nxtscifunnumber).CPass1ForProlFileName(SharedInfo.For.Level));
   C_Strings = stripblanks(C_Strings);
   for cntstr = 1:NumCStrings
      // #RNU_RES_B
      // Prologue
      // #RNU_RES_E
      PrintStringInfo(C_IndentBlanks(SharedInfo.NIndent)+C_Strings(cntstr),CPass1FileName,'file','y','n');
      // #RNU_RES_B
      // Epilogue
      // #RNU_RES_E
      if (length(C_Strings(cntstr)) == 0)
         C_Strings(cntstr) = ' '; // RNU for Bruno: If I don't do that I get a PrintStringInfo error related to mputstr.
         // Function not defined for given argument type(s),
         // check arguments or define function %0_mputstr for overloading.
      end
      PrintStringInfo(string(C_Strings(cntstr)),CPass1ForEpilFileName ,'file','y','n');
   end
   // #RNU_RES_B
   // ----------------------------------------
   // --- Insert "}" in the epilogue file. ---
   // ----------------------------------------
   // #RNU_RES_E
   PrintStringInfo('}',CPass1ForEpilFileName ,'file','y');
   
   // #RNU_RES_B
   // ------------------------------
   // --- Insert for expression. ---
   // ------------------------------
   // #RNU_RES_E
   CCall = 'for('+SharedInfo.ForExpr.SclValCntArg.Name+' = '+SharedInfo.ForExpr.OpColonInfoIn1+'; '...
           +SharedInfo.ForExpr.SclValCntArg.Name+' <= '+SharedInfo.ForExpr.OpColonInfoIn3+'; '...
           +SharedInfo.ForExpr.SclValCntArg.Name+' += '+SharedInfo.ForExpr.OpColonInfoIn2+')';
   PrintStringInfo(C_IndentBlanks(SharedInfo.NIndent)+CCall,CPass1FileName,'file','y');
   
   // #RNU_RES_B
   // -------------------
   // --- Insert "{". ---
   // -------------------
   // #RNU_RES_E
   CCall = '{';
   PrintStringInfo(C_IndentBlanks(SharedInfo.NIndent)+CCall,CPass1FileName,'file','y');
   
   // ---------------------------------
   // --- Update Indentation Level. ---
   // ---------------------------------
   SharedInfo.NIndent = SharedInfo.NIndent + 1;
   
elseif (SharedInfo.ForExpr.AssignmentFun == SharedInfo.CFunId.EqMatrix | ...
        SharedInfo.ForExpr.AssignmentFun == SharedInfo.CFunId.GenFunMtx)
   // #RNU_RES_B
   // ---------------
   // --- Case 3. ---
   // ---------------
   PrintStringInfo('   Handling For Expression with Matrix Equal/Function.',ReportFileName,'file','y');
   
   // for cnt = TMP where TMP is a matrix
   // for cnt = fun(TMP) where TMP is a matrix
   // Conversion is performed as shown in the following example:
   // init code for temp vars --> copy all for.c code (up to last-1 line) into C pass1 code.
   // intSCI2C __forcnt1; --> declaration file.
   // replace in the last C line CntArg.Name with '__TmpVal'+CntArg.Name
   // for (__forcnt1 = 0; __forcnt1 < CntArg.Size(1)*CntArg.Size(2); __forcnt1++)
   // {
   //    CntArg.Name = '__TmpVal'+CntArg.Name[__forcnt1];
   //    init code for temp vars
   // }

   // -------------------------------------------------------------------------------------
   // --- Generate Prologue and Epilogue -> Copy the first N-1 lines of the for.c code. ---
   // -------------------------------------------------------------------------------------
   // #RNU_RES_E
   [C_Strings,NumCStrings] = File2StringArray(FileInfo.Funct(nxtscifunnumber).CPass1ForProlFileName(SharedInfo.For.Level));
   C_Strings = stripblanks(C_Strings);
   
   for cntstr = 1:NumCStrings
      // Prologue
      PrintStringInfo(C_IndentBlanks(SharedInfo.NIndent)+C_Strings(cntstr),CPass1FileName,'file','y');
      // Epilogue
      PrintStringInfo(C_Strings(cntstr),CPass1ForEpilFileName ,'file','y');
   end
   
   // #RNU_RES_B
   // ----------------------------------------
   // --- Insert "}" in the epilogue file. ---
   // ----------------------------------------
   // #RNU_RES_E
   PrintStringInfo('}',CPass1ForEpilFileName ,'file','y');
   
   // ------------------------------
   // --- Insert for expression. ---
   // ------------------------------
   CCall = 'for('+SharedInfo.ForExpr.IntCntArg.Name+' = 0'+'; '+...
           SharedInfo.ForExpr.IntCntArg.Name+' < '+SharedInfo.ForExpr.MtxValCntArg.Size(1)+'*'+SharedInfo.ForExpr.MtxValCntArg.Size(2)+'; '+...
           SharedInfo.ForExpr.IntCntArg.Name+'++)';
   PrintStringInfo(C_IndentBlanks(SharedInfo.NIndent)+CCall,CPass1FileName,'file','y');

   // -------------------
   // --- Insert "{". ---
   // -------------------
   CCall = '{';
   PrintStringInfo(C_IndentBlanks(SharedInfo.NIndent)+CCall,CPass1FileName,'file','y');
   
   // ---------------------------------
   // --- Update Indentation Level. ---
   // ---------------------------------
   SharedInfo.NIndent = SharedInfo.NIndent + 1;
   
   // #RNU_RES_B
   // ----------------------------------------------------
   // --- Add code to read the element of CntArg.Name. ---
   // ----------------------------------------------------
   // #RNU_RES_E
   CCall = SharedInfo.ForExpr.SclValCntArg.Name+' = '+SharedInfo.ForExpr.MtxValCntArg.Name+'['+SharedInfo.ForExpr.IntCntArg.Name+'];';
   PrintStringInfo(C_IndentBlanks(SharedInfo.NIndent)+CCall,CPass1FileName,'file','y');

else 
   // ---------------
   // --- Case 5. ---
   // ---------------

   SCI2Cerror('Could not manage the for expression.');
end

endfunction

function SharedInfo = C_Funcall(FunInfo,FileInfo,SharedInfo,FlagCall)
// function SharedInfo = C_Funcall(FunInfo,FileInfo,SharedInfo,FlagCall)
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
// 27-Oct-2007 -- Raffaele Nutricato: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),4,4);

// -----------------------
// --- Initialization. ---
// -----------------------
nxtscifunname    = SharedInfo.NextSCIFunName;
nxtscifunnumber  = SharedInfo.NextSCIFunNumber;

ReportFileName       = FileInfo.Funct(nxtscifunnumber).ReportFileName;
CPass1FileName       = FileInfo.Funct(nxtscifunnumber).CPass1FileName;
CPass1FreeFileName   = FileInfo.Funct(nxtscifunnumber).CPass1FreeFileName;
HeaderFileName       = FileInfo.Funct(nxtscifunnumber).Pass1HeaderFileName;
CDeclarationFileName = FileInfo.Funct(nxtscifunnumber).CDeclarationFileName;
CInitVarsFileName    = FileInfo.Funct(nxtscifunnumber).CInitVarsFileName;
IndentLevel          = SharedInfo.NIndent;
CCall                = '';

// --- Extract Function Info. ---
FunctionName      = FunInfo.SCIFunctionName;
CFunName          = FunInfo.CFunctionName;
InArg             = FunInfo.InArg;
NInArg            = FunInfo.NInArg;
OutArg            = FunInfo.OutArg;
NOutArg           = FunInfo.NOutArg;
PosFirstOutScalar = FunInfo.PosFirstOutScalar;

// #RNU_RES_B
PrintStringInfo(' ',ReportFileName,'file','y');
PrintStringInfo('***Generating C code***',ReportFileName,'file','y');
// #RNU_RES_E
// ---------------------------
// --- End Initialization. ---
// ---------------------------


// --------------------------------------------------
// --- Manage anticipated exit from the function. ---
// --------------------------------------------------
if (SharedInfo.SkipNextFun > 0)
   SharedInfo.SkipNextFun = SharedInfo.SkipNextFun - 1;
   return;
end

// #RNU_RES_B
// Exit if the function is a precision specifier and the corresponding flag is 1.
// #RNU_RES_E
if ((sum(mtlb_strcmp(FunctionName,SharedInfo.Annotations.DataPrec)) > 0) & ...
    (SharedInfo.SkipNextPrec == 1))
   // #RNU_RES_B
   PrintStringInfo('   Skipping code generating because already generated in the previous function.',ReportFileName,'file','y');
   // #RNU_RES_E
   SharedInfo.SkipNextPrec = SharedInfo.SkipNextPrec - 1;
   return;
end

// #RNU_RES_B
// Exit if the function is OpEqual and the corresponding skip flag is enabled.
// #RNU_RES_E
if ((mtlb_strcmp(FunctionName,'OpEqual')) & ...
    (SharedInfo.SkipNextEqual == 1))
   // #RNU_RES_B
   PrintStringInfo('   Skipping code generating because already generated in the previous function.',ReportFileName,'file','y');
   // #RNU_RES_E
   SharedInfo.SkipNextEqual = SharedInfo.SkipNextEqual - 1;
   return;
end

// #BJ
// size should be managed as other functions
// otherwise size(4) will lead to a C variable __4Size reference
// wich will never exists

// #RNU_RES_B
// Exit if the function is size.
// #RNU_RES_E
// if ((mtlb_strcmp(FunctionName,'size')))
//    // #RNU_RES_B
//    PrintStringInfo('   Anticipated exit for the size function.',ReportFileName,'file','y');
//    // #RNU_RES_E
//    CCall ='';
//    if (NInArg == 1)
//       if (NOutArg == 1)
//          CCall = CCall+OutArg(1).Name+'[0] = __'+InArg(1).Name+'Size[0];';
//          // #RNU_RES_B
//          PrintStringInfo('   '+CCall,ReportFileName,'file','y');
//          // #RNU_RES_E
//          PrintStringInfo(C_IndentBlanks(IndentLevel)+CCall,CPass1FileName,'file','y');

//          CCall ='';
//          CCall = CCall+OutArg(1).Name+'[1] = __'+InArg(1).Name+'Size[1];';
//          // #RNU_RES_B
//          PrintStringInfo('   '+CCall,ReportFileName,'file','y');
//          // #RNU_RES_E
//          PrintStringInfo(C_IndentBlanks(IndentLevel)+CCall,CPass1FileName,'file','y');
//       elseif (NOutArg == 2)
//          CCall = CCall+OutArg(1).Name+' = __'+InArg(1).Name+'Size[0];';
//          // #RNU_RES_B
//          PrintStringInfo('   '+CCall,ReportFileName,'file','y');
//          // #RNU_RES_E
//          PrintStringInfo(C_IndentBlanks(IndentLevel)+CCall,CPass1FileName,'file','y');

//          CCall ='';
//          CCall = CCall+OutArg(2).Name+' = __'+InArg(1).Name+'Size[1];';
//          // #RNU_RES_B
//          PrintStringInfo('   '+CCall,ReportFileName,'file','y');
//          // #RNU_RES_E
//          PrintStringInfo(C_IndentBlanks(IndentLevel)+CCall,CPass1FileName,'file','y');
//       else
//          SCI2Cerror('Don''t know how to manage size function with number of output args different from 1 and 2.');
//       end
//    elseif (NInArg == 2)
//       if (NOutArg == 1)
//          if (InArg(2).Value == 1)
//             CCall = CCall+OutArg(1).Name+' = __'+InArg(1).Name+'Size[0];';
//             // #RNU_RES_B
//             PrintStringInfo('   '+CCall,ReportFileName,'file','y');
//             // #RNU_RES_E
//             PrintStringInfo(C_IndentBlanks(IndentLevel)+CCall,CPass1FileName,'file','y');
//          elseif (InArg(2).Value == 2)
//             CCall = CCall+OutArg(1).Name+' = __'+InArg(1).Name+'Size[1];';
//             // #RNU_RES_B
//             PrintStringInfo('   '+CCall,ReportFileName,'file','y');
//             // #RNU_RES_E
//             PrintStringInfo(C_IndentBlanks(IndentLevel)+CCall,CPass1FileName,'file','y');
//          else
//             SCI2Cerror('Not known the value of the second input arg for the size function.');
//          end
//       else
//          SCI2Cerror('Don''t know how to manage size function with number of output args different from 1.');
//       end
//    else
//       SCI2Cerror('Don''t know how to manage size function with number of input args different from 1 and 2.');
//    end
//    return;
// end
// ------------------------------------------------------
// --- End Manage anticipated exit from the function. ---
// ------------------------------------------------------

// #RNU_RES_B
// ------------------------------------------------------------
// --- Allocate memory and size array for output arguments. ---
// ------------------------------------------------------------
// #RNU_RES_E
if (FlagCall == 1)
// #RNU_RES_B
//RNU qui va tolto tutto una volta sicuri che la memallocout puo' essere fatta dentro la st_insoutarg
//   C_MemAllocOutTempVars(OutArg,NOutArg,CPass1FileName,CPass1FreeFileName,IndentLevel,ReportFileName);
// #RNU_RES_E
end

// ----------------------------
// --- Generate the C call. ---
// ----------------------------
CCall ='';
if (FunInfo.CFunctionName == SharedInfo.CMainFunName)
   if (FlagCall == 1)
      SCI2Cerror('main function called in a source code!');
   else
      CCall =CCall+'SCI2Cint ';
   end
else
   if (PosFirstOutScalar >= 1)
      if (FlagCall == 1)
         CCall = CCall+OutArg(PosFirstOutScalar).Name+' = ';
      else
         CCall = CCall+C_Type(OutArg(PosFirstOutScalar).Type)+' ';
      end
   else
      if (FlagCall == 0)
         CCall = CCall+'void ';
      end
   end
end


CCall = CCall+CFunName+'(';
// #RNU_RES_B
PrintStringInfo('   C call after output scalar args check: '+CCall,ReportFileName,'file','y');
// #RNU_RES_E
clear counterin
for counterin = 1:NInArg

   if (InArg(counterin).Type == 'g' & InArg(counterin).Scope == 'String')
      TmpInArgName = '""'+InArg(counterin).Name+'""';
   elseif (InArg(counterin).Type == 'z' & (InArg(counterin).Scope == 'Number'))
      TmpInArgName = 'DoubleComplex('+SCI2Cstring(real(InArg(counterin).Value))+','+SCI2Cstring(imag(InArg(counterin).Value))+')';
   elseif (InArg(counterin).Type == 'c' & (InArg(counterin).Scope == 'Number'))
      TmpInArgName = 'FloatComplex('+SCI2Cstring(real(InArg(counterin).Value))+','+SCI2Cstring(imag(InArg(counterin).Value))+')';
   else
      TmpInArgName = InArg(counterin).Name;
   end

   TmpInArgType = C_Type(InArg(counterin).Type);

   //if (FunctionName == 'OpEqual')
   //      TmpInArgSizeVar = '__'+OutArg(counterin).Name+'Size';
   // else
   TmpInArgSizeVar = '__'+InArg(counterin).Name+'Size';
   //end

   if (InArg(counterin).Dimension == 0)
      if (FlagCall == 0)
         CCall = CCall+TmpInArgType+' ';
      end
      CCall = CCall+TmpInArgName+',';
   else
      if (FlagCall == 0)
         CCall = CCall+TmpInArgType+'* '+TmpInArgName+', SCI2Cint* __'+TmpInArgName+'Size,';
      else
         CCall = CCall+TmpInArgName+',  '+TmpInArgSizeVar+',';
      end
   end
end
// #RNU_RES_B
PrintStringInfo('   C call after input args analysis: '+CCall,ReportFileName,'file','y');
// #RNU_RES_E
for counterout = 1:NOutArg
   TmpOutArgName = OutArg(counterout).Name;
   TmpOutArgType = C_Type(OutArg(counterout).Type);
   if (counterout == PosFirstOutScalar)
      if (FlagCall == 0)
         // #RNU_RES_B
         // --- Write in the declaration file the returned output scalar (if any). ---
         // #RNU_RES_E
         outscalardeclaration = TmpOutArgType+' '+TmpOutArgName+';';
         // #RNU_RES_B
         PrintStringInfo(outscalardeclaration,ReportFileName,'file','y');
         // #RNU_RES_E
         PrintStringInfo(C_IndentBlanks(1)+outscalardeclaration,CDeclarationFileName,'file','y');
         PrintStringInfo(' ',CDeclarationFileName,'file','y');
      end
   else
      if (OutArg(counterout).Dimension == 0)
         if (FlagCall == 0)
            // --- Write in the declaration file the returned output scalar (if any). ---
            outscalardeclaration = TmpOutArgType+' '+TmpOutArgName+';';
            PrintStringInfo(outscalardeclaration,ReportFileName,'file','y');
            PrintStringInfo(C_IndentBlanks(1)+outscalardeclaration,CDeclarationFileName,'file','y');
            PrintStringInfo(' ',CDeclarationFileName,'file','y');
            CCall = CCall+TmpOutArgType+'* __ptr'+TmpOutArgName+', ';
         else
            CCall = CCall+'&'+TmpOutArgName+', ';//NUT: verifica se ci vuole l'&
         end
      else
         if (FlagCall == 0)
            CCall = CCall+TmpOutArgType+'* '+TmpOutArgName+',';
            if (OutArg(counterout).FindLike == 1)
               CCall = CCall+'SCI2Cint* __'+TmpOutArgName+'Size'+',';
            end
            // #RNU_RES_B
            //NUT prova a sostituire le variabili strutture con variabili dichiarate all'inizio del codice.
            // --- Declare the size of the output arguments. ---
            // #RNU_RES_E
            outscalardeclaration = 'SCI2Cint __'+TmpOutArgName+'Size[2];';
            PrintStringInfo(outscalardeclaration,ReportFileName,'file','y');
            PrintStringInfo(C_IndentBlanks(1)+outscalardeclaration,CDeclarationFileName,'file','y');
            outscalardeclaration = '__'+TmpOutArgName+'Size[0] = '+(OutArg(counterout).Size(1))+';';
            PrintStringInfo(outscalardeclaration,ReportFileName,'file','y');
            PrintStringInfo(C_IndentBlanks(1)+outscalardeclaration,CInitVarsFileName,'file','y');
            outscalardeclaration = '__'+TmpOutArgName+'Size[1] = '+(OutArg(counterout).Size(2))+';';
            PrintStringInfo(outscalardeclaration,ReportFileName,'file','y');
            PrintStringInfo(C_IndentBlanks(1)+outscalardeclaration,CInitVarsFileName,'file','y');
            PrintStringInfo(' ',CInitVarsFileName,'file','y');
         else
            CCall = CCall+OutArg(counterout).Name+',';
            if (OutArg(counterout).FindLike == 1)
               CCall = CCall+'(SCI2Cint* ) __'+TmpOutArgName+'Size'+',';
            end
         end
      end
   end
end
PrintStringInfo('   C call after output args analysis: '+CCall,ReportFileName,'file','y');
// Remove the last " " and ","
if (part(CCall,length(CCall):length(CCall)) == ' ')
   CCall = part(CCall,1:length(CCall)-1);
end
if (part(CCall,length(CCall):length(CCall)) == ',')
   CCall = part(CCall,1:length(CCall)-1);
end

CCall = CCall+')';
if (FlagCall == 1)
   CCall = CCall+';';
end
//NUT: la parte di generazione della C call va inserita in una funzione a parte.
//NUT: tale funzione deve avere anche uno switch che consenta di generare differenti versioni
//NUT: delle chiamate C in accordo con la libreria disponibile, fermo restando che
//NUT:  e' sempre possibile fornire la lista delle macro.
if mtlb_strcmp(FunctionName,'return')
   // Here I introduce the pointer assignment for output scalar arguments.
   for cntout = 1:SharedInfo.CurrentFunInfo.NOutArg
      if (cntout ~= SharedInfo.CurrentFunInfo.PosFirstOutScalar & ...
          SharedInfo.CurrentFunInfo.OutArg(cntout).Dimension == 0)
         CCall = '';
         CCall = CCall+'*__ptr'+SharedInfo.CurrentFunInfo.OutArg(cntout).Name+' = '+...
            SharedInfo.CurrentFunInfo.OutArg(cntout).Name+';';
         PrintStringInfo('   '+CCall,ReportFileName,'file','y');
         PrintStringInfo(C_IndentBlanks(IndentLevel)+CCall,CPass1FileName,'file','y');
      end
   end
   // --- Then I free the memory dinamically allocated. ---
   // ----------------------------
   // --- Handle Free section. ---
   // ----------------------------
   PrintStringInfo(C_IndentBlanks(1)+'/*',CPass1FreeFileName,'file','y');
   PrintStringInfo(C_IndentBlanks(1)+'** ------------------------- ',CPass1FreeFileName,'file','y');
   PrintStringInfo(C_IndentBlanks(1)+'** --- End Free Section. --- ',CPass1FreeFileName,'file','y');
   PrintStringInfo(C_IndentBlanks(1)+'** ------------------------- ',CPass1FreeFileName,'file','y');
   PrintStringInfo(C_IndentBlanks(1)+'*/',CPass1FreeFileName,'file','y');
   PrintStringInfo(' ',CPass1FreeFileName,'file','y');
   SCI2Ccopyfile(CPass1FreeFileName,...
      CPass1FileName,'append');
   // --------------------------------
   // --- End Handle Free section. ---
   // --------------------------------

   // --- Then I introduce the return to the first scalar output arguments. ---
   CCall = '';
   // #RNU_RES_B
   //NUT: non capisco questo skip a questo punto.
   //NUT: perche' la return finale la sto gestendo nella AST_HandleEndProgram.
   PrintStringInfo('   return function of the AST is skipped.',ReportFileName,'file','y');
   //RN provo a non skippare e a mettere la return.
   // #RNU_RES_E
   if (SharedInfo.CurrentFunInfo.CFunctionName == SharedInfo.CMainFunName)
      CCall = CCall+'return(0);';
   else
      if (SharedInfo.CurrentFunInfo.PosFirstOutScalar > 0)
         CCall = CCall+'return('+SharedInfo.CurrentFunInfo.OutArg(SharedInfo.CurrentFunInfo.PosFirstOutScalar).Name+');'
      end
   end
   // #RNU_RES_B
   PrintStringInfo('   '+CCall,ReportFileName,'file','y');
   // #RNU_RES_E
   PrintStringInfo(C_IndentBlanks(IndentLevel)+CCall,CPass1FileName,'file','y');
else
   // #RNU_RES_B
   PrintStringInfo('   '+CCall,ReportFileName,'file','y');
   // #RNU_RES_E
   PrintStringInfo(C_IndentBlanks(IndentLevel)+CCall,CPass1FileName,'file','y');
   if (FlagCall == 0)
      // Add prototype to the header file

     C_InitHeader(CCall+';',HeaderFileName,SharedInfo.Sci2CLibMainHeaderFName);

      // Add { at the beginning of the function.
      PrintStringInfo('   {',ReportFileName,'file','y');
      PrintStringInfo(C_IndentBlanks(IndentLevel)+'{',CPass1FileName,'file','y');

   end
end

// #RNU_RES_B
// Add in the C code the new size of the output argument when SCI2Cresize function is called.
// #RNU_RES_E
if (FunctionName == 'SCI2Cresize')
   // #RNU_RES_B
   PrintStringInfo('   Found SCI2Cresize -> Changing the size of the output argument.',ReportFileName,'file','y');
   // #RNU_RES_E
   OutArgName = OutArg(counterout).Name;
   tmpcode = '__'+OutArgName+'Size[0]='+OutArg(counterout).Size(1)+';';
   PrintStringInfo(C_IndentBlanks(IndentLevel)+tmpcode,CPass1FileName,'file','y');
   // #RNU_RES_B
   PrintStringInfo('   '+tmpcode,ReportFileName,'file','y');
   // #RNU_RES_E
   tmpcode = '__'+OutArgName+'Size[1]='+OutArg(counterout).Size(2)+';';
   PrintStringInfo(C_IndentBlanks(IndentLevel)+tmpcode,CPass1FileName,'file','y');
   // #RNU_RES_B
   PrintStringInfo('   '+tmpcode,ReportFileName,'file','y');
   // #RNU_RES_E
end
endfunction

function [IfCondArg,NIfCondArg] = AST_ParseIfExprStruct(FileInfo,SharedInfo,ASTIfExpType)
// function [IfCondArg,NIfCondArg] = AST_ParseIfExprStruct(FileInfo,SharedInfo,ASTIfExpType)
// -----------------------------------------------------------------
//#RNU_RES_B
// Parses the IfExpression structure of the AST.
//
//   txt=['If '
//        '  Expression:' 
//        '     '+string(I.expression)
//        '  If Statements'
//        '    '+objectlist2string(I.then)]
//   for e=I.elseifs
//     txt=[txt;
// 	 '  Else If Expression'
// 	 '     '+string(e.expression)
// 	 '  Else If Statements'
//        '    '+objectlist2string(e.then)]
//   end
//   txt=[txt;
//        '  Else Statements'
//        '    '+objectlist2string(I.else)
//        'EndIf']
//
//#RNU_RES_E
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
SCI2CNInArgCheck(argn(2),3,3);

// -----------------------
// --- Initialization. ---
// -----------------------
nxtscifunname   = SharedInfo.NextSCIFunName;
nxtscifunnumber = SharedInfo.NextSCIFunNumber;
ReportFileName  = FileInfo.Funct(nxtscifunnumber).ReportFileName;
//#RNU_RES_B
PrintStringInfo('***Retrieving '+ASTIfExpType+' expression parameters from AST***',ReportFileName,'file','y');
//#RNU_RES_E
IfCondArg  = [];
NIfCondArg = 0;

global SCI2CSTACK 
global StackPosition;
global STACKDEDUG
// ---------------------------
// --- End Initialization. ---
// ---------------------------

// ------------------------------------
// --- Read if condition variables. ---
// ------------------------------------
flagendpop = 0;
IfExprField = AST_PopASTStack();
if (ASTIfExpType=='if')
   if (IfExprField=='Expression:')
      flagendpop = 1;
      // Pop Again the If tag from the AST.
      IfExprField = AST_PopASTStack();
   end
elseif (ASTIfExpType=='elseif')
   if (IfExprField=='Else If Expression')
      flagendpop = 1;
   end
else
   SCI2CerrorFile('Unknown ASTIfExpType ""'+ASTIfExpType+'"".',ReportFileName);
end

while (flagendpop == 0)
   if (IfExprField~='<EOL>')   
      if (ASTIfExpType=='if')
         if (IfExprField=='Expression:')
            flagendpop = 1;
            // Pop Again the If tag from the AST.
            IfExprField = AST_PopASTStack();
         else
            NIfCondArg = NIfCondArg + 1;
            [IfCondArg(NIfCondArg),tmpscope] = AST_ExtractNameAndScope(IfExprField);
         end
      elseif (ASTIfExpType=='elseif')
         if (IfExprField=='Else If Expression')
            flagendpop = 1;
         else
            NIfCondArg = NIfCondArg + 1;
            IfCondArg(NIfCondArg) = IfExprField;
            [IfCondArg(NIfCondArg),tmpscope] = AST_ExtractNameAndScope(IfExprField);
         end
      end
   end
   IfExprField = AST_PopASTStack();
end

//#RNU_RES_B
// -------------------------------------------
// --- Print some info in the report file. ---
// -------------------------------------------
PrintStringInfo('N '+ASTIfExpType+' Condition Arguments: '+string(NIfCondArg),ReportFileName,'file','y');
//#RNU_RES_E
for counterifcondargs = 1:NIfCondArg
   //#RNU_RES_B
   PrintStringInfo(ASTIfExpType+' Condition Argument Number '+string(counterifcondargs)+': '+IfCondArg(counterifcondargs),...
      ReportFileName,'file','y');
   //#RNU_RES_E
end

endfunction

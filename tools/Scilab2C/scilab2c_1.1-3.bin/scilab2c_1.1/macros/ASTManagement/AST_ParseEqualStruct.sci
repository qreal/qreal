function [FunctionName,InArg,NInArg,OutArg,NOutArg] = AST_ParseEqualStruct(FileInfo,SharedInfo)
// function [FunctionName,InArg,NInArg,OutArg,NOutArg] = AST_ParseEqualStruct(FileInfo,SharedInfo)
// -----------------------------------------------------------------
//#RNU_RES_B
// Parses the Equal structure of the AST.
// Structure of Equal:
//  txt=['Equal'
//       '  Expression: '
//       '    '+string(e.expression)
//       '  Lhs       : '
//       '     '+objectlist2string(e.lhs)
//       'EndEqual'
//      ]
//#RNU_RES_E
//
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

// -----------------------
// --- Initialization. ---
// -----------------------
nxtscifunname   = SharedInfo.NextSCIFunName;
nxtscifunnumber = SharedInfo.NextSCIFunNumber;
ReportFileName  = FileInfo.Funct(nxtscifunnumber).ReportFileName;

global SCI2CSTACK 
global StackPosition;
global STACKDEDUG

//#RNU_RES_B
PrintStringInfo(' ',ReportFileName,'file','y','n');
PrintStringInfo('***Reading AST***',ReportFileName,'file','y','n');
//#RNU_RES_E

// -------------------------------
// --- Read Output parameters. ---
// -------------------------------
LhsField = AST_PopASTStack();
NOutArg = 0;
OutputArgumentNames = [];
OutputArgumentScope = [];
while (LhsField ~= 'Lhs       :')
   NOutArg = NOutArg + 1;
   [OutputArgumentNames(NOutArg),OutputArgumentScope(NOutArg)] = AST_ExtractNameAndScope(LhsField);
   LhsField = AST_PopASTStack();
   if (LhsField == 'Expression:')
     SCI2Cerror('Found Expression: before Lhs');
   elseif (LhsField == 'Equal')
     SCI2Cerror('Found Equal before Lhs');
   end
end
OutputArgumentNames = SCI2Cflipud(OutputArgumentNames);
OutputArgumentScope = SCI2Cflipud(OutputArgumentScope);

// ------------------------------
// --- Read input parameters. ---
// ------------------------------
ExprField = AST_PopASTStack();
NInArg = 0;
InputArgumentNames = [];
while (ExprField ~= 'Expression:')
   NInArg = NInArg + 1;
   [InputArgumentNames(NInArg),InputArgumentScope(NInArg)] = AST_ExtractNameAndScope(ExprField);
   ExprField = AST_PopASTStack();
   if (ExprField == 'Equal')
     SCI2Cerror('Found Equal before Lhs');
   end
end
InputArgumentNames = SCI2Cflipud(InputArgumentNames);
InputArgumentScope = SCI2Cflipud(InputArgumentScope);

//#RNU_RES_B
// ------------------------------
// --- Extract function name. ---
// ------------------------------
//#RNU_RES_E
FunctionName = AST_PopASTStack();
if (FunctionName ~= 'Equal') then
   SCI2Cerror('Problems with Equal, Expected Equal tag.');
end
FunctionName = 'OpEqual';

//#RNU_RES_B
// -------------------------------------
// --- Generate the InArg structure. ---
// -------------------------------------
//#RNU_RES_E
InArg = [];
for counterinputargs = 1:NInArg
   InArg(counterinputargs).Name=InputArgumentNames(counterinputargs);
   InArg(counterinputargs).Scope=InputArgumentScope(counterinputargs);
end

//#RNU_RES_B
// -------------------------------------
// --- Generate the InArg structure. ---
// -------------------------------------
//#RNU_RES_E
OutArg = [];
for counteroutputargs = 1:NOutArg
   OutArg(counteroutputargs).Name=OutputArgumentNames(counteroutputargs);
   OutArg(counteroutputargs).Scope=OutputArgumentScope(counteroutputargs);
end

// ------------------------
// --- Print Some Info. ---
// ------------------------
//#RNU_RES_B
PrintStringInfo('Function Name: '+FunctionName,ReportFileName,'file','y','n');
PrintStringInfo('N Intput Arguments: '+string(NInArg),ReportFileName,'file','y','n');
//#RNU_RES_E
if (SharedInfo.Equal.Nins > 0)
   //#RNU_RES_B
   PrintStringInfo('N ins functions: '+string(SharedInfo.Equal.Nins),ReportFileName,'file','y');
   //#RNU_RES_E
   for counterinputargs = 1:NInArg
      //#RNU_RES_B
      PrintStringInfo('Input Argument Number '+string(counterinputargs)+': '+InArg(counterinputargs).Name,...
         ReportFileName,'file','y');
      PrintStringInfo('   Scope: '+InArg(counterinputargs).Scope,...
         ReportFileName,'file','y');
      //#RNU_RES_E
   end
   if (NInArg ~= SharedInfo.Equal.Nins)
     SCI2CerrorFile('Number of input arguments must be equal to number of ins functions.',ReportFileName);
   end
else
   //#RNU_RES_B
   PrintStringInfo('N Output Arguments: '+string(NOutArg),ReportFileName,'file','y');
   //#RNU_RES_E
   for counterinputargs = 1:NInArg
      //#RNU_RES_B
      PrintStringInfo('Input Argument Number '+string(counterinputargs)+': '+InArg(counterinputargs).Name,...
         ReportFileName,'file','y','n');
      PrintStringInfo('   Scope: '+InArg(counterinputargs).Scope,...
         ReportFileName,'file','y','n');
      //#RNU_RES_E
   end
   for counteroutputargs = 1:NOutArg
      //#RNU_RES_B
      PrintStringInfo('Output Argument Number '+string(counteroutputargs)+': '+OutArg(counteroutputargs).Name,...
         ReportFileName,'file','y','n');
      PrintStringInfo('   Scope: '+OutArg(counterinputargs).Scope,...
         ReportFileName,'file','y','n');
      //#RNU_RES_E
   end
   if (NInArg ~= NOutArg)
     SCI2CerrorFile('Number of input arguments must be equal to number of output arguments.',ReportFileName);
   end
end

endfunction

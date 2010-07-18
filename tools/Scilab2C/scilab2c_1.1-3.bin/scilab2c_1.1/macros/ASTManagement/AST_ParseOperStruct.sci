function [FunctionName,InArg,NInArg,NOutArg] = AST_ParseOperStruct(FileInfo,SharedInfo)
// function [FunctionName,InArg,NInArg,NOutArg] = AST_ParseOperStruct(FileInfo,SharedInfo)
// -----------------------------------------------------------------
//#RNU_RES_B
// Parses the Operation structure of the AST.
//
// Structure of Operation:
//overloading function for "operation" type tlist string function
//this is a node of the AST
//fields:   
//     operands: a list
//     operator: a string
//   txt=['Operation'
//        '   Operands:'
//        '      '+objectlist2string(O.operands)
//        '   Operator: '+O.operator
//        'EndOperation'
//       ]
//
//#RNU_RES_E
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
PrintStringInfo(' ',ReportFileName,'file','y');
PrintStringInfo('***Reading AST***',ReportFileName,'file','y');

global SCI2CSTACK 
global StackPosition;
global STACKDEDUG
// ---------------------------
// --- End Initialization. ---
// ---------------------------


// ------------------------------
// --- Extract function name. ---
// ------------------------------
buffstring = AST_PopASTStack();
LabelFunctName = 'Operator: ';
FunctionName = stripblanks(part(buffstring,length(LabelFunctName)+1:length(buffstring)));
// Generate the proper function name.
FunctionName  = Operator2FunName(FunctionName);

// ------------------------------
// --- Read input parameters. ---
// ------------------------------
RhsField = AST_PopASTStack();
NInArg = 0;
while (RhsField ~= 'Operands:')
   NInArg = NInArg + 1;
   [InputArgumentNames(NInArg),InputArgumentScope(NInArg)] = AST_ExtractNameAndScope(RhsField);
   RhsField = AST_PopASTStack();
   if (RhsField == 'Operation')
     SCI2Cerror('Found Operation before Rhs');
   end
end

if (stripblanks(InputArgumentNames(NInArg)) == '<empty>')
   //NUT: forse non serve per l'operation 
   NInArg = 0;
   InputArgumentNames = [];
   InputArgumentScope = [];
end
InputArgumentNames = SCI2Cflipud(InputArgumentNames);
InputArgumentScope = SCI2Cflipud(InputArgumentScope);

//#RNU_RES_B
// Remove tag "Operation" from the stack.
//#RNU_RES_E
OperationField = AST_PopASTStack();
if (OperationField ~= 'Operation') then
   SCI2Cerror('Problems with Operation, Expected Operation tag.');
end

// --------------------------------------------
// --- Extract number of output parameters. ---
// --------------------------------------------
if (FunctionName == 'ins')
   NOutArg = 0; // It is always 1. Double check it!
else
   NOutArg = 1; // It is always 1. Double check it!
end
// -------------------------------------
// --- Generate the InArg structure. ---
// -------------------------------------
InArg = [];
for counterinputargs = 1:NInArg
   InArg(counterinputargs).Name=InputArgumentNames(counterinputargs);
   InArg(counterinputargs).Scope=InputArgumentScope(counterinputargs);
end

//#RNU_RES_B
PrintStringInfo('Operation Name: '+FunctionName,ReportFileName,'file','y');
PrintStringInfo('N Intput Arguments: '+string(NInArg),ReportFileName,'file','y');
PrintStringInfo('N Output Arguments: '+string(NOutArg),ReportFileName,'file','y');
//#RNU_RES_E
for counterinputargs = 1:NInArg
   //#RNU_RES_B
   PrintStringInfo('Input Argument Number '+string(counterinputargs)+': '+InArg(counterinputargs).Name,...
      ReportFileName,'file','y');
   PrintStringInfo('   Scope: '+InArg(counterinputargs).Scope,...
      ReportFileName,'file','y');
   //#RNU_RES_E
end

endfunction

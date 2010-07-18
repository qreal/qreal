function [FunctionName,InArg,NInArg,NOutArg] = AST_ParseFuncallStruct(FileInfo,SharedInfo)
// function [FunctionName,InArg,NInArg,NOutArg] = AST_ParseFuncallStruct(FileInfo,SharedInfo)
// -----------------------------------------------------------------
//#RNU_RES_B
// Extracts Input Arguments, Output Arguments and Function Name 
// from the AST.
//
// Structure of Funcall:
// overloading function for "funcall" type tlist string function
// this is a node of the AST
// fields:     
//     rhs  : a list
//     name : string, the name of the function
//     lhsnb: number, the number of function lhs
//  txt=['Funcall  : '+F.name
//       '  #lhs   : '+string(F.lhsnb)
//       '  Rhs    : '
//       '      '+objectlist2string(F.rhs)
//       'EndFuncall'
//      ]
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
// #RNU_RES_B
PrintStringInfo('   Parsing Funcall structure',ReportFileName,'file','y');
// #RNU_RES_E
global SCI2CSTACK 
global StackPosition;
global STACKDEDUG


// ------------------------------
// --- Read input parameters. ---
// ------------------------------
RhsField = AST_PopASTStack();
NInArg = 0;
while (RhsField ~= 'Rhs    :')
   NInArg = NInArg + 1;
   [InputArgumentNames(NInArg),InputArgumentScope(NInArg)] = AST_ExtractNameAndScope(RhsField);
   RhsField = AST_PopASTStack();
   if (RhsField == '#lhs   :')
     SCI2Cerror('Found #lhs before Rhs');
   elseif (RhsField == 'Funcall  :')
     SCI2Cerror('Found Funcall before Rhs');
   end
end
if (stripblanks(InputArgumentNames(NInArg)) == '<empty>')
   NInArg = 0;
   InputArgumentNames = [];
   InputArgumentScope = [];
end
InputArgumentNames = SCI2Cflipud(InputArgumentNames);
InputArgumentScope = SCI2Cflipud(InputArgumentScope);

// --------------------------------------------
// --- Extract number of output parameters. ---
// --------------------------------------------
buffstring = AST_PopASTStack();
NOutArg = eval(stripblanks(part(buffstring,10:length(buffstring))));

// ------------------------------
// --- Extract function name. ---
// ------------------------------
buffstring = AST_PopASTStack();
FunctionName = stripblanks(part(buffstring,12:length(buffstring)));

// -------------------------------------
// --- Generate the InArg structure. ---
// -------------------------------------
InArg = [];
for counterinputargs = 1:NInArg
   if (InputArgumentNames(counterinputargs) == 'r')
      InputArgumentNames(counterinputargs) = 'rr'; //NUT: per ora cerco di risolvere cosi' il baco sulla 'r'
   end
   InArg(counterinputargs).Name=InputArgumentNames(counterinputargs);
   InArg(counterinputargs).Scope=InputArgumentScope(counterinputargs);
end

//#RNU_RES_B
PrintStringInfo('Function Name: '+FunctionName,ReportFileName,'file','y','n');
PrintStringInfo('N Intput Arguments: '+string(NInArg),ReportFileName,'file','y','n');
PrintStringInfo('N Output Arguments: '+string(NOutArg),ReportFileName,'file','y','n');
//#RNU_RES_E
for counterinputargs = 1:NInArg
   //#RNU_RES_B
   PrintStringInfo('Input Argument Number '+string(counterinputargs)+': '+InArg(counterinputargs).Name,...
      ReportFileName,'file','y','n');
   PrintStringInfo('   Scope: '+InArg(counterinputargs).Scope,...
      ReportFileName,'file','y','n');
   //#RNU_RES_E
end

endfunction

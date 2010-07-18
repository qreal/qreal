function FunName = Operator2FunName(OperatorName);
// -----------------------------------------------------------------
// Returns the function name corresponding to the input operator.
// From intmacr2tree.c we have:
//   char *operators[]={"+","-","*",".*","*.",".*.","/","./","/.","./.",
//	       "\\",".\\","\\.",".\\.","^","==","<",">","<=",">=","~=",
// 	       ":","rc","ins","ext","'","cc","|","&","~",".^",".'","cceol"};
// I also added "<>".
//
// Input data:
// //NUT: add description here
//
// Output data:
// //NUT: add description here
//
// Status:
// 29-May-2007 -- Nutricato Raffaele: Changed code into a function.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

//NUT: non e' inserito il "\" verifica come mai.
//NUT: il ".\" l'ho inserito io

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),1,1);

FuncPrefix = 'Op';
FuncSuffix = '';

if (OperatorName == '+')
   FunName = FuncPrefix+'Plus'+FuncSuffix;
elseif (OperatorName == '-')
   FunName = FuncPrefix+'Minus'+FuncSuffix;
elseif (OperatorName == '*')
   FunName = FuncPrefix+'Star'+FuncSuffix;
elseif (OperatorName == '.*')
   FunName = FuncPrefix+'DotStar'+FuncSuffix;
elseif (OperatorName == '*.')
   PrintStringInfo('SCI2CERROR: Operator ""'+OperatorName+'"" not supported.','','stout','y');
   SCI2Cerror(' ');
elseif (OperatorName == '.*.')
   PrintStringInfo('SCI2CERROR: Operator ""'+OperatorName+'"" not supported.','','stout','y');
   SCI2Cerror(' ');
elseif (OperatorName == '/')
   FunName = FuncPrefix+'Slash'+FuncSuffix;
elseif (OperatorName == './')
   FunName = FuncPrefix+'DotSlash'+FuncSuffix;
elseif (OperatorName == '/.')
   PrintStringInfo('SCI2CERROR: Operator ""'+OperatorName+'"" not supported.','','stout','y');
   SCI2Cerror(' ');
elseif (OperatorName == './.')
   PrintStringInfo('SCI2CERROR: Operator ""'+OperatorName+'"" not supported.','','stout','y');
   SCI2Cerror(' ');
elseif (OperatorName == '.\')
   FunName = FuncPrefix+'DotBackSlash'+FuncSuffix;
elseif (OperatorName == '\')
   //PrintStringInfo('SCI2CERROR: Operator ""'+OperatorName+'"" not supported.','','stout','y');
   //SCI2Cerror(' ');
   FunName = FuncPrefix+'BackSlash'+FuncSuffix;
elseif (OperatorName == '.\')
   PrintStringInfo('SCI2CERROR: Operator ""'+OperatorName+'"" not supported.','','stout','y');
   SCI2Cerror(' ');
elseif (OperatorName == '\.')
   PrintStringInfo('SCI2CERROR: Operator ""'+OperatorName+'"" not supported.','','stout','y');
   SCI2Cerror(' ');
elseif (OperatorName == '.\.')
   PrintStringInfo('SCI2CERROR: Operator ""'+OperatorName+'"" not supported.','','stout','y');
   SCI2Cerror(' ');
elseif (OperatorName == '^')
   FunName = FuncPrefix+'Hat'+FuncSuffix;
elseif (OperatorName == '.^')
   FunName = FuncPrefix+'DotHat'+FuncSuffix;
elseif (OperatorName == '''')
   FunName = FuncPrefix+'Apex'+FuncSuffix;
elseif (OperatorName == '.''')
   FunName = FuncPrefix+'DotApex'+FuncSuffix;
elseif (OperatorName == '==')
   FunName = FuncPrefix+'LogEq'+FuncSuffix;
elseif (OperatorName == '<')
   FunName = FuncPrefix+'LogLt'+FuncSuffix;
elseif (OperatorName == '>')
   FunName = FuncPrefix+'LogGt'+FuncSuffix;
elseif (OperatorName == '<=')
   FunName = FuncPrefix+'LogLe'+FuncSuffix;
elseif (OperatorName == '>=')
   FunName = FuncPrefix+'LogGe'+FuncSuffix;
elseif (OperatorName == '~=')
   FunName = FuncPrefix+'LogNe'+FuncSuffix;
elseif (OperatorName == '<>')
   FunName = FuncPrefix+'LogNe'+FuncSuffix;
elseif (OperatorName == '|')
   FunName = FuncPrefix+'LogOr'+FuncSuffix;
elseif (OperatorName == '&')
   FunName = FuncPrefix+'LogAnd'+FuncSuffix;
elseif (OperatorName == '~')
   FunName = FuncPrefix+'LogNot'+FuncSuffix;
elseif (OperatorName == ':')
   FunName = FuncPrefix+'Colon'+FuncSuffix;
elseif (OperatorName == 'rc')
   FunName = 'OpRc';
elseif (OperatorName == 'cc')
   FunName = 'OpCc';
elseif (OperatorName == 'ins')
   FunName = 'OpIns';
elseif (OperatorName == 'ext')
   FunName = 'OpExt';
elseif (OperatorName == 'cceol')
   PrintStringInfo('SCI2CERROR: Operator ""'+OperatorName+'"" not supported.','','stout','y');
   SCI2Cerror(' ');
else
   PrintStringInfo('SCI2CERROR: Unknown Operator ""'+OperatorName+'.','','stout','y');
   SCI2Cerror(' ');
end

endfunction

function AnnotationFnc = AST_CheckPrecSpecifier(FunctionName,FileInfo,SharedInfo);
// function AnnotationFnc = AST_CheckPrecSpecifier(FunctionName,FileInfo,SharedInfo);
// -----------------------------------------------------------------
// #RNU_RES_B
// Searches for one of the following data annotation functions:
//         Funcall  : int
//         Funcall  : float
//         Funcall  : double
// Note: remember to execute this function before pushing the output
// argument names into the stack.
// #RNU_RES_E
//
// Input data:
// //NUT: add description here
//
// Output data:
// //NUT: add description here
//
// Status:
// 13-Apr-2007 -- Raffaele Nutricato: Author.
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
PrintStringInfo(' ',ReportFileName,'file','y');
// #RNU_RES_B
PrintStringInfo('   Checking presence of precision specifier',ReportFileName,'file','y');
//NUT: da sistemare senza le global
// #RNU_RES_E
global SCI2CSTACK 
global StackPosition;
global STACKDEDUG

AnnotationFnc = 'default';
// ---------------------------
// --- End Initialization. ---
// ---------------------------

Pop1 = AST_PopASTStack(); //           Rhs    : 
if (mtlb_strcmp(stripblanks(Pop1),'Rhs    :'))
   Pop2 = AST_PopASTStack(); //           #lhs   : 1
   if (mtlb_strcmp(stripblanks(Pop2),'#lhs   : 1'))
      Pop3 = AST_PopASTStack(); //         Funcall  : double
      FunctionName = stripblanks(part(Pop3,12:length(Pop3)));
      for counterdataprec = 1:max(size(SharedInfo.Annotations.DataPrec))
         if (mtlb_strcmp(FunctionName,SharedInfo.Annotations.DataPrec(counterdataprec)))
            AnnotationFnc = FunctionName;
         end
      end
      // --- Repush strings into the AST stack. ---
      AST_PushASTStack(Pop3);
   end
   // --- Repush strings into the AST stack. ---
   AST_PushASTStack(Pop2);
end
// --- Repush strings into the AST stack. ---
AST_PushASTStack(Pop1);

if mtlb_strcmp(AnnotationFnc,'default')
   // #RNU_RES_B
   PrintStringInfo('Function is not annotated',ReportFileName,'file','y');
   PrintStringInfo('The ""'+SharedInfo.DefaultPrecision+'"" default precision will be used.',ReportFileName,'file','y');
   // #RNU_RES_E
else
   // #RNU_RES_B
   PrintStringInfo('Function is annotated with ""'+AnnotationFnc+'"" specifier',ReportFileName,'file','y');
   // #RNU_RES_E
end

endfunction

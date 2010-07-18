function AST_CheckCommonInOutArgs(InArg,NInArg,OutArg,NOutArg,ReportFileName)
// function AST_CheckCommonInOutArgs(InArg,NInArg,OutArg,NOutArg,ReportFileName)
// -----------------------------------------------------------------
// #RNU_RES_B
// Compares input and output arguments names and issues and error
// when at least one output argument is equal to the one of the
// input arguments. The error is issued only when the common argument
// is not a scalar value. This is a safe approach that prevents error
// when the same matrix is used as both input and output argument of
// a function. 
// #RNU_RES_E
//
// Input data:
// //NUT: add description here
//
// Output data:
// //NUT: add description here
//
// Status:
// 08-Jan-2008 -- Raffaele Nutricato: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),5,5);

ncommonstrings = 0;
commonstrings  = '';

//RNU non mi ricordo per quale motivo avevo commentato il seguente codice 
//RNU e decommentato l'altro a seguire. Sembra che avessi deciso che anche
//RNU le variabili scalari non potessero essere usate nella stessa expr
//RNU contemporaneamente come input e come output
for cnt1 = 1:NInArg
   for cnt2 = 1:NOutArg
      if ((InArg(cnt1).Name == OutArg(cnt2).Name) & ...
          (InArg(cnt1).Dimension > 0))
         ncommonstrings = ncommonstrings + 1;
         commonstrings(ncommonstrings) = InArg(cnt1).Name;
      end
   end
end

// for cnt1 = 1:NInArg
//    for cnt2 = 1:NOutArg
//       if ((InArg(cnt1).Name == OutArg(cnt2).Name))
//          ncommonstrings = ncommonstrings + 1;
//          commonstrings(ncommonstrings) = InArg(cnt1).Name;
//       end
//    end
// end

if (ncommonstrings > 0)
   PrintStringInfo(' ',ReportFileName,'both','y');
   PrintStringInfo('SCI2CERROR: Found '+string(ncommonstrings)+' input/output 2-D arguments',ReportFileName,'both','y');
   PrintStringInfo('SCI2CERROR: with the same name: ',ReportFileName,'both','y');
   for cntstr = 1:ncommonstrings
      PrintStringInfo('SCI2CERROR: Arg('+string(cntstr)+'): '+commonstrings(cntstr),ReportFileName,'both','y');
   end
   PrintStringInfo(' ',ReportFileName,'both','y');
   PrintStringInfo('SCI2CERROR: This approach is not allowed because it is not safe',ReportFileName,'both','y');
   PrintStringInfo('SCI2CERROR: due to the fact that arrays are passed by reference to functions.',ReportFileName,'both','y');
   PrintStringInfo('SCI2CERROR: For example if A is a squared matrix then the following code,',ReportFileName,'both','y');
   PrintStringInfo('SCI2CERROR: A = A'';',ReportFileName,'both','y');
   PrintStringInfo('SCI2CERROR: could generate incorrect results.',ReportFileName,'both','y');
   PrintStringInfo('SCI2CERROR: Please consider renaming input or output arguments.',ReportFileName,'both','y');
   PrintStringInfo('SCI2CERROR: See examples below:',ReportFileName,'both','y');
   PrintStringInfo('SCI2CERROR: ',ReportFileName,'both','y');
   PrintStringInfo('SCI2CERROR: // Example 1: Function call.',ReportFileName,'both','y');
   PrintStringInfo('SCI2CERROR: A = zeros(10,9);',ReportFileName,'both','y');
   PrintStringInfo('SCI2CERROR: A = sin(A);     // Not Allowed',ReportFileName,'both','y');
   PrintStringInfo('SCI2CERROR: // The previous line must be rewritten as:',ReportFileName,'both','y');
   PrintStringInfo('SCI2CERROR: MYTMP = A;      // Allowed',ReportFileName,'both','y');
   PrintStringInfo('SCI2CERROR: A = sin(MYTMP); // Allowed',ReportFileName,'both','y');
   PrintStringInfo('SCI2CERROR: ',ReportFileName,'both','y');
   PrintStringInfo('SCI2CERROR: // Example 2: Function definition.',ReportFileName,'both','y');
   PrintStringInfo('SCI2CERROR: function d = myfun(a,b,c,d) // Not Allowed',ReportFileName,'both','y');
   PrintStringInfo('SCI2CERROR: // The previous line must be rewritten as:',ReportFileName,'both','y');
   PrintStringInfo('SCI2CERROR: function e = myfun(a,b,c,d) // Not Allowed',ReportFileName,'both','y');
   PrintStringInfo(' ',ReportFileName,'both','y');
   SCI2Cerror(' ');
end

endfunction

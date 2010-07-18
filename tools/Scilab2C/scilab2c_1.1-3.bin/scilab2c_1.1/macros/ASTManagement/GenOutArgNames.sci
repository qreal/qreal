function [OutArg,SharedInfo] = GenOutArgNames(FunctionName,InArg,NInArg,OldOutArg,NOutArg,LhsArg,NLhsArg,FileInfo,SharedInfo)
// function [OutArg,SharedInfo] = GenOutArgNames(FunctionName,OutArg,NOutArg,LhsArg,NLhsArg,FileInfo,SharedInfo)
// -----------------------------------------------------------------
//#RNU_RES_B
// Generate the names for the temporary variables that store the 
// output arguments.
//#RNU_RES_E
//
// Input data:
// //NUT: add description here
//
// Output data:
// OutArg: is the Output arguments structure containing the field
//         name that specifies the output argument names.
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
SCI2CNInArgCheck(argn(2),9,9);

// -----------------------
// --- Initialization. ---
// -----------------------
nxtscifunname   = SharedInfo.NextSCIFunName;
nxtscifunnumber = SharedInfo.NextSCIFunNumber;
ReportFileName  = FileInfo.Funct(nxtscifunnumber).ReportFileName;
// #RNU_RES_B
PrintStringInfo('   Generating Out Arg names.',ReportFileName,'file','y');
// #RNU_RES_E
OutArg = OldOutArg;

//#RNU_RES_B
// ---------------------------------------------------------------------------------------
// --- Generate the names for the temporary variables that store the output arguments. ---
// ---------------------------------------------------------------------------------------
// At this step only the name of the output arguments can be generated. ---
//#RNU_RES_E
if (NLhsArg > 0)
   //#RNU_RES_B
   // Use the equal Lhs names.
   PrintStringInfo('Using Equal Lhs names.',ReportFileName,'file','y');
   //#RNU_RES_E
   if (NLhsArg ~= NOutArg)
      SCI2CerrorFile('NLhsArg='+string(NLhsArg)+' must be equal to NOutArg='+string(NOutArg)+'.',ReportFileName);
   end
   for counteroutputargs = 1:NOutArg
      OutArg(counteroutputargs).Name=LhsArg(counteroutputargs).Name;
      OutArg(counteroutputargs).Scope=LhsArg(counteroutputargs).Scope;
   end
else
   //#RNU_RES_B
   // Generate temporary variables.
   PrintStringInfo('Generating temporary variables.',ReportFileName,'file','y');
   //#RNU_RES_E
   if ((sum(mtlb_strcmp(FunctionName,SharedInfo.Annotations.DataPrec)) > 0) & ...
       (SharedInfo.SkipNextPrec == 1))
      //#RNU_RES_B
      PrintStringInfo('   Skipping code generating because already generated in the previous function.',ReportFileName,'file','y');
      //#RNU_RES_E
      for counteroutputargs = 1:NOutArg
         OutArg(counteroutputargs).Name = InArg(counteroutputargs).Name;
      end
   elseif (mtlb_strcmp(FunctionName,'OpEqual'))
      // do nothing.
      //NUT: verifica questa parte di codice. e' sicuro che se ho equal gli oldoutarg contengono gia' il nome?
   else
      for counteroutputargs = 1:NOutArg
         if ((SharedInfo.ASTReader.EnableTempVarsReuse == 1) & ...
             (length(SharedInfo.ASTReader.ReusableTempVars) > 0))
            TmpOutArgName = strcat([SharedInfo.ASTReader.TempVarsName,string(SharedInfo.ASTReader.ReusableTempVars(1))]);
            SharedInfo.ASTReader.ReusableTempVars = SharedInfo.ASTReader.ReusableTempVars(2:$);
         else
            SharedInfo.ASTReader.UsedTempVars = SharedInfo.ASTReader.UsedTempVars + 1;
            TmpOutArgName = strcat([SharedInfo.ASTReader.TempVarsName,string(SharedInfo.ASTReader.UsedTempVars)]);
         end
         OutArg(counteroutputargs).Name=TmpOutArgName;
      end
   end
end

endfunction

function [OutArg,SharedInfo] = ST_InsForCntVars(InArg,NInArg,OutArg,NOutArg,FunctionName,FileInfo,SharedInfo)
// function [OutArg,SharedInfo] = ST_InsForCntVars(InArg,NInArg,OutArg,NOutArg,FunctionName,FileInfo,SharedInfo)
// -----------------------------------------------------------------
// //NUT: add description here
//
// Input data:
// //NUT: add description here
//
// Output data:
// //NUT: add description here
//
// Status:
// 26-Oct-2007 -- Raffaele Nutricato: Author.
// 26-Oct-2007 -- Alberto Morea: Test Ok.
//
// Copyright 2007 Raffaele Nutricato & Alberto Morea.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------


// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),7,7);

// -----------------------
// --- Initialization. ---
// -----------------------
nxtscifunname        = SharedInfo.NextSCIFunName;
nxtscifunnumber      = SharedInfo.NextSCIFunNumber;
ReportFileName       = FileInfo.Funct(nxtscifunnumber).ReportFileName;

// #RNU_RES_B
PrintStringInfo(' ',ReportFileName,'file','y');
PrintStringInfo('***Checking if the current function is handling for counter variables.***',ReportFileName,'file','y');
// #RNU_RES_E

// ---------------------------
// --- End Initialization. ---
// ---------------------------

// -----------------------------------------------
// --- Initial Check on For counter variables. ---
// -----------------------------------------------
if ((SharedInfo.ForExpr.OnExec > 0) & (NOutArg==1) & (OutArg.Scope~='Temp'))
   // #RNU_RES_B
   // If we are here, for sure we are in the last assignment of a for loop expression.
   //
   // All the following cases must be handled:
   // OpColon (1,10,cnt) or Opcolon (1,1,10,cnt) --> cnt must be redefined to dim=0
   // cnt = a; where a is scalar
   // OpEqual(TMP,cnt); where TMP is matrix --> cnt must be redefined to dim=0, a SCI2Cint counter must be generated
   // Fun(TMP,cnt); where TMP is matrix.--> cnt must be redefined to dim=0, a SCI2Cint counter must be generated, CNT must be generated where CNT is a Matrix
   // #RNU_RES_E
   if (FunctionName == 'OpColon')
      // #RNU_RES_B
      PrintStringInfo('   The for expression is using an OpColon-based assignment',ReportFileName,'file','y');
      // #RNU_RES_E
      SharedInfo.SkipNextFun = 1;
      
      OutArg.Size(1)   = '1';
      OutArg.Size(2)   = '1';
      OutArg.Value     = %nan;
      OutArg.FindLike  = 0;
      OutArg.Dimension = 0;
      SharedInfo.ForExpr.OpColonInfoIn1 = InArg(1).Name;

      if (NInArg == 2)
         SharedInfo.ForExpr.OpColonInfoIn2 = '1';
         SharedInfo.ForExpr.OpColonInfoIn3 = InArg(2).Name;
      else 
         SharedInfo.ForExpr.OpColonInfoIn2 = InArg(2).Name;
         if isnan(InArg(2).Value)
            EM_UnknownStep(ReportFileName);
         end
         
         SharedInfo.ForExpr.OpColonInfoIn3 = InArg(3).Name;
      end        
      
      // #RNU_RES_B
      // Generate all info to generate the C for expression
      // #RNU_RES_E
      SharedInfo.ForExpr.AssignmentFun = SharedInfo.CFunId.OpColon;
      SharedInfo.ForExpr.IntCntArg    = [];
      SharedInfo.ForExpr.MtxValCntArg = [];
      SharedInfo.ForExpr.SclValCntArg = OutArg;

   elseif ((FunctionName == 'OpEqual') & (SharedInfo.ForExpr.AssignmentFun == 0))
      // #RNU_RES_B
      //NUT: Test also that SharedInfo.ForExpr.AssignmentFun because sometimes Equal are dummy!
      //NUT: verifica se e' giusta questa mia affermazione.
      // #RNU_RES_E
      if (OutArg.Dimension > 0)
         // #RNU_RES_B
         PrintStringInfo('   The for expression is using a Matrix-Equal-based assignment',ReportFileName,'file','y');
         // #RNU_RES_E
         SharedInfo.SkipNextFun = 1; //NUT: forse qui sarebbe meglio mettere uno skipnextequal per precisione.
         // #RNU_RES_B
         //NUT: a dire il vero occorre capire se c'e' differenza tra i vari skip.
         // #RNU_RES_E
         OutArg.Size(1)   = '1';
         OutArg.Size(2)   = '1';
         OutArg.Value     = %nan;
         OutArg.FindLike  = 0;
         OutArg.Dimension = 0;
         
         // #RNU_RES_B
         // Introduce the int counter variable.
         // #RNU_RES_E
         NNewArg = 1;
         NewArg(NNewArg).Name      = SharedInfo.ASTReader.TempForCntVarsName+string(SharedInfo.CountForTempVars);
         SharedInfo.CountForTempVars = SharedInfo.CountForTempVars + 1;
         NewArg(NNewArg).Size(1)   = '1';
         NewArg(NNewArg).Size(2)   = '1';
         NewArg(NNewArg).Type      = 'i';
         NewArg(NNewArg).Value     = 0;
         NewArg(NNewArg).FindLike  = 0;
         NewArg(NNewArg).Dimension = 0;         
         NewArg(NNewArg).Scope     = 'Temp';         
         
         // #RNU_RES_B
         // Insert New Arguments in the symbol table.
         // #RNU_RES_E
         NNewArg = 1;
         ST_InsOutArg(NewArg,NNewArg,FileInfo,SharedInfo,'all');
         
         // #RNU_RES_B
         // Generate all info to generate the C for expression
         // #RNU_RES_E
         SharedInfo.ForExpr.AssignmentFun = SharedInfo.CFunId.EqMatrix;
         SharedInfo.ForExpr.IntCntArg    = NewArg(1);
         SharedInfo.ForExpr.MtxValCntArg = InArg(1);
         SharedInfo.ForExpr.SclValCntArg = OutArg;
      else
         // #RNU_RES_B
         PrintStringInfo('   The for expression is using a Scalar-Equal-based assignment',ReportFileName,'file','y');
         // #RNU_RES_E
         SharedInfo.ForExpr.AssignmentFun = SharedInfo.CFunId.EqScalar;
      end
   else
      if (OutArg.Dimension > 0)
         // #RNU_RES_B
         PrintStringInfo('   The for expression is using a Matrix-Function-based assignment',ReportFileName,'file','y');

         // Introduce the val counter variable.
         // #RNU_RES_E
         NewArg = OutArg;
         OutArg.Name = SharedInfo.ASTReader.TempForValVarsName+OutArg.Name;

         // #RNU_RES_B
         // Set the counter variable to scalar.
         // #RNU_RES_E
         NNewArg = 1;
         NewArg(NNewArg).Size(1)   = '1';
         NewArg(NNewArg).Size(2)   = '1';
         NewArg(NNewArg).Value     = %nan;
         NewArg(NNewArg).FindLike  = 0;
         NewArg(NNewArg).Dimension = 0;      
         
         // #RNU_RES_B
         // Introduce the int counter variable.
         // #RNU_RES_E
         NNewArg = 2;
         NewArg(NNewArg).Name      = SharedInfo.ASTReader.TempForCntVarsName+string(SharedInfo.CountForTempVars);
         SharedInfo.CountForTempVars = SharedInfo.CountForTempVars + 1;
         NewArg(NNewArg).Size(1)   = '1';
         NewArg(NNewArg).Size(2)   = '1';
         NewArg(NNewArg).Type      = 'i';
         NewArg(NNewArg).Value     = 0;
         NewArg(NNewArg).FindLike  = 0;
         NewArg(NNewArg).Dimension = 0;         
         NewArg(NNewArg).Scope     = 'Temp';         
         
         // #RNU_RES_B
         // Insert New Arguments in the symbol table.         
         // #RNU_RES_E
         NNewArg  = 2;
         ST_InsOutArg(NewArg,NNewArg,FileInfo,SharedInfo,'all');
         
         // #RNU_RES_B
         // Generate all info to generate the C for expression
         // #RNU_RES_E
         SharedInfo.ForExpr.AssignmentFun = SharedInfo.CFunId.GenFunMtx;
         SharedInfo.ForExpr.IntCntArg    = NewArg(2);
         SharedInfo.ForExpr.MtxValCntArg = OutArg(1);
         SharedInfo.ForExpr.SclValCntArg = NewArg(1);
      end
   end   
end

endfunction

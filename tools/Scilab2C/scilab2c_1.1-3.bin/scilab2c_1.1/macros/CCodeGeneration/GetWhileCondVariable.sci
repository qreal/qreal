function SharedInfo = GetWhileCondVariable(OutArg,NOutArg,FunctionName,FileInfo,SharedInfo)
// function SharedInfo = GetWhileCondVariable(OutArg,NOutArg,FunctionName,FileInfo,SharedInfo)
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
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

//NUT: secondo me questa funzione non serve a nulla

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),5,5);

// -----------------------
// --- Initialization. ---
// -----------------------
nxtscifunname        = SharedInfo.NextSCIFunName;
nxtscifunnumber      = SharedInfo.NextSCIFunNumber;
ReportFileName       = FileInfo.Funct(nxtscifunnumber).ReportFileName;

// #RNU_RES_B
PrintStringInfo(' ',ReportFileName,'file','y');
PrintStringInfo('***Checking if the current function is handling while counter variables.***',ReportFileName,'file','y');
// #RNU_RES_E

// ---------------------------
// --- End Initialization. ---
// ---------------------------

// #RNU_RES_B
// -----------------------------------------------
// --- Initial Check on While counter variables. ---
// -----------------------------------------------
// #RNU_RES_E
if ((SharedInfo.WhileExpr.OnExec > 0) & (NOutArg==1))
   // #RNU_RES_B
   //NUT: se sono in una while expression devo memorizzarmi l'ultima variabile di output 
   //NUT: perche' e' quella che contiene la condizione da testare,
   //NUT: allora io me le salvo tutte e l'ultima salvata sara' quella che andra' a finire 
   //NUT: nella while.
   // #RNU_RES_E
   SharedInfo.WhileExpr.CondVar = OutArg(1).Name;
   // #RNU_RES_B
   //if (SharedInfo.WhileExpr.AssignmentFun == 0)
      //NUT: Test also that SharedInfo.WhileExpr.AssignmentFun because sometimes Equal are dummy!
      //NUT: verifica se e' giusta questa mia affermazione.
      //RNU il seguente test e' stato spostato nella AST_HandleWhileStatem.c perche'
      //RNU: secondo me la matrice finale non e' supportata dalla while, ma while(det(M)>0)
      //RNU: puo' essere benissimo supportato.
      // if (OutArg.Dimension > 0)
      //    SCI2CerrorFile('Cannot manage while with matrix conditions',ReportFileName);
      //    SharedInfo.SkipNextFun = 0; //NUT verifica se serve
      // end
   // #RNU_RES_E
      SharedInfo.WhileExpr.DimCondVar = OutArg(1).Dimension;
   ///end
end

endfunction

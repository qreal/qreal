function C_MemAllocOutTempVars(OutArg,NOutArg,CPass1FileName,CPass1FreeFileName,IndentLevel,ReportFileName,ResizeApproach)
// function C_MemAllocOutTempVars(OutArg,NOutArg,CPass1FileName,CPass1FreeFileName,IndentLevel,ReportFileName,ResizeApproach)
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
// 27-Oct-2007 -- Raffaele Nutricato: Author.
// 10-Jun-2008 -- Raffaele Nutricato: replaced malloc with realloc.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),7,7);

// #RNU_RES_B
PrintStringInfo(' ',ReportFileName,'file','y');
PrintStringInfo('***Allocating memory for temp variables***',ReportFileName,'file','y');
// #RNU_RES_E

// #RNU_RES_B
// --- Allocate memory and size array for output arguments. ---
// #RNU_RES_E
for counterout = 1:NOutArg
   if (OutArg(counterout).Dimension > 0)
      // #RNU_RES_B
      // if ((OutArg(counterout).Scope == 'Temp') | (OutArg(counterout).FindLike == -1) | ...
      //     (SCI2Cisnum(OutArg(counterout).Size(1))==%F) | (SCI2Cisnum(OutArg(counterout).Size(2))==%F))
      //NUT: qui forse ci vuole un check per verificare se per caso la variabile e' globale e non se ne conosce la size numerica.
      //NUT infatti. Per ora se la size numerica assumo che la variabile globale e' da reallocare. Secondo me occorre aggiungere
      //NUT un campo negli argomenti che specifichi la presenza di realloc da fare.
      //NUT: ho tolto il check sulle temp perche' se una temp ha size numerica non voglio fare malloc.
      //RNU sulle stringhe ancora non applico realloc
      // #RNU_RES_E
      if ((OutArg(counterout).FindLike == -1) | ...
          (SCI2Cisnum(OutArg(counterout).Size(1))==%F) | (SCI2Cisnum(OutArg(counterout).Size(2))==%F)| ...
          (ResizeApproach=='REALLOC_ALL_RESIZE_ALL' & OutArg(counterout).Type ~= 'g'))
         OutArgName = OutArg(counterout).Name;
         tmpcode = '__'+OutArgName+'Size[0]='+OutArg(counterout).Size(1)+';';
         PrintStringInfo(C_IndentBlanks(IndentLevel)+tmpcode,CPass1FileName,'file','y');
         PrintStringInfo('   '+tmpcode,ReportFileName,'file','y');
         tmpcode = '__'+OutArgName+'Size[1]='+OutArg(counterout).Size(2)+';';
         PrintStringInfo(C_IndentBlanks(IndentLevel)+tmpcode,CPass1FileName,'file','y');
         PrintStringInfo('   '+tmpcode,ReportFileName,'file','y');
         //a->val = (double *) malloc(nnz * sizeof(double));
         // numbers = (int*) realloc (numbers, count * sizeof(int));
         tmpcode = OutArgName+' = ('+C_Type(OutArg(counterout).Type)+'*) realloc('+OutArgName+',('+OutArg(counterout).Size(1)+')*('+OutArg(counterout).Size(2)+')*sizeof('+C_Type(OutArg(counterout).Type)+'));';
         PrintStringInfo(C_IndentBlanks(IndentLevel)+tmpcode,CPass1FileName,'file','y');
         PrintStringInfo('   '+tmpcode,ReportFileName,'file','y');
         PrintStringInfo(C_IndentBlanks(1)+'free('+OutArgName+');',CPass1FreeFileName,'file','y');
      end
   end
end

endfunction

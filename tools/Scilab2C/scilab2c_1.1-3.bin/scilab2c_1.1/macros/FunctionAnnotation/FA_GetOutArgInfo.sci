function UpdatedOutArg = ...
   FA_GetOutArgInfo(InArg,NInArg,OutArg,NOutArg,SharedInfo,FunPrecSpecifier,FunTypeAnnot,FunSizeAnnot,ReportFileName)
// function UpdatedOutArg = ...
//    FA_GetOutArgInfo(InArg,NInArg,OutArg,NOutArg,SharedInfo,FunPrecSpecifier,FunTypeAnnot,FunSizeAnnot,ReportFileName)
// -----------------------------------------------------------------
// #RNU_RES_B
// InArg is used by eval don't remove it from the function call.
//
// #RNU_RES_E
// Input data:
// //NUT: Add description here
//
// Output data:
// //NUT: Add description here
//
// Status:
// 25-Oct-2007 -- Raffaele Nutricato: Author.
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
UpdatedOutArg   = OutArg;
for cntin = 1:NInArg
   IN(cntin).TP    = InArg(cntin).Type;
   IN(cntin).SZ(1) = InArg(cntin).Size(1);
   IN(cntin).SZ(2) = InArg(cntin).Size(2);
   if ((isnan(InArg(cntin).Value)) & (GetSymbolDimension(InArg(cntin).Size) == 0))
      // #RNU_RES_B
      // IN(cntin).VAL = '__SCI2CNANSIZE'; //RNU: toglimi
      //RNU: Replace the value of the variable with its name, in case it is a scalar variable.
      // #RNU_RES_E
      IN(cntin).VAL = InArg(cntin).Name; 
   else
      IN(cntin).VAL = string(InArg(cntin).Value);
   end
end
DefaultPrecision = SharedInfo.DefaultPrecision;

// ---------------------------
// --- End Initialization. ---
// ---------------------------
if (mtlb_strcmp(FunTypeAnnot(1),''))
   NOutArg = 0;
else
   NOutArg = max(size(FunTypeAnnot));
end

flagfindlike = 0;
for counterin = 1:NInArg
   if (InArg(counterin).FindLike == 1 | InArg(counterin).FindLike == -1)
      // #RNU_RES_B
      // Then we must assume that the output will be findlike 
      // 0 = no find-like
      // 1 = pure find-like
      //-1 = similar to find-like (out=fun(in)) where in is find-like.
      // #RNU_RES_E
      flagfindlike = -1;
   end
end

for counterout = 1:NOutArg
   if(mtlb_strcmp(FunTypeAnnot,'FA_TP_USER'))
      UpdatedOutArg(counterout).Type   = FA_TP_USER(FunPrecSpecifier,DefaultPrecision);
   else
      UpdatedOutArg(counterout).Type   = eval(FunTypeAnnot(counterout));
   end
   UpdatedOutArg(counterout).FindLike  = 0;   
   lengthFA_SZ_RTMAX = length('FA_SZ_RTMAX');
   
   if (SCI2Cstrncmps1size('FA_SZ_RTMAX',FunSizeAnnot(counterout,1)))
      UpdatedOutArg(counterout).FindLike = 1;
      FunSizeAnnot(counterout,1) = part(FunSizeAnnot(counterout,1),lengthFA_SZ_RTMAX+1:length(FunSizeAnnot(counterout,1)));
   end
   
   if (SCI2Cstrncmps1size('FA_SZ_RTMAX',FunSizeAnnot(counterout,2)))
      UpdatedOutArg(counterout).FindLike = 1;
      FunSizeAnnot(counterout,2) = part(FunSizeAnnot(counterout,2),lengthFA_SZ_RTMAX+1:length(FunSizeAnnot(counterout,2)));
   end

   if (flagfindlike == -1)
      UpdatedOutArg(counterout).FindLike = -1;
   end
   
   // #RNU_RES_B
   // When the size is given by e IN(x).VAL  annotation we can have two cases:
   // IN(x).VAL is a number or IN(x).VAL is %nan. When it is %nan the 
   // size is equal to the name of IN(x).
   // This is a dynamic memory extension of a local variable and for the moment
   // we issue an error according to SCI2C specifications
   // #RNU_RES_E
   tmpeval = eval(FunSizeAnnot(counterout,1));
   if (IsNanSize(tmpeval))
      if SharedInfo.ForExpr.OnExec == 0
         EM_NanSize(ReportFileName);
      else
         UpdatedOutArg(counterout).Size(1) = string(tmpeval);
      end
   elseif(SCI2Cisnum(tmpeval))
      if (eval(tmpeval) <= 0)
         EM_ZeroSize(ReportFileName);
      else
         UpdatedOutArg(counterout).Size(1) = string(tmpeval);
      end
   else
      UpdatedOutArg(counterout).Size(1) = string(tmpeval);
   end
   
   tmpeval = eval(FunSizeAnnot(counterout,2));
   if (IsNanSize(tmpeval))
      if SharedInfo.ForExpr.OnExec == 0
         EM_NanSize(ReportFileName);
      else
         // #RNU_RES_B
         // If we are in for expression I prefer to issue the error later.
         // #RNU_RES_E
         UpdatedOutArg(counterout).Size(2) = string(tmpeval);
      end
   elseif(SCI2Cisnum(tmpeval))
      if (eval(tmpeval) <= 0)
         EM_ZeroSize(ReportFileName);
      else
         UpdatedOutArg(counterout).Size(2) = string(tmpeval);
      end
   else
      UpdatedOutArg(counterout).Size(2) = string(tmpeval);
   end
   
   UpdatedOutArg(counterout).Value     = %nan;
   UpdatedOutArg(counterout).Dimension = GetSymbolDimension(UpdatedOutArg(counterout).Size);
   UpdatedOutArg(counterout).Scope     = 'Temp';//NUT anche su questo si puo' ragionare verifica anche la handleoperation.
end

endfunction

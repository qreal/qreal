function GenCFunDatFiles(FunctionName,FunPrecSpecifier,FunTypeAnnot,FunSizeAnnot,InArg,NInArg,OutArg,NOutArg,CFunName,LibTypeInfo,FunInfoDatDir)
// function GenCFunDatFiles(FunctionName,FunPrecSpecifier,FunTypeAnnot,FunSizeAnnot,InArg,NInArg,OutArg,NOutArg,CFunName,LibTypeInfo,FunInfoDatDir)
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
// 30-Oct-2007 -- Raffaele Nutricato: Author.
// 30-Oct-2007 -- Alberto Morea: Test Ok.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// #RNU_RES_B
//NUT Nella fun info posso mettere le size simboliche per out arg e non quelle numeriche
//NUT che non usero' mai, anche perche' se un giorno decidero' di cambiare approccio e usero' funzioni
//NUT differenti per size differenti allora dovro' cambiare anche il loro nome per distinguerle
//NUT e di conseguenza avro' funinfo differenti.
// #RNU_RES_E
// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),11,11);


// -----------------------
// --- Initialization. ---
// -----------------------
// ---------------------------
// --- End Initialization. ---
// ---------------------------

// #RNU_RES_B
// ----------------------------------------------------------
// --- Find Position of the first output scalar argument. ---
// ----------------------------------------------------------
// #RNU_RES_E
PosFirstOutScalar = 0;
FoundOutScalar = 0;
for counterout = 1:NOutArg
   if (OutArg(counterout).Dimension == 0)
      if (FoundOutScalar==0)
         PosFirstOutScalar = counterout;
         FoundOutScalar = 1;
      end
   end
end

// ------------------------------------
// --- Update C function dat files. ---
// ------------------------------------
clear FunInfo
FunInfo.SCIFunctionName     = FunctionName;
FunInfo.CFunctionName       = CFunName;
FunInfo.FunPrecSpecifier    = FunPrecSpecifier;
FunInfo.FunTypeAnnot        = FunTypeAnnot;
FunInfo.FunSizeAnnot        = FunSizeAnnot;
FunInfo.InArg               = InArg;
FunInfo.NInArg              = NInArg;
FunInfo.OutArg              = OutArg;
FunInfo.NOutArg             = NOutArg;
FunInfo.PosFirstOutScalar   = PosFirstOutScalar;
FunInfo.LibTypeInfo         = LibTypeInfo;
save(fullfile(FunInfoDatDir,CFunName+'.dat'),FunInfo);
clear FunInfo

endfunction

function flagexist = FL_ExistCFunction(CFunName,USER2CAvailableCDat,SCI2CAvailableCDat,ConvertedDat,ToBeConvertedDat,ReportFileName)
// function flagexist = FL_ExistCFunction(CFunName,USER2CAvailableCDat,SCI2CAvailableCDat,ConvertedDat,ToBeConvertedDat,ReportFileName)
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
// Copyright 2007 Raffaele Nutricato & Alberto Morea.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),6,6);

// -----------------------
// --- Initialization. ---
// -----------------------
flagexist = %F;
// ---------------------------
// --- End Initialization. ---
// ---------------------------


AvailableDat = USER2CAvailableCDat;
load(AvailableDat,'Available');
NAvail = size(Available,1);

tmpcnt = 1;
while ((tmpcnt <=NAvail) & (flagexist == %F))
   if mtlb_strcmp(Available(tmpcnt),CFunName)
      flagexist = %T;
      // #RNU_RES_B
      PrintStringInfo('   Found C Function Name in : '+AvailableDat,ReportFileName,'file','y');
      // #RNU_RES_E
   end
   tmpcnt = tmpcnt + 1;
end
clear Available

if (flagexist == %F)
   AvailableDat = SCI2CAvailableCDat;
   load(AvailableDat,'Available');
   NAvail = size(Available,1);
   
   tmpcnt = 1;
   while ((tmpcnt <=NAvail) & (flagexist == %F))
      if mtlb_strcmp(Available(tmpcnt),CFunName)
         flagexist = %T;
         // #RNU_RES_B
         PrintStringInfo('   Found C Function Name in : '+AvailableDat,ReportFileName,'file','y');
         // #RNU_RES_E
      end
      tmpcnt = tmpcnt + 1;
   end
   clear Available
end

if (flagexist == %F)
   load(ConvertedDat,'Converted');
   NConv = size(Converted,1);
   tmpcnt = 1;
   while ((tmpcnt <=NConv) & (flagexist == %F))
      if mtlb_strcmp(Converted(tmpcnt),CFunName)
         flagexist = %T;
         // #RNU_RES_B
         PrintStringInfo('   Found C Function Name in : '+ConvertedDat,ReportFileName,'file','y');
         // #RNU_RES_E
      end
      tmpcnt = tmpcnt + 1;
   end
   clear Converted
end

if (flagexist == %F)
   load(ToBeConvertedDat,'ToBeConverted');
   NToBeConv = size(ToBeConverted,1);
   tmpcnt = 1;
   while ((tmpcnt <=NToBeConv) & (flagexist == %F))
      if mtlb_strcmp(ToBeConverted(tmpcnt).CFunctionName,CFunName)
         flagexist = %T;
         // #RNU_RES_B
         PrintStringInfo('   Found C Function Name in : '+ToBeConvertedDat,ReportFileName,'file','y');
         // #RNU_RES_E
      end
      tmpcnt = tmpcnt + 1;
   end
   clear ToBeConverted
end

if (flagexist == %F)
   // #RNU_RES_B
   PrintStringInfo('   C Function Name not found in the ""Available"" , ""Converted"" and ""ToBeConverted"" function lists.',ReportFileName,'file','y');
   // #RNU_RES_E
end

endfunction

function [UpdatedInArg,SharedInfo] = ST_GetInArgInfo(InArg,NInArg,FileInfo,SharedInfo)
// function UpdatedInArg = ST_GetInArgInfo(InArg,NInArg,FileInfo,SharedInfo)
// -----------------------------------------------------------------
// #RNU_RES_B
// Generate all the info concerning the input arguments.
//
// Input data:
// InArg: InArg structure containing only the names of the input
//        arguments.
// //NUT: add description here
//
// Output data:
// UpdatedInArg: InArg structure with all the info on the input
//               arguments.
// FileInfoDatFile: name of the .dat file containing the FileInfo 
//                  structure.
// #RNU_RES_E
//
// Status:
// 26-Oct-2007 -- Raffaele Nutricato: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),4,4);

// -----------------------
// --- Initialization. ---
// -----------------------
nxtscifunname   = SharedInfo.NextSCIFunName;
nxtscifunnumber = SharedInfo.NextSCIFunNumber;
ReportFileName  = FileInfo.Funct(nxtscifunnumber).ReportFileName;

// #RNU_RES_B
PrintStringInfo(' ',ReportFileName,'file','y');
PrintStringInfo('***Analyzing Input Arguments***',ReportFileName,'file','y');
// #RNU_RES_E

UpdatedInArg = InArg;
// ---------------------------
// --- End Initialization. ---
// ---------------------------

for cntinarg = 1:NInArg
   tmpname = InArg(cntinarg).Name; 
   tmpscope = InArg(cntinarg).Scope; 
   lengthNumber = length('Number_');
   if (part(tmpscope,1:lengthNumber) == 'Number_')
      // #RNU_RES_B
      PrintStringInfo('Input Argument '+string(cntinarg)+' is a number: '+tmpname+'.',FileInfo.Funct(nxtscifunnumber).ReportFileName,'file');
      // #RNU_RES_E
      UpdatedInArg(cntinarg).Type = part(tmpscope,lengthNumber+1:lengthNumber+1);
      if (UpdatedInArg(cntinarg).Type == 'x')
         UpdatedInArg(cntinarg).Type = SharedInfo.DefaultPrecision; // It is the default.
      elseif (UpdatedInArg(cntinarg).Type == 'X')
         if (SharedInfo.DefaultPrecision == 's')
            UpdatedInArg(cntinarg).Type = 'c'; // It is the default.
         elseif (SharedInfo.DefaultPrecision == 'd')
            UpdatedInArg(cntinarg).Type = 'z'; // It is the default.
         else
            SCI2Cerror('Unexpected value for SharedInfo.DefaultPrecision: ""'+SharedInfo.DefaultPrecision+'""');
         end            
      end
      if (tmpname == '%pi')
         UpdatedInArg(cntinarg).Name = 'SCI2C_PI'; 
         numvalue = %pi;
      elseif (tmpname == '%e')
         UpdatedInArg(cntinarg).Name = 'SCI2C_E'; 
         numvalue = %e;
      elseif (tmpname == '%T' | tmpname == '%t')
         UpdatedInArg(cntinarg).Name = 'SCI2C_T'; 
         numvalue = 1;
      elseif (tmpname == '%F' | tmpname == '%f')
         UpdatedInArg(cntinarg).Name = 'SCI2C_F'; 
         numvalue = 0;
      elseif (tmpname == '%nan')
         UpdatedInArg(cntinarg).Name = 'SCI2C_NAN'; 
         numvalue = %nan;
      elseif (tmpname == '%inf')
         UpdatedInArg(cntinarg).Name = 'SCI2C_INF'; 
         numvalue = %nan; // Otherwise it will put in the C code Inf value.
      elseif (tmpname == '%i')
         // #RNU_RES_B
         //NUT: Other complex numbers are always
         //NUT: stored in variables, and don't appear as immediate numbers.
         // #RNU_RES_E
         UpdatedInArg(cntinarg).Name = 'SCI2C_IMG_'+convstr(UpdatedInArg(cntinarg).Type,'u'); 
         numvalue = %i;
      else
         numvalue = eval(tmpname);
         //UpdatedInArg(cntinarg).Type = 'd'; // it is the default.
      end

      UpdatedInArg(cntinarg).Size(1)   = '1'; 
      UpdatedInArg(cntinarg).Size(2)   = '1'; 
      UpdatedInArg(cntinarg).Value     = numvalue;
      UpdatedInArg(cntinarg).FindLike  = 0;
      UpdatedInArg(cntinarg).Dimension = 0;
      UpdatedInArg(cntinarg).Scope     = 'Number';
      
   elseif (tmpscope == 'String')
      // #RNU_RES_B
      PrintStringInfo('Input Argument '+string(cntinarg)+' is a string: '+tmpname+'.',FileInfo.Funct(nxtscifunnumber).ReportFileName,'file');
      // #RNU_RES_E
      SharedInfo.ASTReader.UsedTempVars = SharedInfo.ASTReader.UsedTempVars + 1;
      TmpOutArgName = strcat([SharedInfo.ASTReader.TempVarsName,string(SharedInfo.ASTReader.UsedTempVars)]);
      
      UpdatedInArg(cntinarg).Name      = TmpOutArgName; // Change the name.
      UpdatedInArg(cntinarg).Type      = 'g'; // it is the default.
      UpdatedInArg(cntinarg).Size(1)   = '1'; 
      UpdatedInArg(cntinarg).Size(2)   = string(length(tmpname)+1); //+1 = (\0)
      UpdatedInArg(cntinarg).Value     = '""'+tmpname+'""';
      UpdatedInArg(cntinarg).FindLike  = 0;
      UpdatedInArg(cntinarg).Dimension = 2; //NUT: in future releases you can set this field to 1.
      UpdatedInArg(cntinarg).Scope     = 'Temp';
      
      // #RNU_RES_B
      // Add the new symbol in the symbol table.
      // #RNU_RES_E
      ST_InsOutArg(UpdatedInArg(cntinarg),1,FileInfo,SharedInfo,'all');

   elseif (tmpscope == 'Variable' | tmpscope == 'Global' | tmpscope == 'Local' | tmpscope == 'Temp')
      // #RNU_RES_B
      //NUT: nelle future versioni qui si puo' fare una utile differenziazione per rendere piu' intelligente il tutto.
      PrintStringInfo('Input Argument '+string(cntinarg)+' is a symbol: '+tmpname+'.',FileInfo.Funct(nxtscifunnumber).ReportFileName,'file');
      // #RNU_RES_E
      [TBFlagfound,TBType,TBSize,TBValue,TBFindLike,TBDimension,TBScope] = ST_GetSymbolInfo(tmpname,FileInfo,SharedInfo);
      if (TBFlagfound == 0)
         PrintStringInfo(' ',ReportFileName,'both','y');
         PrintStringInfo('SCI2CERROR: Unknown symbol ""'+tmpname+'"".',ReportFileName,'both','y');
         PrintStringInfo('SCI2CERROR: Be sure to initialize every symbol you are using.',ReportFileName,'both','y');
         PrintStringInfo('SCI2CERROR: Before running the SCI2C translator, remember to run the code you are trying',ReportFileName,'both','y');
         PrintStringInfo('SCI2CERROR: to translate in order to check syntax errors.',ReportFileName,'both','y');
         PrintStringInfo(' ',ReportFileName,'both','y');
         SCI2Cerror(' ');
      end
      UpdatedInArg(cntinarg).Type      = TBType;
      UpdatedInArg(cntinarg).Size      = TBSize;
      UpdatedInArg(cntinarg).Value     = TBValue;
      UpdatedInArg(cntinarg).FindLike  = TBFindLike;
      UpdatedInArg(cntinarg).Dimension = TBDimension;
      UpdatedInArg(cntinarg).Scope     = TBScope;
      
   else
      SCI2Cerror('Unknown scope identifier ""'+tmpscope+'"" for variable ""'+tmpname+'"".');
   end
   // #RNU_RES_B
   PrintStringInfo('   Type:      '+UpdatedInArg(cntinarg).Type,FileInfo.Funct(nxtscifunnumber).ReportFileName,'file');
   PrintStringInfo('   Size(1):   '+string(UpdatedInArg(cntinarg).Size(1)),FileInfo.Funct(nxtscifunnumber).ReportFileName,'file');
   PrintStringInfo('   Size(2):   '+string(UpdatedInArg(cntinarg).Size(2)),FileInfo.Funct(nxtscifunnumber).ReportFileName,'file');
   PrintStringInfo('   Value:     '+string(UpdatedInArg(cntinarg).Value),FileInfo.Funct(nxtscifunnumber).ReportFileName,'file');
   PrintStringInfo('   FindLike:  '+string(UpdatedInArg(cntinarg).FindLike),FileInfo.Funct(nxtscifunnumber).ReportFileName,'file');
   PrintStringInfo('   Dimension: '+string(UpdatedInArg(cntinarg).Dimension),FileInfo.Funct(nxtscifunnumber).ReportFileName,'file');
   PrintStringInfo('   Scope:     '+UpdatedInArg(cntinarg).Scope,FileInfo.Funct(nxtscifunnumber).ReportFileName,'file');
   // #RNU_RES_E
end

endfunction

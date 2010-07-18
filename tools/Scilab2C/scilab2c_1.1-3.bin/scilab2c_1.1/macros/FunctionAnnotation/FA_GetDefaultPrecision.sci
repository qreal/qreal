function defaultprecision = FA_GetDefaultPrecision(scifilename,ReportFileName)
// function defaultprecision = FA_GetDefaultPrecision(scifilename,ReportFileName)
// -----------------------------------------------------------------
// #RNU_RES_B
// Extracts the default precision for the file .sci passed in input.
// The following annotation will be searched in the scilab code:
// //SCI2C: DEFAULT_PRECISION= FLOAT
// //SCI2C: DEFAULT_PRECISION= DOUBLE
// If the annotation is missing the default DOUBLE precision will be
// implicitly used.
// #RNU_RES_E
// 
// Input data:
// ---
//
// Output data:
// defaultprecision: string which specifies the default precision to be 
//                   used in the translation of scifilename.
//
// Status:
// 12-Feb-2008 -- Raffaele Nutricato: Author.
// 12-Feb-2008 -- Alberto Morea: Test Ok.
//
// Copyright 2008 Raffaele Nutricato & Alberto Morea.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),2,2);

// -----------------------
// --- Initialization. ---
// -----------------------
defaultprecision = 'd';
annotationstring = '//SCI2C: DEFAULT_PRECISION='
// #RNU_RES_B
PrintStringInfo('***Get default precision from: '+scifilename,ReportFileName,'file','y');
// #RNU_RES_E
// ---------------------------
// --- End Initialization. ---
// ---------------------------

// --- Open the .sci file (read only). ---
scifid = SCI2COpenFileRead(scifilename);

// #RNU_RES_B
// --- Loop over the lines of the input file. ---
// Position file pointer to the desired NInArg/NOutArg section,
// and read the NOutArg annotation.
// #RNU_RES_E
foundannotation  = 0;
line_position    = 0;
while ((meof(scifid) == 0) & (foundannotation == 0))
   check_string = stripblanks(mgetl(scifid,1));
   line_position = line_position + 1;
   if (~isempty(check_string))
   
      if (SCI2Cstrncmps1size(annotationstring,check_string))
         tmpprecision = stripblanks(part(check_string,length(annotationstring)+1:length(check_string)));
         // #RNU_RES_B
         PrintStringInfo('   Line '+string(line_position)+...
            ' - Found annotation for default precision'+' ""'+check_string+' ""',...
            ReportFileName,'file','y');
         // #RNU_RES_E
         foundannotation = 1;
      end
   end
end

if (foundannotation == 0)
   // #RNU_RES_B
   PrintStringInfo('Annotation for default precision not found.',ReportFileName,'file','y');
   PrintStringInfo('Using the ""DOUBLE"" default precision.',ReportFileName,'file','y');
   // #RNU_RES_E
else
   if (tmpprecision == 'FLOAT')
      defaultprecision = 's';
   elseif (tmpprecision == 'DOUBLE')
      defaultprecision = 'd';
   end
end

mclose(scifid);
endfunction

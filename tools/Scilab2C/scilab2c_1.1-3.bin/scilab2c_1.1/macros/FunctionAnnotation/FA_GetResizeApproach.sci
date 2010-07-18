function ResizeApproach = FA_GetResizeApproach(scifilename,ReportFileName)

// function ResizeApproach = FA_GetResizeApproach(scifilename,ReportFileName)
// -----------------------------------------------------------------
// Extracts the resize approach from the file .sci passed in input.
// The following annotation will be searched in the scilab code:
// //SCI2C: RESIZE_APPROACH= NO_RESIZE
// //SCI2C: RESIZE_APPROACH= RESIZE_ALL
// //SCI2C: RESIZE_APPROACH= REALLOC_ALL_RESIZE_ALL
// //SCI2C: RESIZE_APPROACH= RESIZE_TEMP
// //SCI2C: RESIZE_APPROACH= RESIZE_LOCAL
// //SCI2C: RESIZE_APPROACH= RESIZE_GLOBAL
// If the annotation is missing the default NO_RESIZE approach will be
// implicitly used.
// 
// Input data:
// ---
//
// Output data:
// ResizeApproach: string which specifies the resize approach 
//                 used in the translation of scifilename.
//
// Status:
// 12-Jul-2008 -- Raffaele Nutricato: Author.
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
ResizeApproach = 'NO_RESIZE';
annotationstring = '//SCI2C: RESIZE_APPROACH='
PrintStringInfo('***Get resize approach from: '+scifilename,ReportFileName,'file','y'); // #RNUREM_ME
// ---------------------------
// --- End Initialization. ---
// ---------------------------

// --- Open the .sci file (read only). ---
scifid = SCI2COpenFileRead(scifilename);

// --- Loop over the lines of the input file. ---
// Position file pointer to the desired NInArg/NOutArg section,
// and read the NOutArg annotation.
foundannotation  = 0;
line_position    = 0;

while ((meof(scifid) == 0) & (foundannotation == 0))
   check_string = stripblanks(mgetl(scifid,1));
   line_position = line_position + 1;
   if (~isempty(check_string))
      if (SCI2Cstrncmps1size(annotationstring,check_string))
         tmpresize = stripblanks(part(check_string,length(annotationstring)+1:length(check_string)));
         // #RNU_RES_B
         PrintStringInfo('   Line '+string(line_position)+...
            ' - Found annotation for resize approach'+' ""'+check_string+' ""',...
            ReportFileName,'file','y');
         // #RNU_RES_E
         foundannotation = 1;
      end
   end
end

if (foundannotation == 0)
   // #RNU_RES_B
   PrintStringInfo('Annotation for resize approach not found.',ReportFileName,'file','y');
   PrintStringInfo('Using the ''NO_RESIZE'' resize approach.',ReportFileName,'file','y');
   // #RNU_RES_E
else
   ResizeApproach = tmpresize;
end
mclose(scifid);
endfunction

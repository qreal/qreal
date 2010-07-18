function Sci2AnnotationFile(SciFileName,ClsFileName,AnnFileName,AnnSpecifier,ReportFileName)
// function Sci2AnnotationFile(SciFileName,ClsFileName,AnnFileName,AnnSpecifier,ReportFileName)
// --------------------------------------------------------------------------------
// #RNU_RES_B
// This function reads the .sci input file and generates the correspondig .ann
// and .acls files.
// #RNU_RES_E
//
// Input data:
// //NUT: add description here
//
// Output data:
// //NUT: add description here
//
// Status:
// 25-Jun-2007 -- Nutricato Raffaele: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),5,5);

[tmppath,tmpfunname,tmpext] = fileparts(SciFileName);

// ---------------------------------------
// --- Open the .sci file (read only). ---
// ---------------------------------------
inscifid = SCI2COpenFileRead(SciFileName);

// ----------------------------------------------
// --- Loop over the lines of the input file. ---
// ----------------------------------------------
line_position = 0;
L_AnnSpecifierP1 = length(AnnSpecifier)+1;
while (meof(inscifid) == 0)
   check_string = stripblanks(mgetl(inscifid,1));
   line_position = line_position + 1;
   L_string = length(check_string);
   if (L_string >= 1)
      if (SCI2Cstrncmps1size(AnnSpecifier,check_string))
         tmpannotation = stripblanks(part(check_string,L_AnnSpecifierP1:L_string));
         PrintStringInfo(tmpannotation,ClsFileName,'file','y');
      end
   end
end
mclose(inscifid);
// --------------------------------------------------
// --- End loop over the lines of the input file. ---
// --------------------------------------------------
PrintStringInfo('CLASS: '+tmpfunname,AnnFileName,'file','y');
endfunction

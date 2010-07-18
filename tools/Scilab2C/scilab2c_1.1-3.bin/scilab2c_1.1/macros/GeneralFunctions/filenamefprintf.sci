function filenamefprintf(filename,ennewline,str,formattedstring)
// function filenamefprintf(filename,ennewline,str,formattedstring)
// --------------------------------------------------------------------------------
// Uses the printf to print the string specified by varargin. filenamefprintf
// uses the filename instead of the fid parameter used by fprintf. 
// Everytime filenamefprintf is called it
// opens the file, prints the string in it and then closes it.
// Opening is performed in read/append mode (at+).
//
// Input data:
// filename: string that specifies the name of the file.
// varargin are the input arguments for the printf.
// formattedstring: if 'n' (default) it means that str is considered as a simple string (mputstr).
//                  if 'y' then str is considered formatted according to mfprint syntax
//
// Output data:
// ---
//
// Status:
// 31-Jan-2006 -- Nutricato Raffaele: Author.
// 31-Jan-2006 -- Nutricato Raffaele: TEST OK.
//
// Copyright 2006 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),4,4);


//   [FidReportFile, mess] = mopen(deblank(filename),'at+');
   [FidReportFile, mess] = mopen(filename,'a+');
   if (FidReportFile == -1) then
      SCI2Cerror(mess);
   end
   if formattedstring == 'n'
      mputstr(str,FidReportFile);
   else
      mfprintf(FidReportFile, str);
   end   
   if ennewline=='y' then
      mfprintf(FidReportFile,'\n');
   end
   mclose(FidReportFile);
   
endfunction

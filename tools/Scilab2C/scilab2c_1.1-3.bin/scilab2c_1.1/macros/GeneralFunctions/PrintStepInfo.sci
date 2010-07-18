function PrintStepInfo(inputstring,filename,outputtype,formattedstring)
// function PrintStepInfo(inputstring,filename,outputtype,formattedstring)
// -----------------------------------------------------------------
// #RNU_RES_B
// Prints a string by using a predefined format into a file or on 
// the stdout.
//
// Input data:
// filename: optional parameter, that specifies the output file.
//           If filename is '' or it is not provided to the function,
//           the string will be printed on the stdout.
// outputtype: 'file' -> prints only on file.
//             'stdout' -> prints only on the stdout.
//             'both' -> prints on both file and stdoud.
//             Default is 'stdout'.
// formattedstring: if 'n' (default) it means that str is considered as a simple string (mputstr).
//                  if 'y' then str is considered formatted according to mfprint syntax
// Output data:
//
// #RNU_RES_E
// Status:
// 02-Jan-2006 -- Nutricato Raffaele: Author.
// 02-Jan-2006 -- Nutricato Raffaele: TEST OK.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),1,4);

if argn(2) < 4
   formattedstring = 'n';
   if argn(2) < 3
      bothout = 'n';
      if argn(2) < 2
         filename = '';
      end
   end
end
if (length(filename) == 0)
   outputtype = 'stdout'; // Prints only on the stdout.
end

Nstars = length(inputstring);
starstring = [];
for counterstars = 1:Nstars
   starstring = starstring+'*';
end
blankstring = ['         '];

if ((outputtype=='both') | (outputtype=='stdout'))
   // disp(' ')
   // disp(' ')
   disp(blankstring+'    '+starstring);
   disp(blankstring+'==> '+inputstring);
   disp(blankstring+'    '+starstring);
   // disp(' ')
end

if ((outputtype=='both') | (outputtype=='file'))
   filenamefprintf(filename,'y',' ',formattedstring);
   filenamefprintf(filename,'y',' ',formattedstring);
   filenamefprintf(filename,'y',blankstring+'    '+starstring,formattedstring);
   filenamefprintf(filename,'y',blankstring+'==> '+inputstring,formattedstring);
   filenamefprintf(filename,'y',blankstring+'    '+starstring,formattedstring);
   filenamefprintf(filename,'y',' ',formattedstring);
end
endfunction

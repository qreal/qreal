function ScilabFileName = FunName2SciFileName(DirList,InFunName);
// function ScilabFileName = FunName2SciFileName(DirList,InFunName);
// -----------------------------------------------------------------
// #RNU_RES_B
// This function generates the full path of the scilab file
// related to the function name (InFunName) specified. 
// In more detail the file "eval(InFunName).sci" file is searched
// in the directories specified in DirList.
// #RNU_RES_E
//
// Input data:
// //NUT: add description here
//
// Output data:
// //NUT: add description here
//
// Status:
// 16-Apr-2007 -- Nutricato Raffaele: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),2,2);

if (prod(size(DirList)) == 0)
   SCI2Cerror('Incorrect DirList parameter.');
end

if (prod(size(InFunName)) == 0)
   SCI2Cerror('Incorrect InFunName parameter.');
end

// --- Generate the PathList. ---
for tmpcounter = 1:max(size(DirList))
   PathList(tmpcounter) = fullfile(DirList(tmpcounter),(InFunName+'.sci'));
end

// --- Search the .sci file. ---
ScilabFileName = listfiles(PathList);

// --- Check on the number of .sci files found. ---
if ((prod(size(ScilabFileName))) > 1)
   disp(ScilabFileName);
   SCI2Cerror('Found more than one scilab file.');
end

if ((prod(size(ScilabFileName))) < 1)
   disp(ScilabFileName);
   SCI2Cerror('Scilab file ""'+InFunName+'.sci"", not found');
end

endfunction

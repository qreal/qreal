function OutPath = ConvertPathMat2C(InPath,CPathStyle)
// function OutPath = ConvertPathMat2C(InPath,CPathStyle)
// -----------------------------------------------------------------
// #RNU_RES_B
// Converts the input path InPath into a path by using the path
// style specified by CPathStyle.
// #RNU_RES_E
//
// Input data:
// //NUT: add description here
//
// Output data:
// //NUT: add description here
//
// Status:
// 26-Jan-2008 -- Nutricato Raffaele: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),2,2);
if (CPathStyle == 'windows')
   OutPath=strsubst(InPath,'/','\');
elseif (CPathStyle == 'unix')
   OutPath=strsubst(InPath,'\','/');
elseif (CPathStyle == 'cygwin')
   OutPath=strsubst(InPath,'\','/');
   OutPath=strsubst(OutPath,'A:','/cygdrive/a');
   OutPath=strsubst(OutPath,'B:','/cygdrive/b');
   OutPath=strsubst(OutPath,'C:','/cygdrive/c');
   OutPath=strsubst(OutPath,'D:','/cygdrive/d');
   OutPath=strsubst(OutPath,'E:','/cygdrive/e');
   OutPath=strsubst(OutPath,'F:','/cygdrive/f');
   OutPath=strsubst(OutPath,'G:','/cygdrive/g');
   OutPath=strsubst(OutPath,'H:','/cygdrive/h');
   OutPath=strsubst(OutPath,'I:','/cygdrive/i');
   OutPath=strsubst(OutPath,'J:','/cygdrive/j');
   OutPath=strsubst(OutPath,'K:','/cygdrive/k');
   OutPath=strsubst(OutPath,'L:','/cygdrive/l');
   OutPath=strsubst(OutPath,'M:','/cygdrive/m');
   OutPath=strsubst(OutPath,'N:','/cygdrive/n');
   OutPath=strsubst(OutPath,'O:','/cygdrive/o');
   OutPath=strsubst(OutPath,'P:','/cygdrive/p');
   OutPath=strsubst(OutPath,'Q:','/cygdrive/q');
   OutPath=strsubst(OutPath,'R:','/cygdrive/r');
   OutPath=strsubst(OutPath,'S:','/cygdrive/s');
   OutPath=strsubst(OutPath,'T:','/cygdrive/t');
   OutPath=strsubst(OutPath,'U:','/cygdrive/u');
   OutPath=strsubst(OutPath,'V:','/cygdrive/v');
   OutPath=strsubst(OutPath,'W:','/cygdrive/w');
   OutPath=strsubst(OutPath,'X:','/cygdrive/x');
   OutPath=strsubst(OutPath,'Y:','/cygdrive/y');
   OutPath=strsubst(OutPath,'Z:','/cygdrive/z');
else
   OutPath = InPath;
end
endfunction

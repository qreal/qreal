function SciFile2ASTFile(SciFile,ASTFile);
// function SciFile2ASTFile(SciFile,ASTFile);
// -----------------------------------------------------------------
// This function makes use of the macr2tree function to generate
// the ASTFile containing the AST (Abstract Syntactic Tree) of the
// input Scilab function (SciFile).
//
// Input data:
// SciFile: full path of the input function.
// ASTFile: full path of the file that will store the AST.
//
// Output data:
// ---
//
// Status:
// 12-Apr-2007 -- Raffaele Nutricato: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

getf(SciFile);
[tmppath,ScilabFunName,tmpext] = fileparts(SciFile);
AST=eval('macr2tree('+ScilabFunName+')');


[ASTx,ASTierr]=fileinfo(ASTFile);
if ASTierr == 0
   mdelete(ASTFile);
end

fd = mopen(ASTFile, "wt");
mputl(string(AST), fd);
mclose(fd);

endfunction

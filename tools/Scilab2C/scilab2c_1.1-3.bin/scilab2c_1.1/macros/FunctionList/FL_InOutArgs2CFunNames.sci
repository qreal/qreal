function FunNameCFuncList = FL_InOutArgs2CFunNames(FunctionName,CommaSepCFuncList,CFuncListNElem)
// function FunNameCFuncList = FL_InOutArgs2CFunNames(FunctionName,CommaSepCFuncList,CFuncListNElem)
// -----------------------------------------------------------------
// #RNU_RES_B
// Reads a Comma Separated Function List and converts it into the
// corresponding list of C function. In the Comma Separated
// Function List only input and output arguments are specified
// and they are separated by a comma.
// #RNU_RES_E
//
// Input data:
// //NUT: add description here
//
// Output data:
// //NUT: add description here
//
// Status:
// 05-Jan-2008 -- Nutricato Raffaele: Author.
//
// Copyright 2008 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------


// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),3,3);


FunNameCFuncList = '';
SepChar = ',';
for cntelem = 1:CFuncListNElem
   tmptokens = tokens(CommaSepCFuncList(cntelem),SepChar);
   if (size(tmptokens,1) == 0)
      FunNameCFuncList(cntelem) = FunctionName;
   elseif (size(tmptokens,1) == 1)
      if part(tmptokens,1:1) == ','
         FunNameCFuncList(cntelem) = FunctionName+tmptokens(2);
      else
         FunNameCFuncList(cntelem) = tmptokens(1)+FunctionName;
      end
   elseif (size(tmptokens,1) == 2)
      FunNameCFuncList(cntelem) = tmptokens(1)+FunctionName+tmptokens(2);
   else
      disp('Incorrect format for the function list class.');
      disp('Check the following function list class item: ""'+CommaSepCFuncList(cntelem)+'"".');
      SCI2Cerror(' ');
   end
end

endfunction

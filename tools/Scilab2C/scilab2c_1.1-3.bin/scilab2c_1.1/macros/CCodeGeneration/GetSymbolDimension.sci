function symboldimension = GetSymbolDimension(Field_Size)
// function symboldimension = GetSymbolDimension(Field_Size)
// -----------------------------------------------------------------
// #RNU_RES_B
// Get the dimesion (0D,1D,2D) of a symbol given its size.
//
// Input data:
// Field_Size: it is the Size field of the InArg or OutArg structures.
//             It is a 2-element array. N-dim array are not supported
//             in this release.
//
// Output data:
// symboldimension: number specifying the dimension of the symbol.
//                  0 = scalar; 1 = column or row; 2 = matrix.
//
// #RNU_RES_E
// Status:
// 26-Oct-2007 -- Raffaele Nutricato: Author.
// 26-Oct-2007 -- Alberto Morea: Test Ok.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),1,1);

// Size is expressed as an array of two strings.
Nelem = max(size(Field_Size));
if (Nelem < 2)
   SCI2Cerror('The size of a symbol cannot be expressed with one or zero numbers.');
end
for countersize = 1:Nelem
   // #RNU_RES_B
   // Field_Type = 1; if Size is Symbol or a number > 1
   // Field_Type = 0; if Size is a number == 1
   // error if Size is 0.
   // A symbol is scalar if the sum of the Field_Type elements is zero.
   // A symbol is column or row if the sum of the Field_Type elements is one.
   // A symbol is a matrix if the sum of the Field_Type elements is > 1.
   // #RNU_RES_E
   if (SCI2Cisnum(Field_Size(countersize)))
      tmpnum = eval(Field_Size(countersize));
      if (tmpnum == 0)
         SCI2Cerror('Found a symbol that has zeros elements. 0xN or Nx0 matrices are not allowed.');
      elseif (tmpnum == 1)
         Field_Type(countersize) = 0;
      else
         Field_Type(countersize) = 1;
      end
   else
     Field_Type(countersize) = 1;
   end
end

Sum_Field_Type = sum(Field_Type);
if (Sum_Field_Type == 0)
   symboldimension = 0;
elseif (Sum_Field_Type == 1)
   // #RNU_RES_B
   // symboldimension = 1; //NUT for this release there will not be difference between vectors and matrices.
   // #RNU_RES_E
   symboldimension = 2;
else
   symboldimension = 2;
end

endfunction

function opoutsize = FA_SZ_OPPLUS(in1size,in2size,in1type,in2type)
// function opoutsize = FA_SZ_OPPLUS(in1size,in2size,in1type,in2type)
// -----------------------------------------------------------------
// Returns the size of the output computed by OPPLUS operator,
// including the string operations.
// 
// Assuming:
// size(in1) = [in1r,in1c]
// size(in2) = [in2r,in2c]
// size(out) = [outr,outc]
//
// we have the following combinations:
// in1   in2   outr   outc
// -----------------------
//  S     S    in2r   in2c    
//  S     M    in2r   in2c
//  M     S    in1r   in1c
//  M     M    in1r   in1c
//
// Where S means that the input is a scalar
// and M means that the input is a matrix.
// There is also the case related to the string catenation!
// This is the main difference between - and + operators.
//
// Input data:
// in1size: size of input number 1. It is an array of 2 strings. 
//          The first string specifies the number of rows.
//          The second string specifies the number of columns.
// 
// in2size: size of input number 2. It is an array of 2 strings. 
//          The first string specifies the number of rows.
//          The second string specifies the number of columns.
//
// Output data:
// opoutsize: size of output. It is an array of 2 strings. 
//            The first string specifies the number of rows.
//            The second string specifies the number of columns. 
//
// Status:
// 08-Dec-2007 -- Raffaele Nutricato: Author.
// 08-Dec-2007 -- Alberto Morea: Test Ok.
//
// Copyright 2007 Raffaele Nutricato & Alberto Morea.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),4,4);

// ------------------------
// --- Generate Output. ---
// ------------------------
// --- Get dimensions of input arguments. ---
in1size = string(in1size);
in2size = string(in2size);
in1type = string(in1type);
in2type = string(in2type);
in1dim = GetSymbolDimension(in1size);

if ((in1type ~= 'g') & (in2type ~= 'g'))
   opoutsize = FA_SZ_OPPLUSA(in1size,in2size);
elseif ((in1type == 'g') & (in2type == 'g'))
   opoutsize(1) = '1';
   if (SCI2Cisnum(in1size(1)) & SCI2Cisnum(in2size(1)))
      in1num = eval(in1size(1));
      in2num = eval(in2size(1));
      if (in1num > 1 | in2num > 1)
         SCI2Cerror('String catenation can be performed only on strings of 1 x N size not N x 1 size');
         //NUT: mi pare che non possano proprio esistere stringe di dimensione Nx1 perche' in
         //NUT: scilab esiste il tipo string che e' di size 1x1 e sono io a trasformarlo in
         //NUT: 1xN per cui se uso sempre questa convenzione non sbaglio mai.
         //NUT: ho provato in scilab a fare la trasposta di una stringa e ottengo sempre 1x1.
      end
   end
   if (SCI2Cisnum(in1size(2)) & SCI2Cisnum(in2size(2)))
      in1num = eval(in1size(2));
      in2num = eval(in2size(2));
      opoutsize(2) = string(in1num+in2num-1);
      if isnan(opoutsize(2))
         opoutsize(2) = '__SCI2CNANSIZE';
      else
         opoutsize(2) = string(opoutsize(2));
      end
   else  
      opoutsize(2) = '('+string(in1size(2))+'+'+string(in2size(2))+'-1)';
   end
else
   SCI2Cerror('Unexpected type combination for ""+"" operator (type1,type2): ('+in1type+in2type+').');
end

endfunction

function SizeIn = SizeInByte(InDataType)
// function SizeIn = SizeInByte(InDataType)
// -----------------------------------------------------------------
// #RNU_RES_B
// Returns the size in bytes of the input data type.
//
// Input data:
// InDataType: input data type. It can be: 
//    'float'   
//    'double'
//    'floatComplex*'
//    'doubleComplex*'
//
// Output data:
// SizeIn: size in bytes of the input data type.
//
// #RNU_RES_E
// Status:
// 12-May-2007 -- Nutricato Raffaele: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),1,1);

if (InDataType == 'float')
   SizeIn = 4;
elseif (InDataType == 'double')
   SizeIn = 8;
elseif (InDataType == 'floatComplex*')
   SizeIn = 8;
elseif (InDataType == 'doubleComplex*')
   SizeIn = 16;
else
   error('Unknown data type: '+InDataType);
end
endfunction

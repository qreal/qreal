function outbool = SCI2Cisnum(instring)
// function outbool = SCI2Cisnum(instring)
// -----------------------------------------------------------------
// It fixes the bug of isnum. isnum('d') -> %T!!!
//
// Input data:
// instring: string to analyze.
//
// Output data:
// outbool: %T if instring is a number.
//
// Status:
// 12-Apr-2007 -- Nutricato Raffaele: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),1,1);

instring = convstr(instring,'l');
outbool   = isnum(instring);
firstchar = part(instring,1:1);
if (firstchar == 'd' | firstchar == 'e')
   outbool = %F;
end
endfunction

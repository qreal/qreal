function opoutsize = FA_SZ_OPSLASH(in1size,in2size)
// function opoutsize = FA_SZ_OPSLASH(in1size,in2size)
// -----------------------------------------------------------------
// Returns the size of the output computed by OPSLASH operator.
// 
// Assuming:
// size(in1) = [in1r,in1c]
// size(in2) = [in2r,in2c]
// size(out) = [outr,outc]
//


// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),2,2);

in1size = string(in1size);
in2size = string(in2size);

// ------------------------
// --- Generate Output. ---
// ------------------------
// --- Get dimensions of input arguments. ---
in1dim = GetSymbolDimension(in1size);
in2dim = GetSymbolDimension(in2size);

if (in1dim == 0)
   opoutsize = in2size;
elseif (in2dim == 0)
   opoutsize = in1size;
else
   opoutsize(1) = in1size(1);
   opoutsize(2) = in2size(1);
end

endfunction

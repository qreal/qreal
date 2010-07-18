//SCI2C: NIN=          2
//SCI2C: NOUT=         1
//SCI2C: OUT(1).TP=    IN(1).TP
//SCI2C: OUT(1).SZ(1)= FA_SZ_1(IN(1).SZ)
//SCI2C: OUT(1).SZ(2)= FA_SZ_2(IN(1).SZ)
//SCI2C: DEFAULT_PRECISION= FLOAT

function outrec = RecursivePower(in,pwrfct)


if (pwrfct == 0)
   outrec = ones(in);
end

if (pwrfct == 1)
   outrec = in;
end

if (pwrfct > 1)
   outrec = in .* RecursivePower(in,pwrfct-1);
end

endfunction

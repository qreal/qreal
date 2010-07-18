//SCI2C: NIN=          2
//SCI2C: NOUT=         1
//SCI2C: OUT(1).TP=    IN(1).TP
//SCI2C: OUT(1).SZ(1)= FA_SZ_1(IN(1).SZ)
//SCI2C: OUT(1).SZ(2)= FA_SZ_2(IN(1).SZ)
//SCI2C: DEFAULT_PRECISION= FLOAT

function outiter = IterativePower(in,pwrfct)

outiter = ones(in); 

for cnt = 1:pwrfct
   mytmp = outiter .* in;
   outiter = mytmp;
end

endfunction

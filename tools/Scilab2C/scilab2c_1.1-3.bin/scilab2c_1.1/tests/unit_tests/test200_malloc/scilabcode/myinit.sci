//SCI2C: NIN=          2
//SCI2C: NOUT=         1
//SCI2C: OUT(1).TP=    IN(1).TP
//SCI2C: OUT(1).SZ(1)= IN(1).VAL
//SCI2C: OUT(1).SZ(2)= IN(1).VAL
//SCI2C: DEFAULT_PRECISION= DOUBLE
function out = myinit(n,m)

out1 = m+ones(n,n);
out = out1+2*ones(n,n);

endfunction


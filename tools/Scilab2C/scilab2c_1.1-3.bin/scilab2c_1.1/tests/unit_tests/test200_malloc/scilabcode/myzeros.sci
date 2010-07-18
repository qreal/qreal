//SCI2C: NIN=          1
//SCI2C: NOUT=         1
//SCI2C: OUT(1).TP=    IN(1).TP
//SCI2C: OUT(1).SZ(1)= IN(1).SZ(1)
//SCI2C: OUT(1).SZ(2)= IN(1).SZ(2)
//SCI2C: DEFAULT_PRECISION= DOUBLE
function out = myzeros(in)

out = 3+zeros(in);
endfunction


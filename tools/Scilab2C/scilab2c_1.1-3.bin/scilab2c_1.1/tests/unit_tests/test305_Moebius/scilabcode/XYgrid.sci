//SCI2C: NIN=          2
//SCI2C: NOUT=         2
//SCI2C: OUT(1).TP=    IN(1).TP
//SCI2C: OUT(1).SZ(1)= IN(2).SZ(2)
//SCI2C: OUT(1).SZ(2)= IN(1).SZ(2)
//SCI2C: OUT(2).TP=    IN(1).TP
//SCI2C: OUT(2).SZ(1)= IN(2).SZ(2)
//SCI2C: OUT(2).SZ(2)= IN(1).SZ(2)
//SCI2C: DEFAULT_PRECISION= DOUBLE

// ------------------------------------
// this script emulate the matlab meshgrid 
// only for bidimensional mesh.
// ------------------------------------

function [X,Y] = XYgrid(x,y)

n = length(x);
m = length(y);

X=zeros(m,n);
Y=zeros(m,n);

for k=1:m
   X(k,1:n)=x;
end

for h=1:n
   Y(1:m,h)=y';
end

endfunction

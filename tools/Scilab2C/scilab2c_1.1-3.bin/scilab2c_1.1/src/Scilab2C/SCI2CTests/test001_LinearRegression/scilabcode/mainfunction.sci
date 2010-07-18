//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

// Knowing that 
// (1) P * (V^gamma) = C
// Where
// P = Pressure
// V = Volume
// gamma,C = constants depending on the particular gas used.
// (2) log10(P) = log10(C) - gamma*log10(V)
// (3) x = log10(V) 
// (4) y = log10(P)
// than (2) becomes:
// y = a + b*x;
// Where
// a = log10(C)
// b = -gamma
// Then thanks to this transformation it is possible to perform 
// a linear regression to estimate gamma and C!

Volume   = [54.3 61.8 72.4 88.7 118.6 194.0];
Pressure = [61.2 49.5 37.6 28.4 19.2 10.1];
x = log10(Volume);
y = log10(Pressure);

a = (sum(y)*sum(x.^2)-sum(x)*sum(x.*y))./(length(x)*sum(x.^2)-sum(x).*sum(x));
b = (length(x)*sum(x.*y)-sum(x)*sum(y))./(length(x)*sum(x.^2)-sum(x).*sum(x));

// Other way to compute a and b
beq = sum((x-mean(x)).*(y-mean(y)))./sum((x-mean(x)).^2);
aeq = mean(y)-mean(x)*beq;

C = 10 .^a;
gamma = -b;

disp('C')
disp(C)

disp('gamma');
disp(gamma);

disp('a');
disp(a)
disp('aeq');
disp(aeq)

disp('b');
disp(b)
disp('beq');
disp(beq)

// plot(Volume,Pressure);
// plot(Volume,(C ./(Volume.^gamma)),'r')
endfunction

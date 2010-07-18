//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

// ------------------------------
// --- Simple Scalar Addition ---
// ------------------------------
a = 1;
b = 2;
c = 0;
c = a + b;
disp(c);

// ------------------------------
// --- Trigonometric Identity ---
// ------------------------------
x = (1:3)' * (4:9);
y = (sin(x).^2) + (cos(x).^2);
disp(x);
disp(y-ones(3,6));

// -------------------------------
// --- Computation of Distance ---
// -------------------------------
// generate a vector w 
w = cos(sin(cos(x*3)*2).* x+ones(3,6).*cos(x-sin(y*2))); 
distxw = sqrt(x.^2 + w.^2);
disp(distxw);
endfunction
   
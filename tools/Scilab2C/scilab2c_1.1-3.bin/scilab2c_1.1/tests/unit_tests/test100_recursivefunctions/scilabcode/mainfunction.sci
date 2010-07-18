//SCI2C: DEFAULT_PRECISION= FLOAT
function mainfunction()

// ---------------------------------------
// --- Initialization of the operands. ---
// ---------------------------------------
powerfactor = 3;
disp('Power Factor: ');
disp(powerfactor)

s1 = 2;
disp('Input Scalar Value');
disp(s1)

V1 = 1:3;
V2 = 1:2;
V1tr = V1';

M1 = V1tr * V2;
disp('Input Matrix Value');
disp(M1)

// -----------------------
// --- Compute powers. ---
// -----------------------
// --- Iterative/Scalar. ---
outs1It = IterativePower(s1,powerfactor);
disp('Output Scalar Value Iterative');
disp(outs1It);

// --- Iterative/Matrix. ---
outM1It = IterativePower(M1,powerfactor);
disp('Output Matrix Iterative');
disp(outM1It);

// --- Recursive/Scalar. ---
outs1Re = RecursivePower(s1,powerfactor);
disp('Output Scalar Value Recursive');
disp(outs1Re);

// --- Recursive/Matrix. ---
outM1Re = RecursivePower(M1,powerfactor);
disp('Output Matrix Recursive');
disp(outM1Re);

//~ // --- Solution/Scalar. ---
//~ outs1So = s1.^powerfactor;
//~ disp('Output Scalar Value Solution');
//~ disp(outs1So);

//~ // --- Solution/Matrix. ---
//~ outM1So = M1.^powerfactor;
//~ disp('Output Matrix Value Solution');
//~ disp(outM1So);

endfunction


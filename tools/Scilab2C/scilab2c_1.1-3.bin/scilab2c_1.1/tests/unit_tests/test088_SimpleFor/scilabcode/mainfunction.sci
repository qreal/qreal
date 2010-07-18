function mainfunction()

// ---------------------------------------
// --- Initialization of the operands. ---
// ---------------------------------------

v1 = zeros(1,5);
disp('v1, before initialization.')
disp(v1)
for cnt = 1:5
   v1(cnt) = cnt;
end
disp('v1, after initialization.')
disp(v1)

endfunction


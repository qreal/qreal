//SCI2C: DEFAULT_PRECISION= DOUBLE

function mainfunction()

s1 = (1:4)' * (1:3);
disp('s1');
disp(s1);

s2 = sum(s1);
disp('s2 = sum(s1);');
disp(s2);

s3 = sum(s1,1);
disp('s3 = sum(s1,1);');
disp(s3);

s4 = sum(s1,2);
disp('s4 = sum(s1,2);');
disp(s4);

endfunction


function runscicode(UserScilabMainFile, UserSciFilesPaths)
// function runscicode(SCI2CInputPrmFile)
// -----------------------------------------------------------------
// ===         hArtes/PoliBa/GAP SCI2C tool                      ===
// ===         Authors:                                          ===
// ===            Raffaele Nutricato                             ===
// ===            raffaele.nutricato@tiscali.it                  ===
// ===            Alberto Morea                                  ===
//
// Run the code written by the user before translating it.
//
// Input data:
// ---
// Output data:
// ---
//
// Status:
// 11-Apr-2007 -- Raffaele Nutricato: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// -------------------
// --- Soft reset. ---
// -------------------
//mode(-1);
//clc;
// -----------------------
// --- End Soft reset. ---
// -----------------------

// -------------------------
// --- Input Parameters. ---
// -------------------------
//RunSci2CMainDir = pwd();
// -----------------------------
// --- End input Parameters. ---
// -----------------------------

//cd(fullfile(RunSci2CMainDir,'ToolInitialization'));
//exec('INIT_SCI2CLoader.sce');
//cd(RunSci2CMainDir);

// --- Read user parameters. ---
//exec(SCI2CInputPrmFile);

// --- Add all user paths. ---
for cntpath = 1:size(UserSciFilesPaths,1)
   getd(UserSciFilesPaths(cntpath));
end

// --- Execute code. ---
disp('-----------------------------------');
disp('--- Executing your SCILAB code. ---');
disp('-----------------------------------');
exec(UserScilabMainFile);
[tmppath,tmpfile,tmpext] = fileparts(UserScilabMainFile);
//cd(tmppath);
execstr(tmpfile);
//cd(RunSci2CMainDir);
disp('------------------------------------------');
disp('--- End Execution of your SCILAB code. ---');
disp('------------------------------------------');


endfunction
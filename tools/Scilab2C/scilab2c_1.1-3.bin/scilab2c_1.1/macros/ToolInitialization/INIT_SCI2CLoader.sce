// -----------------------------------------------------------------
// Load SCI2C directories and files.
//
// Input data:
// SCI2CLoaderMainDir: path of the directory where this script (main.sce) is stored.
//
// Output data:
// ---
//
// Status:
// 11-Apr-2007 -- Raffaele Nutricato: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

SCI2CLoaderMainDir = '..';

// ---------------------------
// --- Define Directories. ---
// ---------------------------
// Directory containing functions related to the management of the Abstract Syntactic tree.
ASTManagement         = 'ASTManagement';

// Directory containing functions that produce the C code.
CCodeGeneration       = 'CCodeGeneration';

// Directory containing functions that perform general tasks.
GeneralFunctions      = 'GeneralFunctions';

// Directory containing functions that perform the initialization of the SCI2C tool.
ToolInitialization    = 'ToolInitialization';

// Directory containing functions that perform the function annotation.
FunctionAnnotation    = 'FunctionAnnotation';

// Directory containing functions that handle symbol table.
SymbolTable    = 'SymbolTable';

// Directory containing functions that handle function lists.
FunctionList    = 'FunctionList';

// Directory containing functions that print SCI2C error messages.
ErrorMessages    = 'ErrorMessages';


// -------------------------------
// --- End Define Directories. ---
// -------------------------------

// -------------
// --- getd. ---
// -------------
getd(fullfile(SCI2CLoaderMainDir,ASTManagement));
getd(fullfile(SCI2CLoaderMainDir,CCodeGeneration));
getd(fullfile(SCI2CLoaderMainDir,GeneralFunctions));
getd(fullfile(SCI2CLoaderMainDir,ToolInitialization));
getd(fullfile(SCI2CLoaderMainDir,FunctionAnnotation));
getd(fullfile(SCI2CLoaderMainDir,SymbolTable));
getd(fullfile(SCI2CLoaderMainDir,FunctionList));
getd(fullfile(SCI2CLoaderMainDir,ErrorMessages));
// -----------------
// --- End getd. ---
// -----------------

// -------------
// --- exec. ---
// -------------
exec(fullfile(SCI2CLoaderMainDir,ASTManagement,'%program_p.sci'));
// -----------------
// --- End exec. ---
// -----------------

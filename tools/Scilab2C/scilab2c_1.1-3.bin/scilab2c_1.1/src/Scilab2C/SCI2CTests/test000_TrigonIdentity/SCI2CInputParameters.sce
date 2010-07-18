// -----------------------------------------------------------------
// ===         hArtes/PoliBa/GAP SCI2C tool                      ===
// ===         Authors:                                          ===
// ===            Raffaele Nutricato                             ===
// ===            raffaele.nutricato@tiscali.it                  ===
// ===            Alberto Morea                                  ===
// ===                                                           ===
// ===         ***************                                   ===
// ===         USER PARAMETERS                                   ===
// ===         ***************                                   ===
// ===                                                           ===
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------


// ------------------------------------------
// --- Specify Paths of User Scilab code. ---
// ------------------------------------------
// UserSciCodeMainDir = 'D:\Nutricato\OpenProjects\FP6_hArtes\WP2_SCI2C\Software\svnhartes\src\Scilab2C\SCI2CTests\test999_WorkingDir';
UserSciCodeMainDir = '../SCI2CTests/test999_WorkingDir';

// --- Path + filename of the .sci main file of the code to be translated. ---
// It is the entry point.
UserScilabMainFile = fullfile(UserSciCodeMainDir,'scilabcode\mainfunction.sci');

// --- List of the paths containing the .sci files written by the user. ---
UserSciFilesPaths = ...
   [...
      fullfile(UserSciCodeMainDir,'scilabcode');...
   ];

// --------------------------
// --- End Specify paths. ---
// --------------------------


// ----------------------------
// --- Select the run mode. ---
// ----------------------------
//RunMode = 'GenLibraryStructure';
//RunMode = 'Translate';
RunMode = 'All';

// --- Select one of the following options. ---

// 'GenLibraryStructure'; 
// Generates the library structure and exits. It is very 
// useful when the user wants to manually change the files stored in that structure
// before running the translation. 'GenLibraryStructure' option forces SCI2C to remove
// the already existing WorkingDir and OutCCCodeDir directories.

// 'Translate';
// Performs the translation without generating the library structure. It means that the library
// structure must be already existing. This is useful when the user doesn't want to spend time 
// to generate again that structure or when he wants to force the SCI2C tool to access to 
// a manually-changed library structure. 'Translate' option forces SCI2C to don't remove
// the already existing WorkingDir. Only OutCCCodeDir directory will be removed.

// 'All';
// Performs all the actions listed above.

// --------------------------------
// --- End Select the run mode. ---
// --------------------------------


// ----------------------------
// --- Translation Options. ---
// ----------------------------
// --- Enable (1) / Disable (0) copy of Scilab code into C code. --- 
// If 1 the Scilab code will be copied into the C code in order to show
// how each Scilab code line has been translated into C code.
CopySciCodeIntoCCode = 1;

// --- Select the path style for the C code. ---
// It can be:
// windows
// unix
// cygwin
CCompilerPathStyle   = 'cygwin';

// --- Path + File name of the main SCI2C library header file.
// Sci2CLibMainHeaderFName = 'D:\Nutricato\OpenProjects\FP6_hArtes\WP2_SCI2C\Software\svnhartes\src\Scilab2C\Scilab2C\CFiles\sci2cincludes\sci2clib.h';
Sci2CLibMainHeaderFName = '../../../Scilab2C/CFiles/sci2cincludes/sci2clib.h';
// --------------------------------
// --- End Translation Options. ---
// --------------------------------

// Before launching this script, please
// check all the paths below.
// User should change parameters only in (USER PARAMETERS) sections.
// cd C:\Nutricato\OpenProjects\FP6_hArtes\WP2_SCI2C\Software\Scilab2C; exec LaunchMultiRunSCI2C.sci

function LaunchMultiRunSCI2C()
// --- CLEAN WORKSPACE ---
//exec full_reset.sce;
//mode(-1);

// --- GENERAL SETTINGS (USER PARAMETERS) ---
RunsDirectory         = pwd(); // Path of the SCI2CRuns directory
SCI2CDirectory        = 'D:\Nutricato_GAPSVN\PROGETTI_APERTI\POLIBA\hArtes\WP2_SCI2C\Software\Version_alpha\Scilab2C'; // Path of the Scilab2C directory

// --- SPECIFIC SETTINGS (USER PARAMETERS) ---
FullListSCI2CInputPrmFiles = ...
   [...
      'D:\Nutricato_GAPSVN\PROGETTI_APERTI\POLIBA\hArtes\WP2_SCI2C\Software\Version_alpha\SCI2CTests\test999_WorkingDir\SCI2CInputParameters.sce';...
   ];
WorkingListSCI2CInputPrmFiles = ...
   [...
      'D:\Nutricato_GAPSVN\PROGETTI_APERTI\POLIBA\hArtes\WP2_SCI2C\Software\Version_alpha\SCI2CTests\test999_WorkingDir\SCI2CInputParameters.sce';...
   ];

// Select one of the two lists above.

ListSCI2CInputPrmFiles = WorkingListSCI2CInputPrmFiles;
//ListSCI2CInputPrmFiles = FullListSCI2CInputPrmFiles;

// ================================
// ================================
// ================================
// ================================

// --- LAUNCH MULTIRUNSCI2C ---
NTranslations = size(ListSCI2CInputPrmFiles,1);
for cnttransl = 1:NTranslations
   cd(SCI2CDirectory);

   SCI2CInputPrmFileName = ListSCI2CInputPrmFiles(cnttransl);

   // --- LAUNCH USER SCI CODE TO TEST IT BEFORE TRANSLATING IT!!! ---
   cd(SCI2CDirectory)
   getf("runscicode.sci");
   runscicode(SCI2CInputPrmFileName);

   // --- ASK USER FOR CONTINUATION. ---
   userchoice = input('Start translation [y/n]?','s');
   if (userchoice == 'y')

      // --- LAUNCH SCI2C ---
      cd(SCI2CDirectory)
      getf("runsci2c.sci");
      runsci2c(SCI2CInputPrmFileName);
      cd(SCI2CDirectory)
   else
   end
end

// ----------------------------------------
// --- Compile and Launch all the code. ---
// ----------------------------------------
// This option is available only for regression tests.
if (1==1)
   cd(fullfile(SCI2CDirectory,'ToolInitialization'));
   exec('INIT_SCI2CLoader.sce');

   PrintStepInfo('Generate Script for Regression Tests...Available only for Debug.','','both');
   C_GenerateLaunchScript('D:\Nutricato\OpenProjects\FP6_hArtes\WP2_SCI2C\Software\SCI2CTests\RegressionTests',ListSCI2CInputPrmFiles);
end

// ---------------------------------------
// --- Go back to the start directory. ---
// ---------------------------------------
cd (RunsDirectory);

// ------------------------
// --- Close all files. ---
// ------------------------
// --- This must be the last instruction. ---
mclose('all');
endfunction
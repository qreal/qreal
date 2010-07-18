// Before launching this script, please
// check all the paths below.
// User should change parameters only in (USER PARAMETERS) sections.
// cd C:\SCI2CTests\SCI2CRuns; exec LaunchRunSCI2C.sci

function LaunchRunSCI2C()
// --- CLEAN WORKSPACE ---
// exec full_reset.sce;
// mode(-1);

// --- GENERAL SETTINGS (USER PARAMETERS) ---
RunsDirectory         = pwd(); // Path of the SCI2CRuns directory

SCI2CDirectory        = '.';

// --- SPECIFIC SETTINGS (USER PARAMETERS) ---
SCI2CInputPrmFileName = '../SCI2CTests/test999_WorkingDir/SCI2CInputParameters.sce';

// --- LAUNCH USER SCI CODE TO TEST IT BEFORE TRANSLATING IT!!! ---
//cd(SCI2CDirectory)
//getf("runscicode.sci");
runscicode(SCI2CInputPrmFileName);

// --- ASK USER FOR CONTINUATION. ---
userchoice = input('Start translation [y/n]?','s');
if (userchoice == 'y')
   // --- LAUNCH SCI2C ---
   //cd(SCI2CDirectory);
   //getf("runsci2c.sci");
   runsci2c(SCI2CInputPrmFileName);
   cd(SCI2CDirectory);
end

// --- GO BACK TO THE ORIGINAL DIRECTORY. ---
cd (RunsDirectory);

// --- CLOSE ALL FILES. ---
mclose('all');
endfunction
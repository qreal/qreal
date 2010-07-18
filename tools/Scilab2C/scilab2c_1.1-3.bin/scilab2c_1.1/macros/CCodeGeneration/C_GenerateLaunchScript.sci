function C_GenerateLaunchScript(OutDir,ListSCI2CInputPrmFiles)
// function C_GenerateLaunchScript(OutDir,ListSCI2CInputPrmFiles)
// -----------------------------------------------------------------
// #RNU_RES_B
// Generate the script that can be used to compile all the regression
// tests and to run them and finally to write results in the report
// file.
// #RNU_RES_E
//
// Input data:
// //NUT: add description here
//
// Output data:
// //NUT: add description here
//
// Status:
// 26-Jan-2008 -- Raffaele Nutricato: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),2,2);

// -----------------------
// --- Initialization. ---
// -----------------------
ScriptFileName = fullfile(OutDir,'LaunchRegressionTests.rc');
NTranslations = size(ListSCI2CInputPrmFiles,1);
// ---------------------------
// --- End Initialization. ---
// ---------------------------
SCI2Cmdelete(ScriptFileName);
PrintStringInfo('#! /bin/bash ',ScriptFileName,'file','y');
PrintStringInfo(' ',ScriptFileName,'file','y');
PrintStringInfo('maindir=$PWD',ScriptFileName,'file','y');
PrintStringInfo(' ',ScriptFileName,'file','y');
PrintStringInfo('INTIALIZE()',ScriptFileName,'file','y');
PrintStringInfo('{',ScriptFileName,'file','y');
PrintStringInfo('   reportfile=$maindir/RegressionTestsReport.txt',ScriptFileName,'file','y');
PrintStringInfo('   echo ""#############'+'#################"" > $reportfile',ScriptFileName,'file','y');
PrintStringInfo('   echo ""REPORT OF THE REGRESSION TESTS"" > $reportfile',ScriptFileName,'file','y');
PrintStringInfo('   echo ""#############'+'#################"" > $reportfile',ScriptFileName,'file','y');
PrintStringInfo('   echo ""Author: Raffaele Nutricato"" > $reportfile',ScriptFileName,'file','y');
PrintStringInfo('   echo ""Copyright 2008 Raffaele Nutricato"" > $reportfile',ScriptFileName,'file','y');
PrintStringInfo('   echo "" "" > $reportfile',ScriptFileName,'file','y');
PrintStringInfo('   cd $maindir   ',ScriptFileName,'file','y');
PrintStringInfo('}',ScriptFileName,'file','y');
PrintStringInfo(' ',ScriptFileName,'file','y');
PrintStringInfo('EXECUTE()',ScriptFileName,'file','y');
PrintStringInfo('{',ScriptFileName,'file','y');
PrintStringInfo('   echo ""xxxxxxxxxxxxxx'+'xxxxxxxxxxxxxxxxxx'+'xxxxxxxxxxxx""',ScriptFileName,'file','y');
PrintStringInfo('   echo "" "" >> $reportfile',ScriptFileName,'file','y');
PrintStringInfo('   echo "" "" >> $reportfile',ScriptFileName,'file','y');
PrintStringInfo('   echo "" "" >> $reportfile',ScriptFileName,'file','y');
PrintStringInfo('   echo ""xxxxxxxxxxxxxx'+'xxxxxxxxxxxxxxxxxxxxxxx'+'xxxxxxx"" >> $reportfile',ScriptFileName,'file','y');
PrintStringInfo('   echo $testname ',ScriptFileName,'file','y');
PrintStringInfo('   echo $testname >> $reportfile',ScriptFileName,'file','y');
PrintStringInfo('   echo ""xxxxxxxxxxxxxxx'+'xxxxxxxxxxxxxxxxx'+'xxxxxxxxxxxx"" ',ScriptFileName,'file','y');
PrintStringInfo('   echo ""xxxxxxxxxxxxxxxx'+'xxxxxxxxxxxxxxxxxxxx'+'xxxxxxxx"" >> $reportfile',ScriptFileName,'file','y');
PrintStringInfo('   cd $testname/C_Code',ScriptFileName,'file','y');
PrintStringInfo('   make >> $reportfile',ScriptFileName,'file','y');
PrintStringInfo('   cd $maindir',ScriptFileName,'file','y');
PrintStringInfo('   echo "" "" >> $reportfile',ScriptFileName,'file','y');
PrintStringInfo('   echo "" "" >> $reportfile',ScriptFileName,'file','y');
PrintStringInfo('   echo "" "" >> $reportfile',ScriptFileName,'file','y');
PrintStringInfo('}',ScriptFileName,'file','y');
PrintStringInfo(' ',ScriptFileName,'file','y');
PrintStringInfo('#############'+'##############',ScriptFileName,'file','y');
PrintStringInfo('### ADD YOUR TESTS HERE ###',ScriptFileName,'file','y');
PrintStringInfo('##############'+'#############',ScriptFileName,'file','y');
PrintStringInfo('INTIALIZE',ScriptFileName,'file','y');
PrintStringInfo(' ',ScriptFileName,'file','y');

for cnttransl = 1:NTranslations
   [testpath,tmpname,tmpext] = fileparts(ListSCI2CInputPrmFiles(cnttransl));
   testpath = ConvertPathMat2C(testpath,'cygwin');
   PrintStringInfo('testname=""'+testpath+'""',ScriptFileName,'file','y');
   PrintStringInfo('EXECUTE $testname',ScriptFileName,'file','y');
   PrintStringInfo(' ',ScriptFileName,'file','y');
end

endfunction

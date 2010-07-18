function INIT_CreateDirs(FileInfo)
// function INIT_CreateDirs(FileInfo)
// -----------------------------------------------------------------
// Create directories.
//
// Input data:
// FileInfo: structure containing all info about SCI2C files.
//
// Output data:
// ---
//
// Status:
// 03-Jan-2008 -- Raffaele Nutricato: Author.
//
// Copyright 2008 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),1,1);

// ---------------------------
// --- Create Directories. ---
// ---------------------------
// --- Main directories. ---
SCI2CCreateDir(FileInfo.WorkingDir);
SCI2CCreateDir(FileInfo.OutCCCodeDir);

// --- SCI2C Library. ---
SCI2CCreateDir(FileInfo.SCI2CLibDir);
SCI2CCreateDir(FileInfo.SCI2CLibSCIAnnDir);
SCI2CCreateDir(FileInfo.SCI2CLibSCIAnnFun);
SCI2CCreateDir(FileInfo.SCI2CLibSCIAnnCls);
SCI2CCreateDir(FileInfo.SCI2CLibSCIFunListDir);
SCI2CCreateDir(FileInfo.SCI2CLibSCIFLFun);
SCI2CCreateDir(FileInfo.SCI2CLibSCIFLCls);   
SCI2CCreateDir(FileInfo.SCI2CLibCAnnDir);   
SCI2CCreateDir(FileInfo.SCI2CLibCAnnFun);    
SCI2CCreateDir(FileInfo.SCI2CLibCAnnCls);    
SCI2CCreateDir(FileInfo.SCI2CLibCFunListDir);
SCI2CCreateDir(FileInfo.SCI2CLibCFLFun);
SCI2CCreateDir(FileInfo.SCI2CLibCFLCls);     

// --- USER2C Library. ---
SCI2CCreateDir(FileInfo.USER2CLibDir);     
SCI2CCreateDir(FileInfo.USER2CLibSCIAnnDir);  
SCI2CCreateDir(FileInfo.USER2CLibSCIAnnFun); 
SCI2CCreateDir(FileInfo.USER2CLibSCIAnnCls); 
SCI2CCreateDir(FileInfo.USER2CLibSCIFunListDir);
SCI2CCreateDir(FileInfo.USER2CLibSCIFLFun);
SCI2CCreateDir(FileInfo.USER2CLibSCIFLCls);    
SCI2CCreateDir(FileInfo.USER2CLibCAnnDir);
SCI2CCreateDir(FileInfo.USER2CLibCAnnFun);     
SCI2CCreateDir(FileInfo.USER2CLibCAnnCls);     
SCI2CCreateDir(FileInfo.USER2CLibCFunListDir);
SCI2CCreateDir(FileInfo.USER2CLibCFLFun); 
SCI2CCreateDir(FileInfo.USER2CLibCFLCls);      

// --- Function List. ---
SCI2CCreateDir(FileInfo.FunctionList.MainDir);
SCI2CCreateDir(FileInfo.FunctionList.FunInfoDatDir);

endfunction

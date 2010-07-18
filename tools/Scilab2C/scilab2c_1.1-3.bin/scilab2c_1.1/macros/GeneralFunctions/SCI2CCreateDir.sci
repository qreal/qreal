function SCI2CCreateDir(OutDir)
// function SCI2CCreateDir(OutDir)
// -----------------------------------------------------------------
// Create the dir OutDir.
//
// Input data:
// OutDir: full path (absolute or relative) of the directory to be created.
//
// Output data:
// ---
//
// Status:
// 25-Jun-2007 -- Raffaele Nutricato: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),1,1);

[tmppath,tmpfname,tmpextension]=fileparts(OutDir)  ;

status_dir = mkdir(tmppath,tmpfname+tmpextension) ;
if (status_dir == 0)
   SCI2Cerror('Cannot create: '+OutDir);
end

endfunction

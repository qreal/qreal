//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

function scilab2c(varargin)
  [lhs, rhs] = argn();
  
  select rhs
    //
    // scilab2c()
    //
   case 0
    sci2c_gui();
    return
    //
    // scilab2c(UserScilabMainFile, CCodeOutputDir)
    //
   case 2
    for i = 1:2
      if typeof(varargin(i)) <> "string"
	error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"scilab2c",i));
	return
      end
    end
    UserScilabMainFile = varargin(1);
    CCodeOutputDir = varargin(2);
    UserSciFilesPaths = [];
    RunMode = 'All';
    //RunMode = 'GenLibraryStructure';
    //RunMode = 'Translate';
    //
    // scilab2c(UserScilabMainFile, CCodeOutputDir, UserSciFilesPaths)
    //
   case 3
    for i = 1:3
      if typeof(varargin(i)) <> "string"
	error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"scilab2c",i));
	return
      end
    end
    UserScilabMainFile = varargin(1);
    CCodeOutputDir = varargin(2);
    if varargin(3) == ""
      UserSciFilesPaths = [];
    else
      UserSciFilesPaths = varargin(3);
    end
    RunMode = "All";
    //
    // scilab2c(UserScilabMainFile, CCodeOutputDir, UserSciFilesPaths, RunMode)
    //
   case 4
    for i = 1:4
      if typeof(varargin(i)) <> "string"
	error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"scilab2c",i));
	return
      end
    end
    if varargin(4) <> "All" & varargin(4) <> "Translate" & varargin(4) <> "GenLibraryStructure"
      	error(msprintf(gettext("%s: argument #%d must be: ""All"", ""Translate"" or ""GenLibraryStructure"".\n"),"scilab2c",4));
	return
      return
    end
    UserScilabMainFile = varargin(1);
    CCodeOutputDir = varargin(2);
    if varargin(3) == ""
      UserSciFilesPaths = [];
    else
      UserSciFilesPaths = varargin(3);
    end
    RunMode = varargin(4);
  else
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"scilab2c",2));
  end


// --- LAUNCH USER SCI CODE TO TEST IT BEFORE TRANSLATING IT!!! ---
    runscicode(UserScilabMainFile, UserSciFilesPaths);
// --- ASK USER FOR CONTINUATION. ---

// -- FIXME : bypass this for now !!
    userchoice = messagebox("Exection Succesfull. Start translation ?", "modal", "info", ["Yes" "No"])
    if (userchoice == 1)
// --- LAUNCH SCI2C ---
      runsci2c(UserScilabMainFile, UserSciFilesPaths, CCodeOutputDir, RunMode);
    end

endfunction

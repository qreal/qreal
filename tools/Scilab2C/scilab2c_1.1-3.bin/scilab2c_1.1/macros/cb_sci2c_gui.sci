//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

// Callback function for SCI2C GUI (See sci2c_gui.sci)

function cb_sci2c_gui

//
// --- File to convert ---
//
if get(gcbo,"tag")=="filebtn" then

  filename = uigetfile("*.sci", pwd(), gettext("Select the file to translate"));
  if ~isempty(filename) then
    set(findobj("tag", "fileedit"), "string", filename);
  end

//
// --- Sub-functions directory ---
//
elseif get(gcbo,"tag")=="subfunsbtn" then

  directory = uigetdir(pwd(), gettext("Select your sub-functions directory"));
  if ~isempty(directory) then
    set(findobj("tag", "subfunsedit"), "string", directory);
  end

//
// --- Output directory ---
//
elseif get(gcbo,"tag")=="outbtn" then

  directory = uigetdir(pwd(), gettext("Select the directory for generated files"));
  if ~isempty(directory) then
    set(findobj("tag", "outedit"), "string", directory);
  end

//
// --- Run mode option ---
//
elseif or(get(gcbo, "tag")==["runradioall","runradiotranslate","runradiogenlib"]) then

  set(findobj("tag", "runradioall"), "value", 0);
  set(findobj("tag", "runradiotranslate"), "value", 0);
  set(findobj("tag", "runradiogenlib"), "value", 0);

  set(gcbo, "value", 1);

//
// --- Copy Scilab code into C option ---
//
elseif or(get(gcbo, "tag")==["sciintocradioyes","sciintocradiono"]) then

  set(findobj("tag", "sciintocradioyes"), "value", 0);
  set(findobj("tag", "sciintocradiono"), "value", 0);

  set(gcbo, "value", 1);

//
// --- Paths style option ---
//
elseif or(get(gcbo, "tag")==["pathsradiowin","pathsradiounix","pathsradiocygwin"]) then

  set(findobj("tag", "pathsradiowin"), "value", 0);
  set(findobj("tag", "pathsradiounix"), "value", 0);
  set(findobj("tag", "pathsradiocygwin"), "value", 0);

  set(gcbo, "value", 1);

//
// --- Cancel conversion ---
//
elseif get(gcbo, "tag")=="cancelbtn" | get(gcbo, "tag")=="close_menu" then
  delete(findobj("tag", "sci2cfig"));

//
// --- Launch conversion ---
//
elseif get(gcbo, "tag")=="convertbtn" then
  UserScilabMainFile =  get(findobj("tag", "fileedit"), "string");

  UserSciFilesPaths = get(findobj("tag", "subfunsedit"), "string");

//  Sci2CLibMainHeaderFName = get(findobj("tag", "headeredit"), "string");

  UserSciCodeMainDir = get(findobj("tag", "outedit"), "string");

  if get(findobj("tag", "runradioall"), "value") == 1 then
    RunMode = "All";
  elseif get(findobj("tag", "runradiotranslate"), "value") == 1 then
    RunMode = "Translate";
  else
    RunMode = "GenLibraryStructure";
  end

  CopySciCodeIntoCCode = get(findobj("tag", "sciintocradioyes"), "value") == 1;

  if get(findobj("tag", "pathsradiowin"), "value") == 1 then
    CCompilerPathStyle = "windows";
  elseif get(findobj("tag", "pathsradiounix"), "value") == 1 then
    CCompilerPathStyle = "unix";
//  else
//    CCompilerPathStyle = "cygwin";
  end

  mprintf("UserScilabMainFile = {%s}\n", UserScilabMainFile);
  mprintf("UserSciFilesPaths = {%s}\n", UserSciFilesPaths);
//  mprintf("Sci2CLibMainHeaderFName = {%s}\n", Sci2CLibMainHeaderFName);
  mprintf("UserSciCodeMainDir = {%s}\n", UserSciCodeMainDir);
  mprintf("RunMode = {%s}\n", RunMode);
  mprintf("CopySciCodeIntoCCode = {%d}\n", bool2s(CopySciCodeIntoCCode));
  mprintf("CCompilerPathStyle = {%s}\n", CCompilerPathStyle);
  scilab2c(UserScilabMainFile, UserSciCodeMainDir, UserSciFilesPaths, RunMode);
//
// --- sci2c help ---
//
elseif get(gcbo, "tag")=="sci2c_help_menu" then
  help sci2c

//
// --- About SCI2C ---
//
elseif get(gcbo, "tag")=="about_sci2c_menu" then
  help(gettext("About_SCI2C_tools"))
end

endfunction

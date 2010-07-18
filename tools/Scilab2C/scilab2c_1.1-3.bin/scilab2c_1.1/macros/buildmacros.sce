// ====================================================================
// Allan CORNET
// Simon LIPP
// INRIA 2008
// This file is released into the public domain
// ====================================================================
Directories = [ "ASTManagement", ...
		"CCodeGeneration", ...
		"ErrorMessages", ...
		"findDeps", ...
		"FunctionAnnotation", ...
		"FunctionList", ...
		"GeneralFunctions", ...
		"SymbolTable", ...
		"ToolInitialization" ];


current_path_buildmacros = get_absolute_file_path('buildmacros.sce');

for K=1:size(Directories,"*")
  if isdir( current_path_buildmacros + filesep() + Directories(K) ) then
    chdir( current_path_buildmacros + filesep() + Directories(K) );
    exec("buildmacros.sce");
    chdir(current_path_buildmacros);
  end
end

clear current_path_buildmacros;

tbx_build_macros(TOOLBOX_NAME, get_absolute_file_path('buildmacros.sce'));

clear tbx_build_macros;

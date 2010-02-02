defineTest(addExtraCompiler) {
    eval($${1}.commands = $$FAKE_COMMAND)
    eval($${1}.input = QREAL_XML)
	eval($${1}.output = $${2})
    eval($${1}.variable_out = GENERATED_RESOURCES)
	
    export($${1}.output)
    export($${1}.input)
    export($${1}.commands)
    export($${1}.variable_out)

    QMAKE_EXTRA_COMPILERS += $${1}

    export(QMAKE_EXTRA_COMPILERS)
	
    return(true)
}

SDF_FILES = $$files($$QREAL_EDITOR_NAME$$quote(\generated\shapes\*.sdf))

for(sdfFile,SDF_FILES) {
	FILE_NAME_LONG = $${sdfFile}
	COMPILER_NAME_LONG = $$basename(FILE_NAME_LONG)
	COMPILER_NAME = $$section(COMPILER_NAME_LONG,".",0,0)
	FILE_NAME = $$quote(generated/shapes/)$$COMPILER_NAME$$quote(.sdf)
	addExtraCompiler($$COMPILER_NAME, $$FILE_NAME)
}

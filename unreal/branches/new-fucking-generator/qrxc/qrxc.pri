win32 | win32-msvc* {
        CONFIG(debug, debug|release) {
                QMAKE_QRXC = $$PWD/debug/qrxc.exe
        } else:CONFIG(release, debug|release){
                QMAKE_QRXC = $$PWD/release/qrxc.exe
        } else {
                error(Windows build definitely needs to be fixed)
        }
} else {
        QMAKE_QRXC = $$PWD/qrxc
}

INPUT = $$QREAL_XML$$quote(.xml)

qrxc_source.commands = $$QMAKE_QRXC $$INPUT generated/${QMAKE_FILE_IN_BASE}.pro
qrxc_source.input = INPUT
qrxc_source.output = generated/pluginInterface.cpp
qrxc_source.variable_out = SOURCES
qrxc_source.name = QReal_XML_Compiler

QMAKE_EXTRA_COMPILERS += qrxc_source

qrxc_header.commands = $$FAKE
qrxc_header.input = INPUT
qrxc_header.output = generated/pluginInterface.h
qrxc_header.variable_out = HEADERS

QMAKE_EXTRA_COMPILERS += qrxc_header

qrxc_resource.commands = $$FAKE
qrxc_resource.input = INPUT
qrxc_resource.output = $$quote(generated/shapes/)$$QREAL_XML$$quote(.qrc)
qrxc_resource.variable_out = RESOURCES

QMAKE_EXTRA_COMPILERS += qrxc_resource

SDF_FILES = $$files($$quote(..\qrxml\)$$QREAL_XML$$quote(\generated\shapes\*.sdf))
#message($$SDF_FILES)


defineTest(addExtraCompiler) {
    eval($${1}.commands = $$FAKE)
    eval($${1}.input = INPUT)
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

for(sdfFile,SDF_FILES) {
	FILE_NAME_LONG = $${sdfFile}
	COMPILER_NAME_LONG = $$section(FILE_NAME_LONG,"/",1,1)
	COMPILER_NAME = $$section(COMPILER_NAME_LONG,".",0,0)
	FILE_NAME = $$quote(generated/shapes/)$$COMPILER_NAME$$quote(.sdf)

	addExtraCompiler($$COMPILER_NAME, $$FILE_NAME)
}

COMMAND = cd $$quote(..\qrxml\)$$QREAL_XML\ && $$QMAKE_QRXC $$QREAL_XML$$quote(.xml) $$quote(generated/)$$QREAL_XML$$quote(.pro)

!exists($$quote(..\qrxml\)$$QREAL_XML$$quote(\generated\pluginInterface.h)) {
	message($$COMMAND)
	SYS = $$system($$COMMAND) 
}

QRXML_ROOT = ..

!include (../editorsSdk.pri) {
	message("editorsSdk.pri not found at ../")
}

QT += xml

TEMPLATE =  lib
CONFIG += plugin
DESTDIR = ../../qrgui/plugins/
INCLUDEPATH += ../

OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .moc
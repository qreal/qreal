QT += xml widgets svg

CONFIG += console c++11
TARGET = editorPluginTestFramework

INCLUDEPATH += \
	$$PWD/../.. \
	$$PWD/../../qrgui/ \

DESTDIR = ../../bin

!macx {
	CONFIG += rpath_libdirs
	QMAKE_LFLAGS="-Wl,-O1,-rpath,$(PWD),-rpath,$(PWD)/../../bin"
}

RESOURCES = editorPluginTestingFramework.qrc

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

TRANSLATIONS = editorPluginTestingFramework_ru.ts

LIBS += -L../../bin -lqrrepo -lqrkernel -lqrutils -lqrmc

HEADERS += \
	mainClass.h \
	qrmcLauncher.h \
	pluginCompiler.h \
	pluginLoader.h \
	qrxcLauncher.h \
	../../qrgui/mainwindow/errorReporter.h \
	../../qrgui/mainwindow/error.h \
	../../plugins/metaEditor/metaEditorSupport/editorGenerator.h \
        ../../qrgui/plugins/pluginManager/interpreterEditorManager.h \
	../../qrgui/plugins/pluginManager/sdfRenderer.h \
	../../qrgui/plugins/pluginManager/details/interpreterElementImpl.h \
	../../qrgui/plugins/pluginManager/details/interpreterPortImpl.h \
	../../qrgui/plugins/pluginManager/explosion.h \
	../../qrgui/plugins/pluginManager/pattern.h \
	../../qrgui/plugins/pluginManager/editorManager.h \
	../../qrgui/plugins/pluginManager/details/patternParser.h \
	defs.h \
	htmlMaker.h \
	convertingMethods.h \
	methodsTesterForQrxcAndQrmc.h \
	methodsTester.h \
	methodsTesterForQrxcAndInterpreter.h \
	configurationFileParser.h \
	abstractStringGenerator.h \
	methodsCheckerForTravis.h

SOURCES += \
	mainClass.cpp \
	qrmcLauncher.cpp \
	main.cpp \
	pluginCompiler.cpp \
	pluginLoader.cpp \
	qrxcLauncher.cpp \
	../../qrgui/mainwindow/errorReporter.cpp \
	../../qrgui/mainwindow/error.cpp \
	../../plugins/metaEditor/metaEditorSupport/editorGenerator.cpp \
        ../../qrgui/plugins/pluginManager/interpreterEditorManager.cpp \
	../../qrgui/plugins/pluginManager/sdfRenderer.cpp \
	../../qrgui/plugins/pluginManager/details/interpreterElementImpl.cpp \
	../../qrgui/plugins/pluginManager/details/interpreterPortImpl.cpp \
	../../qrgui/plugins/pluginManager/explosion.cpp \
	../../qrgui/plugins/pluginManager/pattern.cpp \
	../../qrgui/plugins/pluginManager/editorManager.cpp \
	../../qrgui/plugins/pluginManager/details/patternParser.cpp \
	htmlMaker.cpp \
	convertingMethods.cpp \
	methodsTesterForQrxcAndQrmc.cpp \
	methodsTester.cpp \
	methodsTesterForQrxcAndInterpreter.cpp \
	configurationFileParser.cpp \
	methodsCheckerForTravis.cpp

win32 {
	QMAKE_POST_LINK = "cmd /C "xcopy methodsToTest ..\\..\\bin /s /e /q /y /i &&"\
						" copy configurationParameters.xml ..\\..\\bin /y &&"\
						" copy travisConfigurationParameters.xml ..\\..\\bin /y""
}
else {
	QMAKE_POST_LINK = " cp -r methodsToTest/* ../../bin/ &&"\
						" cp configurationParameters.xml ../../bin/ &&"\
						" cp travisConfigurationParameters.xml ../../bin/"
}

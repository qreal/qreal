QT += xml widgets

CONFIG += console
TARGET = editorPluginTestFramework

DESTDIR = ../../bin

!macx {
	CONFIG += rpath_libdirs
	QMAKE_LFLAGS="-Wl,-O1,-rpath,$(PWD),-rpath,$(PWD)/../../bin"
}

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
	methodsTester.h \
	qrxcLauncher.h \
	../../qrgui/mainwindow/errorReporter.h \
	../../qrgui/mainwindow/error.h \
	../../plugins/metaEditor/metaEditorSupport/editorGenerator.h \
	../../qrgui/pluginManager/interpreterEditorManager.h \
	../../qrgui/umllib/sdfRenderer.h \
	../../qrgui/pluginManager/interpreterElementImpl.h \
	../../qrgui/pluginManager/pattern.h \
	../../qrgui/pluginManager/editorManager.h \
	../../qrgui/pluginManager/patternParser.h \
#	../../qrgui/umllib/element.h \
#	../../qrgui/pluginManager/pattern.h \
#	../../qrgui/mainwindow/mainWindow.h \
	defs.h \
	htmlMaker.h \
    editorManagerMethodsTester.h

SOURCES += \
	mainClass.cpp \
	qrmcLauncher.cpp \
	main.cpp \
	pluginCompiler.cpp \
	pluginLoader.cpp \
	methodsTester.cpp \
	qrxcLauncher.cpp \
	../../qrgui/mainwindow/errorReporter.cpp \
	../../qrgui/mainwindow/error.cpp \
	../../plugins/metaEditor/metaEditorSupport/editorGenerator.cpp \
	../../qrgui/pluginManager/interpreterEditorManager.cpp \
	../../qrgui/umllib/sdfRenderer.cpp \
	../../qrgui/pluginManager/interpreterElementImpl.cpp \
	../../qrgui/pluginManager/pattern.cpp \
	../../qrgui/pluginManager/editorManager.cpp \
	../../qrgui/pluginManager/patternParser.cpp \
#	../../qrgui/umllib/element.cpp \
#	../../qrgui/pluginManager/pattern.cpp \
#	../../qrgui/mainwindow/mainWindow.cpp \
	htmlMaker.cpp \
    editorManagerMethodsTester.cpp








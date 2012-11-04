QT += xml

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








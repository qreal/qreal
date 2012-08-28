HEADERS += \
	mainwindow/mainWindow.h \
	mainwindow/propertyEditorProxyModel.h \
	mainwindow/errorReporter.h \
	mainwindow/gesturesPainterInterface.h \
	mainwindow/gesturesPainterInterface.h \
	mainwindow/error.h \
	mainwindow/errorListWidget.h \
	mainwindow/mainWindowInterpretersInterface.h \
	mainwindow/helpBrowser.h \
	mainwindow/paletteTree.h \
	mainwindow/findManager.h \
	mainwindow/dotRunner.h \
	mainwindow/splashScreen.h \
	mainwindow/projectManager/projectManagementInterface.h \
	mainwindow/projectManager/projectManager.h \
	mainwindow/tabWidget.h \
	mainwindow/miniMap.h \
	mainwindow/referenceList.h \
	mainwindow/projectManager/autosaver.h \
	mainwindow/externBrowser.h \

SOURCES += \
	mainwindow/mainWindow.cpp \
	mainwindow/propertyEditorProxyModel.cpp \
	mainwindow/errorReporter.cpp \
	mainwindow/error.cpp \
	mainwindow/errorListWidget.cpp \
	mainwindow/helpBrowser.cpp \
	mainwindow/paletteTree.cpp \
	mainwindow/findManager.cpp \
	mainwindow/dotRunner.cpp \
	mainwindow/splashScreen.cpp \
	mainwindow/projectManager/projectManager.cpp \
	mainwindow/tabWidget.cpp \
	mainwindow/miniMap.cpp \
	mainwindow/referenceList.cpp \
	mainwindow/projectManager/autosaver.cpp \
	mainwindow/macExternBrowser.cpp \

win32 {
	HEADERS += \
		mainwindow/windowsExternBrowser.h \

	SOURCES += \
		mainwindow/windowsExternBrowser.cpp \
}

unix {
	HEADERS += \
		mainwindow/linuxExternBrowser.h \

	SOURCES += \
		mainwindow/linuxExternBrowser.cpp \
}

macx {
	HEADERS += \
		mainwindow/macExternBrowser.h \

	SOURCES += \
		mainwindow/macExternBrowser.cpp \
}

FORMS += \
	mainwindow/mainWindow.ui \
	mainwindow/referenceList.ui

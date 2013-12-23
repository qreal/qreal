HEADERS += \
	$$PWD/mainWindow.h \
	$$PWD/propertyEditorProxyModel.h \
	$$PWD/errorReporter.h \
	$$PWD/gesturesPainterInterface.h \
	$$PWD/gesturesPainterInterface.h \
	$$PWD/error.h \
	$$PWD/errorListWidget.h \
	$$PWD/mainWindowInterpretersInterface.h \
	$$PWD/findManager.h \
	$$PWD/dotRunner.h \
	$$PWD/splashScreen.h \
	$$PWD/tabWidget.h \
	$$PWD/modelExplorer.h \
	$$PWD/miniMap.h \
	$$PWD/referenceList.h \
	$$PWD/externBrowser.h \
	$$PWD/qscintillaTextEdit.h \
	$$PWD/projectManager/autosaver.h \
	$$PWD/projectManager/projectManagementInterface.h \
	$$PWD/projectManager/projectManager.h \
	$$PWD/palette/paletteTree.h \
	$$PWD/palette/paletteTreeWidget.h \
	$$PWD/palette/draggableElement.h \
	$$PWD/palette/paletteTreeWidgets.h \
	$$PWD/palette/paletteElement.h \
	$$PWD/startWidget/suggestToCreateDiagramWidget.h \
	$$PWD/startWidget/startWidget.h \
	$$PWD/startWidget/recentProjectsListWidget.h \
	$$PWD/startWidget/listWidget.h \

SOURCES += \
	$$PWD/mainWindow.cpp \
	$$PWD/propertyEditorProxyModel.cpp \
	$$PWD/errorReporter.cpp \
	$$PWD/error.cpp \
	$$PWD/errorListWidget.cpp \
	$$PWD/findManager.cpp \
	$$PWD/dotRunner.cpp \
	$$PWD/splashScreen.cpp \
	$$PWD/tabWidget.cpp \
	$$PWD/miniMap.cpp \
	$$PWD/modelExplorer.cpp \
	$$PWD/referenceList.cpp \
	$$PWD/qscintillaTextEdit.cpp \
	$$PWD/projectManager/autosaver.cpp \
	$$PWD/projectManager/projectManager.cpp \
	$$PWD/palette/paletteTree.cpp \
	$$PWD/palette/paletteTreeWidget.cpp \
	$$PWD/palette/draggableElement.cpp \
	$$PWD/palette/paletteTreeWidgets.cpp \
	$$PWD/palette/paletteElement.cpp \
	$$PWD/startWidget/suggestToCreateDiagramWidget.cpp \
	$$PWD/startWidget/startWidget.cpp \
	$$PWD/startWidget/recentProjectsListWidget.cpp \
	$$PWD/startWidget/listWidget.cpp \

win32 {
	HEADERS += \
		$$PWD/windowsExternBrowser.h \

	SOURCES += \
		$$PWD/windowsExternBrowser.cpp \
}

unix {
	HEADERS += \
		$$PWD/linuxExternBrowser.h \

	SOURCES += \
		$$PWD/linuxExternBrowser.cpp \
}

macx {
	HEADERS += \
		$$PWD/macExternBrowser.h \

	SOURCES += \
		$$PWD/macExternBrowser.cpp \
}

FORMS += \
	$$PWD/mainWindow.ui \
	$$PWD/referenceList.ui \

HEADERS += \
	mainwindow/mainWindow.h \
	mainwindow/propertyEditorProxyModel.h \
	mainwindow/errorReporter.h \
	mainwindow/gesturesPainterInterface.h \
	mainwindow/gesturesPainterInterface.h \
	mainwindow/error.h \
	mainwindow/errorListWidget.h \
	mainwindow/mainWindowInterpretersInterface.h \
	mainwindow/findManager.h \
	mainwindow/dotRunner.h \
	mainwindow/splashScreen.h \
	mainwindow/tabWidget.h \
	mainwindow/miniMap.h \
	mainwindow/referenceList.h \
	mainwindow/externBrowser.h \
	mainwindow/qscintillaTextEdit.h \
	mainwindow/projectManager/autosaver.h \
	mainwindow/projectManager/projectManagementInterface.h \
	mainwindow/projectManager/projectManager.h \
	mainwindow/palette/paletteTree.h \
	mainwindow/palette/paletteTreeWidget.h \
	mainwindow/palette/draggableElement.h \
	mainwindow/palette/paletteTreeWidgets.h \

SOURCES += \
	mainwindow/mainWindow.cpp \
	mainwindow/propertyEditorProxyModel.cpp \
	mainwindow/errorReporter.cpp \
	mainwindow/error.cpp \
	mainwindow/errorListWidget.cpp \
	mainwindow/findManager.cpp \
	mainwindow/dotRunner.cpp \
	mainwindow/splashScreen.cpp \
	mainwindow/tabWidget.cpp \
	mainwindow/miniMap.cpp \
	mainwindow/referenceList.cpp \
	mainwindow/qscintillaTextEdit.cpp \
	mainwindow/projectManager/autosaver.cpp \
	mainwindow/projectManager/projectManager.cpp \
	mainwindow/palette/paletteTree.cpp \
	mainwindow/palette/paletteTreeWidget.cpp \
	mainwindow/palette/draggableElement.cpp \
	mainwindow/palette/paletteTreeWidgets.cpp \

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
	mainwindow/referenceList.ui \

# Shape editor
HEADERS += \
	mainwindow/shapeEdit/shapeEdit.h \
	mainwindow/shapeEdit/scene.h \
	mainwindow/shapeEdit/arch.h \
	mainwindow/shapeEdit/line.h \
	mainwindow/shapeEdit/item.h \
	mainwindow/shapeEdit/ellipse.h \
	mainwindow/shapeEdit/rectangle.h \
	mainwindow/shapeEdit/text.h \
	mainwindow/shapeEdit/pointPort.h \
	mainwindow/shapeEdit/linePort.h \
	mainwindow/shapeEdit/xmlLoader.h \
	mainwindow/shapeEdit/stylus.h \
	mainwindow/shapeEdit/colorListEditor.h \
	mainwindow/shapeEdit/curve.h \
	mainwindow/shapeEdit/view.h \
	mainwindow/shapeEdit/textPicture.h \
	mainwindow/shapeEdit/path.h \
	mainwindow/shapeEdit/image.h \
	mainwindow/shapeEdit/visibilityConditionsDialog.h \
	mainwindow/shapeEdit/visibilityValueWidget.h \

SOURCES += \
	mainwindow/shapeEdit/shapeEdit.cpp \
	mainwindow/shapeEdit/scene.cpp \
	mainwindow/shapeEdit/arch.cpp \
	mainwindow/shapeEdit/line.cpp \
	mainwindow/shapeEdit/item.cpp \
	mainwindow/shapeEdit/ellipse.cpp \
	mainwindow/shapeEdit/rectangle.cpp \
	mainwindow/shapeEdit/text.cpp \
	mainwindow/shapeEdit/pointPort.cpp \
	mainwindow/shapeEdit/linePort.cpp \
	mainwindow/shapeEdit/xmlLoader.cpp \
	mainwindow/shapeEdit/stylus.cpp \
	mainwindow/shapeEdit/colorListEditor.cpp \
	mainwindow/shapeEdit/path.cpp \
	mainwindow/shapeEdit/curve.cpp \
	mainwindow/shapeEdit/view.cpp \
	mainwindow/shapeEdit/textPicture.cpp \
	mainwindow/shapeEdit/image.cpp \
	mainwindow/shapeEdit/visibilityConditionsDialog.cpp \
	mainwindow/shapeEdit/visibilityValueWidget.cpp \

FORMS += \
	mainwindow/shapeEdit/shapeEdit.ui \
	mainwindow/shapeEdit/visibilityConditionsDialog.ui \

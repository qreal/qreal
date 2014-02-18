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
	$$PWD/filterObject.h \
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
	$$PWD/startWidget/circleWidget.h \
	$$PWD/startWidget/styledButton.h \

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
	$$PWD/filterObject.cpp \
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
	$$PWD/startWidget/circleWidget.cpp \
	$$PWD/startWidget/styledButton.cpp \

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

# Shape editor
HEADERS += \
	$$PWD/shapeEdit/shapeEdit.h \
	$$PWD/shapeEdit/scene.h \
	$$PWD/shapeEdit/arch.h \
	$$PWD/shapeEdit/line.h \
	$$PWD/shapeEdit/item.h \
	$$PWD/shapeEdit/ellipse.h \
	$$PWD/shapeEdit/rectangle.h \
	$$PWD/shapeEdit/text.h \
	$$PWD/shapeEdit/pointPort.h \
	$$PWD/shapeEdit/linePort.h \
	$$PWD/shapeEdit/xmlLoader.h \
	$$PWD/shapeEdit/stylus.h \
	$$PWD/shapeEdit/colorListEditor.h \
	$$PWD/shapeEdit/curve.h \
	$$PWD/shapeEdit/view.h \
	$$PWD/shapeEdit/textPicture.h \
	$$PWD/shapeEdit/path.h \
	$$PWD/shapeEdit/image.h \
	$$PWD/shapeEdit/visibilityConditionsDialog.h \
	$$PWD/shapeEdit/visibilityValueWidget.h \

SOURCES += \
	$$PWD/shapeEdit/shapeEdit.cpp \
	$$PWD/shapeEdit/scene.cpp \
	$$PWD/shapeEdit/arch.cpp \
	$$PWD/shapeEdit/line.cpp \
	$$PWD/shapeEdit/item.cpp \
	$$PWD/shapeEdit/ellipse.cpp \
	$$PWD/shapeEdit/rectangle.cpp \
	$$PWD/shapeEdit/text.cpp \
	$$PWD/shapeEdit/pointPort.cpp \
	$$PWD/shapeEdit/linePort.cpp \
	$$PWD/shapeEdit/xmlLoader.cpp \
	$$PWD/shapeEdit/stylus.cpp \
	$$PWD/shapeEdit/colorListEditor.cpp \
	$$PWD/shapeEdit/path.cpp \
	$$PWD/shapeEdit/curve.cpp \
	$$PWD/shapeEdit/view.cpp \
	$$PWD/shapeEdit/textPicture.cpp \
	$$PWD/shapeEdit/image.cpp \
	$$PWD/shapeEdit/visibilityConditionsDialog.cpp \
	$$PWD/shapeEdit/visibilityValueWidget.cpp \

FORMS += \
	$$PWD/shapeEdit/shapeEdit.ui \
	$$PWD/shapeEdit/visibilityConditionsDialog.ui \

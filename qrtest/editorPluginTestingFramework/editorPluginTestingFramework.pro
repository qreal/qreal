include(../../global.pri)
QT += xml widgets svg printsupport

CONFIG += console c++11
TARGET = editorPluginTestFramework

INCLUDEPATH += \
	$$PWD/../.. \
	$$PWD/../../qrgui/ \

DESTDIR = ../../bin/debug

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

HEADERS += \
	mainClass.h \
	qrmcLauncher.h \
	pluginCompiler.h \
	pluginLoader.h \
	qrxcLauncher.h \
	../../qrgui/mainwindow/projectManager/projectManagerWrapper.h\
	../../qrgui/mainwindow/palette/paletteTree.h\
	../../qrgui/mainwindow/palette/paletteTreeWidgets.h\
	../../qrgui/mainwindow/palette/paletteTreeWidget.h\
	../../qrgui/mainwindow/palette/paletteElement.h\
	../../qrgui/mainwindow/splashScreen.h \
	../../qrgui/mainWindow/shapeEdit/shapeEdit.h \
	../../qrgui/mainWindow/shapeEdit/scene.h \
	../../qrgui/mainWindow/shapeEdit/arch.h \
	../../qrgui/mainWindow/shapeEdit/colorListEditor.h \
	../../qrgui/mainWindow/shapeEdit/curve.h \
	../../qrgui/mainWindow/shapeEdit/ellipse.h \
	../../qrgui/mainWindow/shapeEdit/image.h \
	../../qrgui/mainWindow/shapeEdit/item.h \
	../../qrgui/mainWindow/shapeEdit/line.h \
	../../qrgui/mainWindow/shapeEdit/linePort.h \
	../../qrgui/mainWindow/shapeEdit/path.h \
	../../qrgui/mainWindow/shapeEdit/pointPort.h \
	../../qrgui/mainWindow/shapeEdit/rectangle.h \
	../../qrgui/mainWindow/shapeEdit/stylus.h \
	../../qrgui/mainWindow/shapeEdit/text.h \
	../../qrgui/mainWindow/shapeEdit/textPicture.h \
	../../qrgui/mainWindow/shapeEdit/view.h \
	../../qrgui/mainWindow/shapeEdit/visibilityConditionsDialog.h \
	../../qrgui/mainWindow/shapeEdit/visibilityValueWidget.h \
	../../qrgui/mainWindow/shapeEdit/xmlLoader.h \
	../../qrgui/mainWindow/referenceList.h \
	../../qrgui/mainWindow/miniMap.h \
	../../qrgui/mainWindow/dotRunner.h \
	../../qrgui/mainWindow/startWidget/startWidget.h \
	../../qrgui/mainWindow/startWidget/circleWidget.h \
	../../qrgui/mainWindow/startWidget/styledButton.h \
	../../qrgui/mainWindow/tabWidget.h \
	../../qrgui/mainwindow/findManager.h \
	../../qrgui/mainwindow/palette/draggableElement.h \
	../../qrgui/mainwindow/modelExplorer.h \
	../../qrgui/mainwindow/qrealApplication.h  \
	../../qrgui/mainwindow/projectManager/versionsConverterManager.h \
	../../qrgui/mainwindow/mainWindow.h \
	../../qrgui/mainwindow/errorReporter.h \
	../../qrgui/mainwindow/error.h \
	../../qrgui/mainWindow/errorListWidget.h \
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
	../../qrgui/mainwindow/projectManager/projectManagerWrapper.cpp\
	../../qrgui/mainwindow/palette/paletteTree.cpp\
	../../qrgui/mainwindow/palette/paletteTreeWidgets.cpp\
	../../qrgui/mainwindow/palette/paletteTreeWidget.cpp\
	../../qrgui/mainwindow/palette/paletteElement.cpp\
	../../qrgui/mainwindow/palette/draggableElement.cpp \
	../../qrgui/mainwindow/qrealApplication.cpp  \
	../../qrgui/mainWindow/shapeEdit/shapeEdit.cpp \
	../../qrgui/mainWindow/shapeEdit/scene.cpp \
	../../qrgui/mainWindow/shapeEdit/arch.cpp \
	../../qrgui/mainWindow/shapeEdit/colorListEditor.cpp \
	../../qrgui/mainWindow/shapeEdit/curve.cpp \
	../../qrgui/mainWindow/shapeEdit/ellipse.cpp \
	../../qrgui/mainWindow/shapeEdit/image.cpp \
	../../qrgui/mainWindow/shapeEdit/item.cpp \
	../../qrgui/mainWindow/shapeEdit/line.cpp \
	../../qrgui/mainWindow/shapeEdit/linePort.cpp \
	../../qrgui/mainWindow/shapeEdit/path.cpp \
	../../qrgui/mainWindow/shapeEdit/pointPort.cpp \
	../../qrgui/mainWindow/shapeEdit/rectangle.cpp \
	../../qrgui/mainWindow/shapeEdit/stylus.cpp \
	../../qrgui/mainWindow/shapeEdit/text.cpp \
	../../qrgui/mainWindow/shapeEdit/textPicture.cpp \
	../../qrgui/mainWindow/shapeEdit/view.cpp \
	../../qrgui/mainWindow/shapeEdit/visibilityConditionsDialog.cpp \
	../../qrgui/mainWindow/shapeEdit/visibilityValueWidget.cpp \
	../../qrgui/mainWindow/shapeEdit/xmlLoader.cpp \
	../../qrgui/mainWindow/referenceList.cpp \
	../../qrgui/mainWindow/miniMap.cpp \
	../../qrgui/mainWindow/dotRunner.cpp \
	../../qrgui/mainWindow/startWidget/startWidget.cpp \
	../../qrgui/mainWindow/startWidget/circleWidget.cpp \
	../../qrgui/mainwindow/splashScreen.cpp \
	../../qrgui/mainWindow/startWidget/styledButton.cpp \
	../../qrgui/mainWindow/tabWidget.cpp \
	../../qrgui/mainwindow/findManager.cpp \
	../../qrgui/mainwindow/modelExplorer.cpp \
	../../qrgui/mainwindow/projectManager/versionsConverterManager.cpp \
	../../qrgui/mainwindow/mainWindow.cpp \
	../../qrgui/mainwindow/errorReporter.cpp \
	../../qrgui/mainwindow/error.cpp \
	../../qrgui/mainWindow/errorListWidget.cpp \
	../../plugins/metaEditor/metaEditorSupport/editorGenerator.cpp \
#        ../../qrgui/plugins/pluginManager/interpreterEditorManager.cpp \
	../../qrgui/plugins/pluginManager/sdfRenderer.cpp \
	../../qrgui/plugins/pluginManager/details/interpreterElementImpl.cpp \
	../../qrgui/plugins/pluginManager/details/interpreterPortImpl.cpp \
#	../../qrgui/plugins/pluginManager/explosion.cpp \
#	../../qrgui/plugins/pluginManager/pattern.cpp \
#	../../qrgui/plugins/pluginManager/editorManager.cpp \
	../../qrgui/plugins/pluginManager/details/patternParser.cpp \
	htmlMaker.cpp \
	convertingMethods.cpp \
	methodsTesterForQrxcAndQrmc.cpp \
	methodsTester.cpp \
	methodsTesterForQrxcAndInterpreter.cpp \
	configurationFileParser.cpp \
	methodsCheckerForTravis.cpp

links (qrrepo qrtext qrkernel qrutils qrmc qrgui-plugin-manager qrgui-editor qrgui-thirdparty qrgui-tool-plugin-interface qscintilla2 qrgui-models qrgui-controller qrgui-dialogs qrgui-preferences-dialog qrgui-text-editor qrgui-mouse-gestures qrgui-hotkey-manager qrgui-brand-manager qrgui-facade qslog)                        

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

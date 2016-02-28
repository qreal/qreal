# Copyright 2007-2015 QReal Research Group
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

include(../../global.pri)
include(../../qrgui/mainWindow/scriptAPI/scriptAPI.pri)
QT += xml widgets svg printsupport

CONFIG += console
TEMPLATE = app

INCLUDEPATH += \
	$$PWD/../.. \
	$$PWD/../../qrgui/ \
	$$PWD/../../qrgui/mainWindow/ \

!macx {
	QMAKE_LFLAGS += -Wl,-O1,-rpath,.
	QMAKE_LFLAGS += -Wl,-rpath-link,$$DESTDIR
}

RESOURCES = editorPluginTestingFramework.qrc

TRANSLATIONS = editorPluginTestingFramework_ru.ts

HEADERS += \
	mainClass.h \
	qrmcLauncher.h \
	pluginCompiler.h \
	pluginLoader.h \
	qrxcLauncher.h \
	../../qrgui/mainWindow/projectManager/projectManagerWrapper.h\
	../../qrgui/mainWindow/palette/paletteTree.h\
	../../qrgui/mainWindow/palette/paletteTreeWidgets.h\
	../../qrgui/mainWindow/palette/paletteTreeWidget.h\
	../../qrgui/mainWindow/palette/paletteElement.h\
	../../qrgui/mainWindow/splashScreen.h \
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
	../../qrgui/mainWindow/findManager.h \
	../../qrgui/mainWindow/palette/draggableElement.h \
	../../qrgui/mainWindow/modelExplorer.h \
	../../qrgui/mainWindow/qrealApplication.h  \
	../../qrgui/mainWindow/projectManager/versionsConverterManager.h \
	../../qrgui/mainWindow/mainWindow.h \
	../../qrgui/mainWindow/errorReporter.h \
	../../qrgui/mainWindow/error.h \
	../../qrgui/mainWindow/errorListWidget.h \
	../../plugins/metaEditor/metaEditorSupport/editorGenerator.h \
	../../qrgui/plugins/pluginManager/interpreterEditorManager.h \
	../../qrgui/plugins/pluginManager/sdfRenderer.h \
	../../qrgui/plugins/pluginManager/details/interpreterElementImpl.h \
	../../qrgui/plugins/pluginManager/explosion.h \
	../../qrgui/plugins/pluginManager/pattern.h \
	../../qrgui/plugins/pluginManager/editorManager.h \
	../../qrgui/plugins/pluginManager/editorManagerInterface.h \
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
	../../qrgui/mainWindow/projectManager/projectManagerWrapper.cpp\
	../../qrgui/mainWindow/palette/paletteTree.cpp\
	../../qrgui/mainWindow/palette/paletteTreeWidgets.cpp\
	../../qrgui/mainWindow/palette/paletteTreeWidget.cpp\
	../../qrgui/mainWindow/palette/paletteElement.cpp\
	../../qrgui/mainWindow/palette/draggableElement.cpp \
	../../qrgui/mainWindow/qrealApplication.cpp  \
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
	../../qrgui/mainWindow/splashScreen.cpp \
	../../qrgui/mainWindow/startWidget/styledButton.cpp \
	../../qrgui/mainWindow/tabWidget.cpp \
	../../qrgui/mainWindow/findManager.cpp \
	../../qrgui/mainWindow/modelExplorer.cpp \
	../../qrgui/mainWindow/projectManager/versionsConverterManager.cpp \
	../../qrgui/mainWindow/mainWindow.cpp \
	../../qrgui/mainWindow/errorReporter.cpp \
	../../qrgui/mainWindow/error.cpp \
	../../qrgui/mainWindow/errorListWidget.cpp \
	../../plugins/metaEditor/metaEditorSupport/editorGenerator.cpp \
	../../qrgui/plugins/pluginManager/details/interpreterElementImpl.cpp \
	../../qrgui/plugins/pluginManager/details/patternParser.cpp \
	htmlMaker.cpp \
	convertingMethods.cpp \
	methodsTesterForQrxcAndQrmc.cpp \
	methodsTester.cpp \
	methodsTesterForQrxcAndInterpreter.cpp \
	configurationFileParser.cpp \
	methodsCheckerForTravis.cpp

FORMS += \
	../../qrgui/mainWindow/mainWindow.ui \
	../../qrgui/mainWindow/shapeEdit/shapeEdit.ui \
	../../qrgui/mainWindow/shapeEdit/visibilityConditionsDialog.ui \
	../../qrgui/mainWindow/referenceList.ui \


links (qrrepo qrtext qrkernel qrutils qrmc qrgui-plugin-manager qrgui-editor qrgui-thirdparty qrgui-tool-plugin-interface qscintilla2 qrgui-models qrgui-controller qrgui-dialogs qrgui-preferences-dialog qrgui-text-editor qrgui-mouse-gestures qrgui-hotkey-manager qrgui-brand-manager qrgui-facade qslog)

copyToDestdir(methodsToTest)
copyToDestdir(configurationParameters.xml)
copyToDestdir(travisConfigurationParameters.xml)

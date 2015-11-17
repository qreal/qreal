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

TARGET = qreal

include(../../global.pri)

TEMPLATE = app

QT += widgets printsupport xml svg

links(qrkernel qslog qrutils qrtext qrrepo qscintilla2 qrgui-models qrgui-editor qrgui-controller qrgui-dialogs qrgui-preferences-dialog \
		qrgui-text-editor qrgui-mouse-gestures qrgui-hotkey-manager qrgui-brand-manager  \
		qrgui-facade qrgui-plugin-manager qrgui-tool-plugin-interface qrgui-thirdparty \
)

includes(qrgui)

TRANSLATIONS = \
	$$PWD/../../qrtranslations/ru/qrgui_mainWindow_ru.ts \
	$$PWD/../../qrtranslations/fr/qrgui_mainWindow_fr.ts \

!macx {
	QMAKE_LFLAGS="-Wl,-O1,-rpath,."
}

HEADERS += \
	$$PWD/qrealApplication.h \
	$$PWD/mainWindow.h \
	$$PWD/errorReporter.h \
	$$PWD/error.h \
	$$PWD/errorListWidget.h \
	$$PWD/findManager.h \
	$$PWD/dotRunner.h \
	$$PWD/splashScreen.h \
	$$PWD/tabWidget.h \
	$$PWD/modelExplorer.h \
	$$PWD/miniMap.h \
	$$PWD/referenceList.h \
	$$PWD/externBrowser.h \
	$$PWD/projectManager/projectManagerWrapper.h \
	$$PWD/projectManager/versionsConverterManager.h \
	$$PWD/palette/paletteTree.h \
	$$PWD/palette/paletteTreeWidget.h \
	$$PWD/palette/draggableElement.h \
	$$PWD/palette/paletteTreeWidgets.h \
	$$PWD/palette/paletteElement.h \
	$$PWD/startWidget/startWidget.h \
	$$PWD/startWidget/circleWidget.h \
	$$PWD/startWidget/styledButton.h \
    shapeEdit/item/arch.h \
    shapeEdit/item/curve.h \
    shapeEdit/item/ellipse.h \
    shapeEdit/item/image.h \
    shapeEdit/item/item.h \
    shapeEdit/item/line.h \
    shapeEdit/item/linePort.h \
    shapeEdit/item/path.h \
    shapeEdit/item/pointPort.h \
    shapeEdit/item/rectangle.h \
    shapeEdit/item/stylus.h \
    shapeEdit/item/text.h \
    shapeEdit/item/textPicture.h \
    shapeEdit/iShapeEdit.h \
    shapeEdit/propertyInfo.h \
    shapeEdit/saveLoadLogic.h \
    shapeEdit/saveLoadLogicForInterpreter.h \
    shapeEdit/saveLoadLogicUsingModel.h \
    shapeEdit/visibilityCondition.h \
    shapeEdit/item/createItemPushButton.h \
    shapeEdit/commands/addItemCommand.h \
    shapeEdit/commands/resizeItemCommand.h

SOURCES += \
	$$PWD/main.cpp \
	$$PWD/qrealApplication.cpp \
	$$PWD/mainWindow.cpp \
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
	$$PWD/projectManager/projectManagerWrapper.cpp \
	$$PWD/projectManager/versionsConverterManager.cpp \
	$$PWD/palette/paletteTree.cpp \
	$$PWD/palette/paletteTreeWidget.cpp \
	$$PWD/palette/draggableElement.cpp \
	$$PWD/palette/paletteTreeWidgets.cpp \
	$$PWD/palette/paletteElement.cpp \
	$$PWD/startWidget/startWidget.cpp \
	$$PWD/startWidget/circleWidget.cpp \
	$$PWD/startWidget/styledButton.cpp \
    shapeEdit/item/arch.cpp \
    shapeEdit/item/curve.cpp \
    shapeEdit/item/ellipse.cpp \
    shapeEdit/item/image.cpp \
    shapeEdit/item/item.cpp \
    shapeEdit/item/line.cpp \
    shapeEdit/item/linePort.cpp \
    shapeEdit/item/path.cpp \
    shapeEdit/item/pointPort.cpp \
    shapeEdit/item/rectangle.cpp \
    shapeEdit/item/stylus.cpp \
    shapeEdit/item/text.cpp \
    shapeEdit/item/textPicture.cpp \
    shapeEdit/saveLoadLogic.cpp \
    shapeEdit/saveLoadLogicForInterpreter.cpp \
    shapeEdit/saveLoadLogicUsingModel.cpp \
    shapeEdit/item/createItemPushButton.cpp \
    shapeEdit/commands/addItemCommand.cpp \
    shapeEdit/commands/resizeItemCommand.cpp

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

FORMS += \
	$$PWD/mainWindow.ui \
        $$PWD/referenceList.ui

# Shape editor
HEADERS += \
	$$PWD/shapeEdit/shapeEdit.h \
        $$PWD/shapeEdit/scene.h \
        $$PWD/shapeEdit/xmlLoader.h \
        $$PWD/shapeEdit/colorListEditor.h \
	$$PWD/shapeEdit/visibilityConditionsDialog.h \
	$$PWD/shapeEdit/visibilityValueWidget.h \

SOURCES += \
	$$PWD/shapeEdit/shapeEdit.cpp \
        $$PWD/shapeEdit/scene.cpp \
        $$PWD/shapeEdit/xmlLoader.cpp \
        $$PWD/shapeEdit/colorListEditor.cpp \
	$$PWD/shapeEdit/visibilityConditionsDialog.cpp \
	$$PWD/shapeEdit/visibilityValueWidget.cpp \

FORMS += \
	$$PWD/shapeEdit/shapeEdit.ui \
	$$PWD/shapeEdit/visibilityConditionsDialog.ui \

RESOURCES = $$PWD/mainWindow.qrc

win32 {
	RC_FILE = $$PWD/mainWindow.rc
}

macx {
	ICON = icon.icns
}

include(scriptAPI/scriptAPI.pri)


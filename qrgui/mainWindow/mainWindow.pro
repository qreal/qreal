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

includes(qrgraph qrgui qrgui/plugins/metaMetaModel)

TRANSLATIONS = \
	$$PWD/../../qrtranslations/ru/qrgui_mainWindow_ru.ts \
	$$PWD/../../qrtranslations/fr/qrgui_mainWindow_fr.ts \

!macx {
	QMAKE_LFLAGS+="-Wl,-O1,-rpath,."
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

RESOURCES = $$PWD/mainWindow.qrc

win32 {
	RC_FILE = $$PWD/mainWindow.rc
}

macx {
	ICON = icon.icns
}

include(scriptAPI/scriptAPI.pri)


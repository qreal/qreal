# Copyright 2015 QReal Research Group
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

include(../commonGui.pri) #because "SOURCES -=" doesnt work. (bug?)

QT += script

HEADERS += \
	$$PWD/QRealGuiTests.h \
	$$PWD/workaroundTestFunctions.h \
	$$PWD/TestAgent.h \

SOURCES += \
	$$PWD/QRealGuiTests.cpp \
	$$PWD/startQreal.cpp \
	$$PWD/mainGuiTest.cpp \
	$$PWD/workaroundTestFunctions.cpp \
	$$PWD/TestAgent.cpp \


# in Existence checks initial states of required elements
# in Functioning checks the correct work of required elements after some preparing actions
# in other scripts checks the correct work of the complicated sequence of actions
OTHER_FILES += \
	$$PWD/testScripts/qrealScripts/editActionsExistence.js \
	$$PWD/testScripts/qrealScripts/editActionsFunctioning.js \
	$$PWD/testScripts/qrealScripts/fileActionsExistence.js \
	$$PWD/testScripts/qrealScripts/fileActionsFunctioning.js \
	$$PWD/testScripts/qrealScripts/findDialogElementsExistence.js \
	$$PWD/testScripts/qrealScripts/findDialogElementsFunctioning.js \
	$$PWD/testScripts/qrealScripts/helpActionsExistence.js \
	$$PWD/testScripts/qrealScripts/helpActionsFunctioning.js \
	$$PWD/testScripts/qrealScripts/dockWidgetsExistence.js \
	$$PWD/testScripts/qrealScripts/dockWidgetsFunctioning.js \
	$$PWD/testScripts/qrealScripts/mouseGesturesElementsExistence.js \
	$$PWD/testScripts/qrealScripts/preferenceDialogElementsExistence.js \
	$$PWD/testScripts/qrealScripts/preferenceDialogElementsFunctioning.js \
	$$PWD/testScripts/qrealScripts/settingsActionsExistence.js \
	$$PWD/testScripts/qrealScripts/createRootElementOnSceneAndQuit.js \
	$$PWD/testScripts/qrealScripts/toolbarsElementsExistence.js \
	$$PWD/testScripts/qrealScripts/toolbarsElementsFunctioning.js \
	$$PWD/testScripts/qrealScripts/toolsActionsExistence.js \
	$$PWD/testScripts/qrealScripts/toolsActionsFunctioning.js \
	$$PWD/testScripts/qrealScripts/viewActionsExistence.js \
	$$PWD/testScripts/qrealScripts/viewActionsFunctioning.js \
	$$PWD/testScripts/qrealScripts/fullscreenModeScript.js \
	$$PWD/testScripts/qrealScripts/propertyEditorScript.js \
	$$PWD/testScripts/qrealScripts/linkNodeCooperationScript.js \
	$$PWD/testScripts/qrealScripts/logicalGraphicalModelsAndSceneInteraction.js \
	$$PWD/testScripts/qrealScripts/paletteSearchScript.js \
	$$PWD/testScripts/qrealScripts/paletteAndEditorPropertiesConcordance.js \
	$$PWD/testScripts/qrealScripts/miniMapWorkScript.js \
	$$PWD/testScripts/qrealScripts/rightclickAllScreenScript.js \
	$$PWD/testScripts/qrealScripts/autoSaveWork.js \
	$$PWD/testScripts/qrealScripts/saveOpenScript.js \
	$$PWD/testScripts/qrealScripts/hotKeysExistenceAndFunctioning.js \
	$$PWD/testScripts/qrealScripts/createDiagramInCurrentModel.js \
	$$PWD/testScripts/qrealScripts/verySimpleShapeEditorTest.js \
	$$PWD/testScripts/common.js \

copyToDestdir($$PWD/testScripts/qrealScripts/, now)
copyToDestdir($$PWD/testScripts/common.js, now)

include(../../../qrgui/mainWindow/mainWindowForTests.pri)

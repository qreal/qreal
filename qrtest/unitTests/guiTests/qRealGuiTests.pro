# Copyright 2014 - 2015 CyberTech Labs Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

#     http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

include(../common.pri)

QT += script

HEADERS += \
	$$PWD/QRealGuiTests.h \

SOURCES += \
	$$PWD/QRealGuiTests.cpp \
	$$PWD/startQreal.cpp \

OTHER_FILES += \
	$$PWD/testScripts/qrealScripts/editActionsExistence.js \
	$$PWD/testScripts/qrealScripts/editActionsFunctioning.js \
	$$PWD/testScripts/qrealScripts/fileActionsExistence.js \
	$$PWD/testScripts/qrealScripts/fileActionsFunctioning.js \
	$$PWD/testScripts/qrealScripts/findDialogElementsExistence.js \
	$$PWD/testScripts/qrealScripts/findDialogElementsFunctioning.js \
	$$PWD/testScripts/qrealScripts/helpActionsExistence.js \
	$$PWD/testScripts/qrealScripts/helpActionsFunctioning.js \
	$$PWD/testScripts/qrealScripts/mainPanelsElementsExistence.js \
	$$PWD/testScripts/qrealScripts/mainPanelsElementsFunctioning.js \
	$$PWD/testScripts/qrealScripts/mouseGesturesElementsExistence.js \
	$$PWD/testScripts/qrealScripts/mouseGesturesElementsFunctioning.js \
	$$PWD/testScripts/qrealScripts/preferenceDialogElementsExistence.js \
	$$PWD/testScripts/qrealScripts/preferenceDialogElementsFunctioning.js \
	$$PWD/testScripts/qrealScripts/settingsActionsExistence.js \
	$$PWD/testScripts/qrealScripts/settingsActionsFunctioning.js \
	$$PWD/testScripts/qrealScripts/tabSceneExistence.js \
	$$PWD/testScripts/qrealScripts/tabSceneFunctioning.js \
	$$PWD/testScripts/qrealScripts/toolbarsElementsExistence.js \
	$$PWD/testScripts/qrealScripts/toolbarsElementsFunctioning.js \
	$$PWD/testScripts/qrealScripts/toolsActionsExistence.js \
	$$PWD/testScripts/qrealScripts/toolsActionsFunctioning.js \
	$$PWD/testScripts/qrealScripts/viewActionsExistence.js \
	$$PWD/testScripts/qrealScripts/viewActionsFunctioning.js \
	$$PWD/testScripts/qrealScripts/script1.js \
	$$PWD/testScripts/qrealScripts/script2.js \
	$$PWD/testScripts/qrealScripts/script3.js \
	$$PWD/testScripts/qrealScripts/script4.js \
	$$PWD/testScripts/qrealScripts/script5.js \
	$$PWD/testScripts/qrealScripts/script6.js \
	$$PWD/testScripts/qrealScripts/script7.js \
	$$PWD/testScripts/qrealScripts/script8.js \
	$$PWD/testScripts/qrealScripts/script9.js \
	$$PWD/testScripts/qrealScripts/script10.js \
	$$PWD/testScripts/qrealScripts/hotKeysExistanceAndFunctioning.js \

copyToDestdir($$PWD/testScripts/, now)

include(../../../qrgui/mainWindow/mainWindowForTests.pri)

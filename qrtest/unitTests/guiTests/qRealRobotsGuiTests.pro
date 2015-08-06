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
	$$PWD/QRealRobotsGuiTests.h \

SOURCES += \
	$$PWD/QRealGuiTests.cpp \
	$$PWD/startQreal.cpp \
	$$PWD/QRealRobotsGuiTests.cpp \

OTHER_FILES += \
	$$PWD/testScripts/qrealRobotsScripts/editActionsExistence.js \
	$$PWD/testScripts/qrealRobotsScripts/editActionsFunctioning.js \
	$$PWD/testScripts/qrealRobotsScripts/fileActionsExistence.js \
	$$PWD/testScripts/qrealRobotsScripts/fileActionsFunctioning.js \
	$$PWD/testScripts/qrealRobotsScripts/findDialogElementsExistence.js \
	$$PWD/testScripts/qrealRobotsScripts/findDialogElementsFunctioning.js \
	$$PWD/testScripts/qrealRobotsScripts/helpActionsExistence.js \
	$$PWD/testScripts/qrealRobotsScripts/helpActionsFunctioning.js \
	$$PWD/testScripts/qrealRobotsScripts/mainPanelsElementsExistence.js \
	$$PWD/testScripts/qrealRobotsScripts/mainPanelsElementsFunctioning.js \
	$$PWD/testScripts/qrealRobotsScripts/mouseGesturesElementsExistence.js \
	$$PWD/testScripts/qrealRobotsScripts/mouseGesturesElementsFunctioning.js \
	$$PWD/testScripts/qrealRobotsScripts/preferenceDialogElementsExistence.js \
	$$PWD/testScripts/qrealRobotsScripts/preferenceDialogElementsFunctioning.js \
	$$PWD/testScripts/qrealRobotsScripts/settingsActionsExistence.js \
	$$PWD/testScripts/qrealRobotsScripts/settingsActionsFunctioning.js \
	$$PWD/testScripts/qrealRobotsScripts/tabSceneExistence.js \
	$$PWD/testScripts/qrealRobotsScripts/tabSceneFunctioning.js \
	$$PWD/testScripts/qrealRobotsScripts/toolbarsElementsExistence.js \
	$$PWD/testScripts/qrealRobotsScripts/toolbarsElementsFunctioning.js \
	$$PWD/testScripts/qrealRobotsScripts/toolsActionsExistence.js \
	$$PWD/testScripts/qrealRobotsScripts/toolsActionsFunctioning.js \
	$$PWD/testScripts/qrealRobotsScripts/viewActionsExistence.js \
	$$PWD/testScripts/qrealRobotsScripts/viewActionsFunctioning.js \
	$$PWD/testScripts/qrealRobotsScripts/script1.js \
	$$PWD/testScripts/qrealRobotsScripts/script2.js \
	$$PWD/testScripts/qrealRobotsScripts/script3.js \
	$$PWD/testScripts/qrealRobotsScripts/script4.js \
	$$PWD/testScripts/qrealRobotsScripts/script5.js \
	$$PWD/testScripts/qrealRobotsScripts/script6.js \
	$$PWD/testScripts/qrealRobotsScripts/script7.js \
	$$PWD/testScripts/qrealRobotsScripts/script8.js \
	$$PWD/testScripts/qrealRobotsScripts/script9.js \
	$$PWD/testScripts/qrealRobotsScripts/script10.js \
	$$PWD/testScripts/qrealRobotsScripts/hotKeysExistanceAndFunctioning.js \

copyToDestdir($$PWD/testScripts/qrealRobotsScripts/, now)

include(../../../qrgui/mainWindow/mainWindowForTests.pri)

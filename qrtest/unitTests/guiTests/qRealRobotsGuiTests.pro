# Copyright 2015 QReal Research Group
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

include(../commonGui.pri) #because "SOURCES -=" doesn't work. (bug?)

QT += script

HEADERS += \
	$$PWD/qRealRobotsGuiTests.h \
	$$PWD/qRealGuiTests.h \
	$$PWD/testAgent.h \
	$$PWD/workaroundTestFunctions.h \

SOURCES += \
	$$PWD/qRealRobotsGuiTests.cpp \
	$$PWD/qRealGuiTests.cpp \
	$$PWD/startQreal.cpp \
	$$PWD/testAgent.cpp \
	$$PWD/workaroundTestFunctions.cpp \
	$$PWD/mainRobotsGuiTest.cpp

OTHER_FILES += \
	$$PWD/testScripts/qrealRobotsScripts/alongTheLineTest.js \
	$$PWD/testScripts/common.js \

copyToDestdir($$PWD/testScripts/qrealRobotsScripts/, now)
copyToDestdir($$PWD/testScripts/common.js, now)

include(../../../qrgui/mainWindow/mainWindow.pri)

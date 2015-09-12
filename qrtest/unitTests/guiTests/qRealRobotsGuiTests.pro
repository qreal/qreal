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

include(../commonGui.pri) #because "SOURCES -=" doesnt work. (bug)

SOURCES += $$PWD/mainGuiTest.cpp

QT += script

HEADERS += \
	$$PWD/QRealRobotsGuiTests.h \
	$$PWD/QRealGuiTests.h \

SOURCES += \
	$$PWD/QRealRobotsGuiTests.cpp \

OTHER_FILES += \
	$$PWD/testScripts/qrealRobotsScripts/alongTheLineTest.js \
	$$PWD/testScripts/common.js \

copyToDestdir($$PWD/testScripts/qrealRobotsScripts/, now)
copyToDestdir($$PWD/testScripts/common.js, now)

include(../../../qrgui/mainWindow/mainWindow.pri)

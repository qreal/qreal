# Copyright 2007-2016 QReal Research Group, CyberTech Labs Ltd.
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

TARGET = test-utils

include($$PWD/../common.pri)

DEFINES += TEST_UTILS_LIBRARY

TEMPLATE = lib

HEADERS += \
	$$PWD/include/testUtils/declSpec.h \
	$$PWD/include/testUtils/delay.h \
	$$PWD/include/testUtils/fakeSender.h \
	$$PWD/include/testUtils/qrguiFacade.h \
	$$PWD/include/testUtils/signalsTester.h \
	$$PWD/include/testUtils/testRegistry.h \
	$$PWD/include/testUtils/wait.h \

SOURCES += \
	$$PWD/src/delay.cpp \
	$$PWD/src/qrguiFacade.cpp \
	$$PWD/src/signalsTester.cpp \
	$$PWD/src/testRegistry.cpp \
	$$PWD/src/wait.cpp \

# Mocks
HEADERS += \
	$$PWD/../mocks/qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterfaceMock.h \
	$$PWD/../mocks/qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowDockInterfaceMock.h \
	$$PWD/../mocks/qrgui/mainWindow/projectManager/projectManagementInterfaceMock.h \
	$$PWD/../mocks/qrgui/view/sceneCustomizationInterfaceMock.h \
	$$PWD/../mocks/qrgui/textEditor/textManagerInterfaceMock.h \
	$$PWD/../mocks/qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterMock.h \

links(qrgui-tool-plugin-interface qrgui-models qrgui-text-editor qrkernel qrgui-plugin-manager)

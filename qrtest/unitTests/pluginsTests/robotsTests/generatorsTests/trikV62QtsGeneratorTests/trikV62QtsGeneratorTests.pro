# Copyright 2016 CyberTech Labs Ltd.
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

TARGET = trik-v62-qts-generator-tests

include($$PWD/../../../../common.pri)

include($$PWD/../../../../../../plugins/robots/generators/trik/trikV62QtsGenerator/trikV62QtsGenerator.pri)

links(test-utils qrgui-tool-plugin-interface qrgui-models qrgui-plugin-manager qrgui-text-editor qrtext \
		qrgui-controller tcp-robot-simulator)

INCLUDEPATH += \
	$$PWD/../../../../../../plugins/robots/generators/trik/trikV62QtsGenerator \
	$$PWD/../../../../../../plugins/robots/generators/trik/trikV62QtsGenerator \
	$$PWD/../../tcpRobotSimulator/include \

HEADERS += \
	$$PWD/trikV62QtsGeneratorTest.h \

SOURCES += \
	$$PWD/trikV62QtsGeneratorTest.cpp \

HEADERS += \
	$$PWD/../../../../mocks/plugins/robots/common/kitBase/include/kitBase/robotModel/robotModelManagerInterfaceMock.h \
	$$PWD/../../../../mocks/plugins/robots/common/kitBase/include/kitBase/robotModel/interpreterControlInterfaceMock.h \
	$$PWD/../../../../mocks/qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterMock.h \
	$$PWD/../../support/testRobotModel.h \

copyToDestdir($$PWD/support/testData/unittests, NOW)

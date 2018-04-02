# Copyright 2018 QReal Research Group
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

TARGET = semantic-tree-tests

include($$PWD/../../../../common.pri)
include($$PWD/../../../../../../plugins/robots/generators/generatorBase/generatorBase.pri)

links(qrgui-plugin-manager test-utils robots-trik-generator-base robots-utils)

includes(plugins/robots/interpreters \
	plugins/robots/interpreters/interpreterCore \
	plugins/robots/common/kitBase \
	qrtest/unitTests/mocks/plugins/robots/common/kitBase \
	qrtest/unitTests/mocks/plugins/robots/interpreters)

QT += xml

HEADERS += \
	$$PWD/semanticTreeTest.h \


SOURCES += \
	$$PWD/semanticTreeTest.cpp \

HEADERS += \
	$$PWD/../../../../mocks/plugins/robots/common/kitBase/include/kitBase/robotModel/robotModelManagerInterfaceMock.h \
#	$$PWD/../../../../mocks/plugins/robots/common/kitBase/include/kitBase/robotModel/robotModelInterfaceMock.h \
#	$$PWD/../../../../../../plugins/robots/utils/include/utils/timelineInterface/h

copyToDestdir($$PWD/diagrams, NOW)

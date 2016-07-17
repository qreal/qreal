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

TARGET = robots_interpreterCore_unittests

include(../../../../common.pri)

include(../../../../../../plugins/robots/interpreters/interpreterCore/interpreterCore.pri)

links(qrkernel qslog test-utils qrgui-text-editor)

includes(plugins/robots/interpreters \
	plugins/robots/interpreters/interpreterCore \
	plugins/robots/common/kitBase \
	qrtest/unitTests/mocks/plugins/robots/common/kitBase \
	qrtest/unitTests/mocks/plugins/robots/interpreters)

# Tests
HEADERS += \
	kitPluginManagerTest.h \
	interpreterTests/interpreterTest.h \
	interpreterTests/detailsTests/blocksTableTest.h \
	managersTests/sensorsConfigurationManagerTest.h \
	support/dummySensorsConfigurer.h \

SOURCES += \
	kitPluginManagerTest.cpp \
	interpreterTests/interpreterTest.cpp \
	interpreterTests/detailsTests/blocksTableTest.cpp \
	managersTests/sensorsConfigurationManagerTest.cpp \
	support/dummySensorsConfigurer.cpp \

# Mocks
include(mocks.pri)

# Supporting code
HEADERS += \
	support/dummyBlock.h \
	support/dummyBlocksFactory.h \

SOURCES += \
	support/dummyBlock.cpp \
	support/dummyBlocksFactory.cpp \

copyToDestdir(../support/testData/unittests, NOW)

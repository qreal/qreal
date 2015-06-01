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

TARGET = robots_kitBase_unittests

include(../../../../common.pri)

include(../../../../../../plugins/robots/common/kitBase/kitBase.pri)

links(qslog)

INCLUDEPATH += \
	../../../../../../plugins/robots/common/kitBase \
	../../../../../../plugins/robots/common/kitBase/include \

# Tests
HEADERS += \
	robotModelTests/defaultRobotModelTest.h \
	robotModelTests/commonRobotModelTest.h \
	robotModelTests/configurationTest.h \
	robotModelTests/robotPartsTests/deviceTest.h \
	robotModelTests/deviceInfoTest.h \

SOURCES += \
	robotModelTests/defaultRobotModelTest.cpp \
	robotModelTests/commonRobotModelTest.cpp \
	robotModelTests/configurationTest.cpp \
	robotModelTests/robotPartsTests/deviceTest.cpp \
	robotModelTests/deviceInfoTest.cpp \

# Support classes
HEADERS += \
	support/dummyDevice.h \

SOURCES += \
	support/dummyDevice.cpp \

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

TEMPLATE = app
CONFIG += console

include(../../global.pri)

QT += widgets

SOURCES = $$PWD/mainTest.cpp

if (equals(QMAKE_CXX, "g++") : !macx) {
	QMAKE_LFLAGS += -Wl,-E
	# For gmock
	QMAKE_CXXFLAGS += -Wno-unused-local-typedefs
}

INCLUDEPATH += \
	$$PWD/ \
	$$PWD/../../ \
	$$PWD/../thirdparty/gmock-1.6.0/include \
	$$PWD/../thirdparty/gmock-1.6.0/gtest/include \

links(gmock)

include($$PWD/utils/utils.pri)
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

TARGET = qrgui-thirdparty

include(../../global.pri)

TEMPLATE = lib

DEFINES += QRGUI_THIRDPARTY_LIBRARY

TRANSLATIONS = $$PWD/../../qrtranslations/ru/qrgui_thirdparty_ru.ts

# Some warnings are turned off because of problems specific to thirdparty code.
QMAKE_CXXFLAGS += -Wno-switch

win32 {
	HEADERS += \
		$$PWD/windowsmodernstyle.h \

	SOURCES += \
		$$PWD/windowsmodernstyle.cpp \
}

HEADERS += \
	$$PWD/thirdPartyDeclSpec.h \
	$$PWD/qsKineticScroller.h \

SOURCES += \
	$$PWD/qsKineticScroller.cpp \

include($$PWD/qtpropertybrowser/src/qtpropertybrowser.pri)

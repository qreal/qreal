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

TARGET = qrgui-brand-manager

include(../../global.pri)

HEADERS += \
	$$PWD/brandManagerDeclSpec.h \
	$$PWD/brandManager.h \
	$$PWD/fonts.h \
	$$PWD/styles.h \

SOURCES += \
	$$PWD/brandManager.cpp \

RESOURCES += \
	$$PWD/fonts/fonts.qrc \
	$$PWD/styles/styles.qrc \

TEMPLATE = lib

QT += widgets

DEFINES += QRGUI_BRAND_MANAGER_LIBRARY

links(qrkernel qrutils qrgui-plugin-manager)
includes(qrgui)


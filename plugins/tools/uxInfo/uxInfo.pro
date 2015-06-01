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

include(../../../global.pri)

TEMPLATE = lib
CONFIG += plugin 
DESTDIR = $$DESTDIR/plugins/tools/

links(qrkernel qslog qrgui-preferences-dialog qrgui-tool-plugin-interface)

INCLUDEPATH += \
	$$PWD/../../../ \
	$$PWD/../../../qrgui/ \

QT += widgets

TRANSLATIONS = $$PWD/../../../qrtranslations/ru/plugins/uxInfo_ru.ts

HEADERS = \
	$$PWD/uxInfoPlugin.h \
	$$PWD/uxInfo.h \
	$$PWD/filterObject.h \

SOURCES = \
	$$PWD/uxInfoPlugin.cpp \
	$$PWD/uxInfo.cpp \
	$$PWD/filterObject.cpp \

RESOURCES += \
	$$PWD/uxInfo.qrc \

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

QT += xml widgets

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$DESTDIR/plugins/tools/

INCLUDEPATH += \
	$$PWD/../../.. \
	$$PWD/../../../qrgui/ \
	$$PWD/../../../qrtext/include/ \

links(qrkernel qrutils qrgui-preferences-dialog qrgui-tool-plugin-interface qrtext)

TRANSLATIONS = $$PWD/../../../qrtranslations/ru/plugins/deploymentInterpreter_ru.ts

RESOURCES = deploymentInterpreter.qrc

HEADERS += \
	$$PWD/deploymentInterpreterPlugin.h \
	$$PWD/blocksTable.h \
	$$PWD/shellWidget.h \
	$$PWD/blocks/shellBlock.h \
	$$PWD/blocks/incrementVersionInSourceCodeBlock.h \
	$$PWD/blocks/helpWithChangelogBlock.h \
	$$PWD/blocks/buildOnVirtualMachineBlock.h \
	$$PWD/blocks/uploadToGoogleDriveBlock.h \
	$$PWD/blocks/updateDownloadsCounterBlock.h \
	$$PWD/blocks/uploadToGoogleSiteBlock.h \

SOURCES += \
	$$PWD/deploymentInterpreterPlugin.cpp \
	$$PWD/blocksTable.cpp \
	$$PWD/shellWidget.cpp \
	$$PWD/blocks/shellBlock.cpp \
	$$PWD/blocks/incrementVersionInSourceCodeBlock.cpp \
	$$PWD/blocks/helpWithChangelogBlock.cpp \
	$$PWD/blocks/buildOnVirtualMachineBlock.cpp \
	$$PWD/blocks/uploadToGoogleDriveBlock.cpp \
	$$PWD/blocks/updateDownloadsCounterBlock.cpp \
	$$PWD/blocks/uploadToGoogleSiteBlock.cpp \

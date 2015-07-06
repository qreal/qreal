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

TARGET = qrgui-facade

include(../../global.pri)

TEMPLATE = lib

QT += widgets

links(qrkernel qslog qrutils qrgui-models qrgui-plugin-manager qrgui-text-editor qrgui-tool-plugin-interface)

TRANSLATIONS = \
	$$PWD/../../qrtranslations/ru/qrgui_system_facade_ru.ts \
	$$PWD/../../qrtranslations/fr/qrgui_system_facade_fr.ts \

DEFINES += QRGUI_SYSTEM_FACADE_LIBRARY

HEADERS += \
	$$PWD/systemFacadeDeclSpec.h \
	$$PWD/systemFacade.h \
	$$PWD/components/nullErrorReporter.h \
	$$PWD/components/consoleErrorReporter.h \
	$$PWD/components/nullTextManager.h \
	$$PWD/components/projectManager.h \
	$$PWD/components/autosaver.h \
	$$PWD/components/nullMainWindow.h \

SOURCES += \
	$$PWD/systemFacade.cpp \
	$$PWD/components/nullErrorReporter.cpp \
	$$PWD/components/consoleErrorReporter.cpp \
	$$PWD/components/nullTextManager.cpp \
	$$PWD/components/projectManager.cpp \
	$$PWD/components/autosaver.cpp \
	$$PWD/components/nullMainWindow.cpp \

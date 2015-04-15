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

QT += xml script widgets

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$DESTDIR/plugins/tools/

links(qrkernel qrutils qrgui-preferences-dialog)

INCLUDEPATH += \
	$$PWD/../../.. \
	$$PWD/../../../qrgui/ \
	$$PWD/../../../qrtext/include/ \

TRANSLATIONS = $$PWD/../../../qrtranslations/ru/plugins/visualInterpreter_ru.ts

HEADERS = \
	visualInterpreterPlugin.h \
	visualInterpreterPreferencesPage.h \
	visualInterpreterUnit.h \
	textualPart/ruleParser.h \
	textualPart/pythonInterpreter.h \
	textualPart/pythonGenerator.h \
	textualPart/textCodeGenerator.h \
	textualPart/textCodeInterpreter.h \
	textualPart/qtScriptGenerator.h \
	textualPart/qtScriptInterpreter.h

SOURCES = \
	visualInterpreterPlugin.cpp \
	visualInterpreterPreferencesPage.cpp \
	visualInterpreterUnit.cpp \
	textualPart/ruleParser.cpp \
	textualPart/pythonInterpreter.cpp \
	textualPart/pythonGenerator.cpp \
	textualPart/textCodeGenerator.cpp \
	textualPart/textCodeInterpreter.cpp \
	textualPart/qtScriptGenerator.cpp \
	textualPart/qtScriptInterpreter.cpp

FORMS += \
	visualInterpreterPreferencePage.ui \

RESOURCES += \
	visualInterpreter.qrc \

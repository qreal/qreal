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

TARGET = qrgui-text-editor

include(../../global.pri)

TEMPLATE = lib

links(qrkernel qrutils qscintilla2 qrgui-tool-plugin-interface)

QT += widgets

DEFINES += QRGUI_TEXT_EDITOR_LIBRARY

TRANSLATIONS = \
	$$PWD/../../qrtranslations/ru/qrgui_textEditor_ru.ts \
	$$PWD/../../qrtranslations/fr/qrgui_textEditor_fr.ts \

INCLUDEPATH = \
	$$PWD/ \
	$$PWD/../ \
	$$PWD/../../ \
	$$PWD/../../thirdparty/qscintilla/Qt4Qt5 \

HEADERS += \
	$$PWD/textEditorDeclSpec.h \
	$$PWD/textHighlighter.h \
	$$PWD/sqlHighlighter.h \
	$$PWD/textManager.h \
	$$PWD/textManagerInterface.h \
	$$PWD/qscintillaTextEdit.h \
	$$PWD/languageInfo.h \

SOURCES += \
	$$PWD/textHighlighter.cpp \
	$$PWD/sqlHighlighter.cpp \
	$$PWD/textManager.cpp \
	$$PWD/qscintillaTextEdit.cpp \
	$$PWD/languageInfo.cpp \

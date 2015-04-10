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

QT += widgets

links(qrkernel qslog qrtext)

INCLUDEPATH += \
	$$PWD/../.. \
	$$PWD/../../qrgui/ \
	$$PWD/../../qrtext \
	$$PWD/../../qrtext/include \

HEADERS += \
	$$PWD/generationRulesPlugin.h \
	$$PWD/lexer.h \
	$$PWD/tokenTypes.h \
	$$PWD/parser.h \
	$$PWD/ast/program.h \
	$$PWD/ast/node.h \
	$$PWD/ast/text.h \
	$$PWD/ast/identifier.h \
	$$PWD/ast/newline.h \
	$$PWD/ast/foreach.h \
	$$PWD/ast/complexIdentifier.h \
	$$PWD/dialogs/specifyGenerationRulesDialog.h \

SOURCES += \
	$$PWD/generationRulesPlugin.cpp \
	$$PWD/lexer.cpp \
	$$PWD/parser.cpp \
	$$PWD/dialogs/specifyGenerationRulesDialog.cpp \

FORMS += \
	$$PWD/dialogs/specifyGenerationRulesDialog.ui \

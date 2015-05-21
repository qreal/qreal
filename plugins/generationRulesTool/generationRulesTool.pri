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
	$$PWD \
	$$PWD/.. \
	$$PWD/../.. \
	$$PWD/../../qrgui/ \
	$$PWD/../../qrtext \
	$$PWD/../../qrtext/include \

HEADERS += \
	$$PWD/generationRulesPlugin.h \
	$$PWD/lexer.h \
	$$PWD/tokenTypes.h \
	$$PWD/parser.h \
	$$PWD/callGeneratorFor.h \
	$$PWD/ast/program.h \
	$$PWD/ast/node.h \
	$$PWD/ast/text.h \
	$$PWD/ast/identifier.h \
	$$PWD/ast/newline.h \
	$$PWD/ast/foreach.h \
	$$PWD/ast/complexIdentifier.h \
	$$PWD/ast/incomingLinks.h \
	$$PWD/ast/links.h \
	$$PWD/ast/outcomingLinks.h \
	$$PWD/ast/tab.h \
	$$PWD/ast/transitionEnd.h \
	$$PWD/ast/callGeneratorFor.h \
	$$PWD/dialogs/specifyGenerationRulesDialog.h \
	$$PWD/generator/commonGenerator.h \
	$$PWD/generator/generatorForComplexIdentifierNode.h \
	$$PWD/generator/generatorForForeachNode.h \
	$$PWD/generator/generatorForProgramNode.h \
	$$PWD/generator/variablesTable.h \
	$$PWD/generator/auxiliaryGenerators/outcomingLinksListGenerator.h \
	$$PWD/generator/auxiliaryGenerators/simpleTypeListGenerator.h \
	$$PWD/generator/generatorForCallGenerator.h \
	$$PWD/treeGeneratorFromString.h \
	$$PWD/ast/generator.h \
	$$PWD/generator/generatorForGeneratorNode.h \
	$$PWD/ast/list.h \
	$$PWD/ast/transitionStart.h \
	$$PWD/ast/elementIdentifier.h \
	$$PWD/generator/generatorForElementIdentifierNode.h \
	$$PWD/generator/auxiliaryGenerators/incomingLinksListGenerator.h \
	$$PWD/generator/auxiliaryGenerators/listGenerator.h \
	$$PWD/generator/generatorConfigurer.h \
	$$PWD/generator/auxiliaryGenerators/identifierTypeGenerator.h \
	$$PWD/generator/typeQualifier.h \
	$$PWD/ast/this.h \
	$$PWD/generator/currentScope.h \
	$$PWD/dialogs/specifyPathToGeneratedCodeDialog.h \
	$$PWD/ast/generateToFile.h \
	$$PWD/generator/generatorForGenerateToFile.h \
	$$PWD/ast/string.h \
	$$PWD/generator/auxiliaryGenerators/stringGenerator.h \
	$$PWD/appearance/ruleHighlighter.h \
	$$PWD/ast/equal.h \
	$$PWD/ast/notEqual.h \
	$$PWD/ast/condition.h \
	$$PWD/ast/ifNode.h \
	$$PWD/generator/generatorForIfNode.h \
	$$PWD/generator/auxiliaryGenerators/generatorForCondition.h \
	$$PWD/ast/separator.h

SOURCES += \
	$$PWD/generationRulesPlugin.cpp \
	$$PWD/lexer.cpp \
	$$PWD/parser.cpp \
	$$PWD/dialogs/specifyGenerationRulesDialog.cpp \
	$$PWD/generator/commonGenerator.cpp \
	$$PWD/generator/generatorForComplexIdentifierNode.cpp \
	$$PWD/generator/generatorForForeachNode.cpp \
	$$PWD/generator/generatorForProgramNode.cpp \
	$$PWD/generator/variablesTable.cpp \
	$$PWD/generator/auxiliaryGenerators/outcomingLinksListGenerator.cpp \
	$$PWD/generator/auxiliaryGenerators/simpleTypeListGenerator.cpp \
	$$PWD/generator/generatorForCallGenerator.cpp \
	$$PWD/treeGeneratorFromString.cpp \
	$$PWD/generator/generatorForGeneratorNode.cpp \
	$$PWD/generator/generatorForElementIdentifierNode.cpp \
	$$PWD/generator/auxiliaryGenerators/incomingLinksListGenerator.cpp \
	$$PWD/generator/auxiliaryGenerators/listGenerator.cpp \
	$$PWD/generator/auxiliaryGenerators/identifierTypeGenerator.cpp \
	$$PWD/generator/typeQualifier.cpp \
	$$PWD/generator/currentScope.cpp \
	$$PWD/dialogs/specifyPathToGeneratedCodeDialog.cpp \
	$$PWD/generator/generatorForGenerateToFile.cpp \
	$$PWD/generator/auxiliaryGenerators/stringGenerator.cpp \
	$$PWD/appearance/ruleHighlighter.cpp \
	$$PWD/generator/generatorForIfNode.cpp \
	$$PWD/generator/auxiliaryGenerators/generatorForCondition.cpp \

FORMS += \
	$$PWD/dialogs/specifyGenerationRulesDialog.ui \
	$$PWD/dialogs/specifyPathToGeneratedCodeDialog.ui

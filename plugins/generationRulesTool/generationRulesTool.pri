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
	$$PWD/ast/partOfComplexIdentifier.h \
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
	$$PWD/treeGeneratorFromString.h

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
	$$PWD/treeGeneratorFromString.cpp

FORMS += \
	$$PWD/dialogs/specifyGenerationRulesDialog.ui \

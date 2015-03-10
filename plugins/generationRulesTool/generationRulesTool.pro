include(../../global.pri)

TEMPLATE = lib
CONFIG += plugin 
DESTDIR = $$DESTDIR/plugins/interpreted/

INCLUDEPATH += \
	$$PWD/../.. \
	$$PWD/../../qrgui/ \
	$$PWD/../../qrtext \
	$$PWD/../../qrtext/include \

links(qrkernel qslog qrtext)

QT += widgets

HEADERS = \
	generationRulesPlugin.h \
	lexer.h \
	tokenTypes.h \
	parser.h \
	ast/program.h \
	ast/node.h \
	ast/text.h \
	ast/identifier.h \
	ast/newline.h \
	ast/foreach.h \
	ast/complexIdentifier.h \
	generator/commonGenerator.h \
	generator/generatorForProgramNode.h \
	generator/generatorForForeachNode.h \
	generator/generatorForComplexIdentifierNode.h

SOURCES = \
	generationRulesPlugin.cpp \
	lexer.cpp \
	parser.cpp \
	generator/commonGenerator.cpp \
	generator/generatorForProgramNode.cpp \
	generator/generatorForForeachNode.cpp \
	generator/generatorForComplexIdentifierNode.cpp

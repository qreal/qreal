include(../../global.pri)

TEMPLATE = lib
CONFIG += plugin 
DESTDIR = $$DESTDIR/plugins/interpreted/

INCLUDEPATH += \
	$$PWD/../.. \
	$$PWD/../../qrgui/ \
	$$PWD/../../qrtext \
	$$PWD/../../qrtext/include \

LIBS += -L$$PWD/../../bin -lqrkernel -lqslog -lqrtext

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

SOURCES = \
	generationRulesPlugin.cpp \
	lexer.cpp \
	parser.cpp \

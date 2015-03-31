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

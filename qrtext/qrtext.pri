CONFIG += c++11

INCLUDEPATH += \
	$$PWD/include/ \
	$$PWD/.. \

LIBS += -L$$PWD/../bin -lqrkernel -lqslog

DEFINES += QRTEXT_LIBRARY

RESOURCES = $$PWD/qrtext.qrc

HEADERS += \
	$$PWD/include/qrtext/declSpec.h \
	$$PWD/include/qrtext/core/error.h \
	$$PWD/include/qrtext/core/connection.h \
	$$PWD/include/qrtext/core/range.h \
	$$PWD/include/qrtext/core/ast/node.h \
	$$PWD/include/qrtext/core/lexer/lexer.h \
	$$PWD/include/qrtext/core/lexer/token.h \
	$$PWD/include/qrtext/core/lexer/tokenPatterns.h \
	$$PWD/include/qrtext/core/parser/parser.h \
	$$PWD/include/qrtext/core/parser/parserContext.h \
	$$PWD/include/qrtext/core/parser/tokenStream.h \
	$$PWD/include/qrtext/core/parser/operators/parserCombinators.h \
	$$PWD/include/qrtext/core/parser/operators/tokenParser.h \
	$$PWD/include/qrtext/core/parser/operators/simpleParser.h \
	$$PWD/include/qrtext/core/parser/operators/parserInterface.h \
	$$PWD/include/qrtext/core/parser/operators/alternativeParser.h \
	$$PWD/include/qrtext/core/parser/operators/transformingParser.h \
	$$PWD/include/qrtext/core/parser/operators/concatenationParser.h \
	$$PWD/include/qrtext/core/parser/operators/optionalParser.h \
	$$PWD/include/qrtext/core/parser/operators/kleeneStarParser.h \
	$$PWD/include/qrtext/core/parser/operators/expressionParser.h \
	$$PWD/include/qrtext/lua/luaToolbox.h \
	$$PWD/include/qrtext/lua/ast/unaryOperator.h \
	$$PWD/include/qrtext/lua/ast/binaryOperator.h \
	$$PWD/include/qrtext/lua/ast/number.h \
	$$PWD/include/qrtext/lua/ast/unaryMinus.h \
	$$PWD/include/qrtext/lua/ast/not.h \
	$$PWD/include/qrtext/lua/ast/bitwiseNegation.h \
	$$PWD/include/qrtext/lua/ast/length.h \
	$$PWD/include/qrtext/lua/ast/addition.h \
	$$PWD/include/qrtext/lua/ast/subtraction.h \
	$$PWD/include/qrtext/lua/ast/multiplication.h \
	$$PWD/include/qrtext/lua/ast/division.h \
	$$PWD/include/qrtext/lua/ast/integerDivision.h \
	$$PWD/include/qrtext/lua/ast/modulo.h \
	$$PWD/include/qrtext/lua/ast/exponentiation.h \
	$$PWD/include/qrtext/lua/ast/bitwiseAnd.h \
	$$PWD/include/qrtext/lua/ast/bitwiseOr.h \
	$$PWD/include/qrtext/lua/ast/bitwiseXor.h \
	$$PWD/include/qrtext/lua/ast/bitwiseLeftShift.h \
	$$PWD/include/qrtext/lua/ast/bitwiseRightShift.h \
	$$PWD/include/qrtext/lua/ast/concatenation.h \
	$$PWD/include/qrtext/lua/ast/equality.h \
	$$PWD/include/qrtext/lua/ast/lessThan.h \
	$$PWD/include/qrtext/lua/ast/lessOrEqual.h \
	$$PWD/include/qrtext/lua/ast/inequality.h \
	$$PWD/include/qrtext/lua/ast/greaterThan.h \
	$$PWD/include/qrtext/lua/ast/greaterOrEqual.h \
	$$PWD/include/qrtext/lua/ast/integerNumber.h \
	$$PWD/include/qrtext/lua/ast/floatNumber.h \
	$$PWD/include/qrtext/lua/ast/fieldInitialization.h \
	$$PWD/include/qrtext/lua/ast/expression.h \
	$$PWD/include/qrtext/lua/ast/tableConstructor.h \
	$$PWD/include/qrtext/lua/ast/integerNumber.h \
	$$PWD/include/qrtext/lua/ast/floatNumber.h \
	$$PWD/include/qrtext/lua/ast/string.h \
	$$PWD/include/qrtext/lua/ast/true.h \
	$$PWD/include/qrtext/lua/ast/false.h \
	$$PWD/include/qrtext/lua/ast/nil.h \
	$$PWD/include/qrtext/lua/ast/identifier.h \
	$$PWD/include/qrtext/lua/ast/functionCall.h \
	$$PWD/include/qrtext/lua/ast/methodCall.h \
	$$PWD/include/qrtext/lua/ast/assignment.h \
	$$PWD/include/qrtext/lua/ast/block.h \
	$$PWD/include/qrtext/lua/ast/indexingExpression.h \
	$$PWD/src/lua/luaParser.h \
	$$PWD/src/lua/luaLexer.h \
	$$PWD/src/lua/luaTokenTypes.h \

SOURCES += \
	$$PWD/src/core/connection.cpp \
	$$PWD/src/core/error.cpp \
	$$PWD/src/lua/luaToolbox.cpp \
	$$PWD/src/lua/luaParser.cpp \
	$$PWD/src/lua/luaLexer.cpp \

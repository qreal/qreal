HEADERS += \
	$$PWD/tokenPatterns.h \
	$$PWD/parserError.h \
	$$PWD/textLanguageParser.h \
	$$PWD/textLanguageParserInterface.h \
	$$PWD/tokenType.h \
	$$PWD/ast/connection.h \
	$$PWD/ast/node.h \
	$$PWD/ast/range.h \
	$$PWD/ast/unaryOperator.h \
	$$PWD/ast/binaryOperator.h \
	$$PWD/ast/temporaryToken.h \
	$$PWD/ast/temporaryPair.h \
	$$PWD/ast/number.h \
	$$PWD/details/lexer.h \
	$$PWD/details/token.h  \
	$$PWD/details/tokenStream.h  \
	$$PWD/details/parserCombinators.h \
	$$PWD/details/simpleParser.h \
	$$PWD/details/parserInterface.h \
	$$PWD/details/alternativeParser.h \
	$$PWD/details/transformingParser.h \
	$$PWD/details/concatenationParser.h \
	$$PWD/details/functionTraits.h \
	$$PWD/details/parserRef.h \

SOURCES += \
	$$PWD/tokenPatterns.cpp \
	$$PWD/parserError.cpp \
	$$PWD/textLanguageParser.cpp \
	$$PWD/ast/connection.cpp \
	$$PWD/ast/node.cpp \
	$$PWD/ast/range.cpp \
	$$PWD/details/lexer.cpp \
	$$PWD/details/token.cpp \
	$$PWD/details/tokenStream.cpp  \

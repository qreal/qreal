#pragma once

#include <type_traits>
#include <QtCore/QHash>

namespace textLanguageParser {

enum class TokenType {
	whitespace
	, newline
	, identifier

	, andKeyword
	, breakKeyword
	, doKeyword
	, elseKeyword
	, elseifKeyword
	, endKeyword
	, falseKeyword
	, forKeyword
	, functionKeyword
	, gotoKeyword
	, ifKeyword
	, inKeyword
	, localKeyword
	, nilKeyword
	, notKeyword
	, orKeyword
	, repeatKeyword
	, returnKeyword
	, thenKeyword
	, trueKeyword
	, untilKeyword
	, whileKeyword

	, plus
	, minus
	, asterick
	, slash
	, percent
	, hat
	, sharp

	, ampersand
	, tilda
	, verticalLine
	, doubleLess
	, doubleGreater
	, doubleSlash

	, doubleEquals
	, tildaEquals
	, lessEquals
	, greaterEquals
	, less
	, greater
	, equals

	, openingBracket
	, closingBracket
	, openingCurlyBracket
	, closingCurlyBracket
	, openingSquareBracket
	, closingSquareBracket
	, doubleColon

	, semicolon
	, colon
	, comma
	, dot
	, doubleDot
	, tripleDot

	, string
	, integerLiteral
	, floatLiteral
	, comment
};

inline uint qHash(TokenType const &key)
{
	return ::qHash(static_cast<std::underlying_type<TokenType>::type>(key));
}

}

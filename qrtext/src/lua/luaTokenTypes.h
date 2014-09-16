#pragma once

#include <type_traits>
#include <QtCore/QHash>
#include <QtCore/QDebug>

namespace qrtext {
namespace lua {
namespace details {

/// Tokens recognizable by Lua lexer.
enum class LuaTokenTypes {
	whitespace  // 0
	, newline
	, identifier

	, andKeyword
	, breakKeyword
	, doKeyword  // 5
	, elseKeyword
	, elseifKeyword
	, endKeyword
	, falseKeyword
	, forKeyword  // 10
	, functionKeyword
	, gotoKeyword
	, ifKeyword
	, inKeyword
	, localKeyword  //15
	, nilKeyword
	, notKeyword
	, orKeyword
	, repeatKeyword
	, returnKeyword  // 20
	, thenKeyword
	, trueKeyword
	, untilKeyword
	, whileKeyword

	, plus  // 25
	, minus
	, asterick
	, slash
	, percent
	, hat  // 30
	, sharp

	, ampersand
	, tilda
	, verticalLine
	, doubleLess  // 35
	, doubleGreater
	, doubleSlash

	, doubleEquals
	, tildaEquals
	, exclamationMarkEquals  // 40
	, lessEquals
	, greaterEquals
	, less
	, greater
	, equals  // 45

	, openingBracket
	, closingBracket
	, openingCurlyBracket
	, closingCurlyBracket
	, openingSquareBracket  // 50
	, closingSquareBracket
	, doubleColon

	, semicolon
	, colon
	, comma  // 55
	, dot
	, doubleDot
	, tripleDot

	, string
	, integerLiteral  // 60
	, floatLiteral
	, comment
};

/// Overload that allows to use token types as hash keys.
inline uint qHash(LuaTokenTypes const &key)
{
	return ::qHash(static_cast<std::underlying_type<LuaTokenTypes>::type>(key));
}

/// Helper operator for debugging.
inline QDebug operator <<(QDebug debug, LuaTokenTypes const &token)
{
	debug << static_cast<std::underlying_type<LuaTokenTypes>::type>(token);
	return debug;
}

}
}
}

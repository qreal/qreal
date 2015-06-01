/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <type_traits>
#include <QtCore/QHash>
#include <QtCore/QDebug>

namespace qrtext {
namespace lua {
namespace details {

/// Tokens recognizable by Lua lexer.
enum class LuaTokenTypes
{
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
	, doubleAmpersand
	, tilda
	, verticalLine
	, doubleVerticalLine
	, doubleLess
	, doubleGreater
	, doubleSlash

	, doubleEquals
	, tildaEquals
	, exclamationMarkEquals
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

/// Overload that allows to use token types as hash keys.
inline uint qHash(const LuaTokenTypes &key)
{
	return ::qHash(static_cast<std::underlying_type<LuaTokenTypes>::type>(key));
}

/// Helper operator for debugging.
inline QDebug operator <<(QDebug debug, const LuaTokenTypes &token)
{
	debug << static_cast<std::underlying_type<LuaTokenTypes>::type>(token);
	return debug;
}

}
}
}

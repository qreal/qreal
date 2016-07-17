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

#pragma once

#include <QtCore/QHash>
#include <QtCore/QDebug>
#include <type_traits>

/// Tokens recognizable by lexer.
namespace simpleParser {
enum class TokenTypes {
	newline
	, whitespace
	, identifier

	, openingBracket
	, closingBracket
	, openingCurlyBracket
	, closingCurlyBracket

	, dot
	, comma
	, doubleColon
	, arrow

	, plus

	, greater
	, less
	, equal
	, notEqual

	, ifKeyword
	, elseKeyword

	, foreachKeyword
	, foreachExcludeKeyword
	, excludeKeyword
	, newlineKeyword
	, tabKeyword
	, inKeyword

	, thisKeyword

	, callGeneratorForKeyword
	, generatorKeyword

	, generateToFileKeyword

	, text

	, outcomingLinksKeyword
	, incomingLinksKeyword
	, linksKeyword

	, transitionEndKeyword
	, transitionStartKeyword

	, comment
	, string
};

/// Overload that allows to use token types as hash keys.
inline uint qHash(TokenTypes const &key)
{
	return ::qHash(static_cast<std::underlying_type<TokenTypes>::type>(key));
}

/// Helper operator for debugging.
inline QDebug operator <<(QDebug debug, TokenTypes const &token)
{
	switch (token)
	{
	case TokenTypes::newline:
		debug << "newline ";
		break;

	case TokenTypes::whitespace:
		debug << "whitespace ";
		break;

	case TokenTypes::identifier:
		debug << "identifier ";
		break;

	case TokenTypes::openingBracket:
		debug << "openingBracket ";
		break;

	case TokenTypes::closingBracket:
		debug << "closingBracket ";
		break;

	case TokenTypes::openingCurlyBracket:
		debug << "openingCurlyBracket ";
		break;

	case TokenTypes::closingCurlyBracket:
		debug << "closingCurlyBracket ";
		break;

	case TokenTypes::dot:
		debug << "dot ";
		break;

	case TokenTypes::comma:
		debug << "comma";
		break;

	case TokenTypes::doubleColon:
		debug << "doubleColon";
		break;

	case TokenTypes::arrow:
		debug << "arrow";
		break;

	case TokenTypes::plus:
		debug << "plus";
		break;

	case TokenTypes::greater:
		debug << "greater";
		break;

	case TokenTypes::less:
		debug << "less";
		break;

	case TokenTypes::equal:
		debug << "equal";
		break;

	case TokenTypes::notEqual:
		debug << "notEqual";
		break;

	case TokenTypes::ifKeyword:
		debug << "ifKeyword";
		break;

	case TokenTypes::elseKeyword:
		debug << "elseKeyword";
		break;

	case TokenTypes::foreachKeyword:
		debug << "foreachKeyword ";
		break;

	case TokenTypes::foreachExcludeKeyword:
		debug << "foreachExcludeKeyword";
		break;

	case TokenTypes::excludeKeyword:
		debug << "excludeKeyword";
		break;

	case TokenTypes::newlineKeyword:
		debug << "newlineKeyword ";
		break;

	case TokenTypes::tabKeyword:
		debug << "tabKeyword ";
		break;

	case TokenTypes::inKeyword:
		debug << "inKeyword ";
		break;

	case TokenTypes::thisKeyword:
		debug << "thisKeyword ";
		break;

	case TokenTypes::callGeneratorForKeyword:
		debug << "callGeneratorForKeyword ";
		break;

	case TokenTypes::generatorKeyword:
		debug << "generatorKeyword ";
		break;

	case TokenTypes::generateToFileKeyword:
		debug << "generateToFileKeyword ";
		break;

	case TokenTypes::text:
		debug << "text ";
		break;

	case TokenTypes::outcomingLinksKeyword:
		debug << "outcomingLinksKeyword ";
		break;

	case TokenTypes::incomingLinksKeyword:
		debug << "incomingLinksKeyword ";
		break;

	case TokenTypes::linksKeyword:
		debug << "linksKeyword ";
		break;

	case TokenTypes::transitionEndKeyword:
		debug << "transitionEndKeyword ";
		break;

	case TokenTypes::transitionStartKeyword:
		debug << "transitionStartKeyword ";
		break;

	default:
		debug << "ERROR ";
		break;
	}

	return debug;
}

}

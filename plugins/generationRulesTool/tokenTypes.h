#pragma once

#pragma once

#include <QtCore/QHash>
#include <QtCore/QDebug>
#include <type_traits>

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

	, foreachKeyword
	, newlineKeyword
	, inKeyword

	, callGeneratorForKeyword

	, text

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

	case TokenTypes::foreachKeyword:
		debug << "foreachKeyword ";
		break;

	case TokenTypes::newlineKeyword:
		debug << "newlineKeyword ";
		break;

	case TokenTypes::inKeyword:
		debug << "inKeyword ";
		break;

	case TokenTypes::callGeneratorForKeyword:
		debug << "callGeneratorForKeyword ";
		break;

	case TokenTypes::text:
		debug << "text ";
		break;

	default:
		debug << "ERROR ";
		break;
	}

	return debug;
}

}

#pragma once

#include <QtCore/QString>

#include "qrtext/core/range.h"

namespace qrtext {
namespace core {

/// Token. Has type, location in source code and string representation. Parameterized by enum class containing types
/// of tokens.
template<typename TokenType>
class Token {
public:
	/// Constructor.
	/// @param tokenType - token type.
	/// @param range - token location in source code.
	/// @param lexeme - string representation of a token.
	Token(TokenType tokenType, Range const &range, QString const &lexeme)
		: mToken(tokenType), mRange(range), mLexeme(lexeme)
	{
	}

	/// Returns type of the token.
	TokenType token() const
	{
		return mToken;
	}

	/// Returns location of the token.
	Range const &range() const
	{
		return mRange;
	}

	/// Returns string representation of the token.
	QString const &lexeme() const
	{
		return mLexeme;
	}

private:
	TokenType mToken;
	Range mRange;
	QString mLexeme;
};

}
}

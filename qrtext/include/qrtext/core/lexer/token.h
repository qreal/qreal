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

#include <QtCore/QString>

#include "qrtext/core/range.h"

namespace qrtext {
namespace core {

/// Token. Has type, location in source code and string representation. Parameterized by enum class containing types
/// of tokens.
template<typename TokenType>
class Token
{
public:
	/// Constructor.
	/// @param tokenType - token type.
	/// @param range - token location in source code.
	/// @param lexeme - string representation of a token.
	Token(TokenType tokenType, const Range &range, const QString &lexeme)
		: mToken(tokenType), mRange(range), mLexeme(lexeme)
	{
	}

	/// Returns type of the token.
	TokenType token() const
	{
		return mToken;
	}

	/// Returns location of the token.
	const Range &range() const
	{
		return mRange;
	}

	/// Returns string representation of the token.
	const QString &lexeme() const
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

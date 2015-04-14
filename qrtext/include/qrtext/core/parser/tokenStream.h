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

#include "qrtext/core/lexer/token.h"
#include "qrtext/core/error.h"

namespace qrtext {
namespace core {

/// Stream of tokens.
template<typename TokenType>
class TokenStream
{
public:
	/// Constructor. Takes list of tokens and names of tokens (for error reporting) from lexer and a reference to error
	/// stream where to put errors.
	TokenStream(QList<Token<TokenType>> const &tokenList, QHash<TokenType, QString> const &tokenUserFriendlyNames
			, QList<Error> &errorList)
		: mTokenList(tokenList), mErrorList(errorList), mTokenUserFriendlyNames(tokenUserFriendlyNames), mPosition(0)
	{
	}

	/// Returns next token, or last token if there is end of stream.
	Token<TokenType> next() const
	{
		if (mPosition >= mTokenList.size()) {
			return mTokenList.last();
		}

		return mTokenList.at(mPosition);
	}

	/// Advances stream.
	void consume()
	{
		++mPosition;
	}

	/// Advances stream if current token is the expected one, raises error otherwise.
	bool expect(TokenType token)
	{
		if (next().token() == token) {
			consume();
			return true;
		} else {
			mErrorList << Error(next().range().start()
					, QString(QObject::tr("Expected \"%1\", got \"%2\""))
							.arg(mTokenUserFriendlyNames[token])
							.arg(mTokenUserFriendlyNames[next().token()])
					, ErrorType::syntaxError
					, Severity::error);

			return false;
		}
	}

	/// Returns true, if end of token stream is reached.
	bool isEnd()
	{
		return mPosition == mTokenList.size();
	}

private:
	QList<Token<TokenType>> mTokenList;
	QList<Error> &mErrorList;
	QHash<TokenType, QString> mTokenUserFriendlyNames;
	int mPosition;
};

}
}

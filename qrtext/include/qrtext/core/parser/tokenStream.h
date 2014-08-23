#pragma once

#include <QtCore/QString>

#include "qrtext/core/lexer/token.h"
#include "qrtext/core/error.h"

namespace qrtext {
namespace core {

template<typename TokenType>
class TokenStream {
public:
	TokenStream(QList<Token<TokenType>> const &tokenList, QList<Error> &errorList)
		: mTokenList(tokenList), mErrorList(errorList), mPosition(0)
	{
	}

	Token<TokenType> next() const
	{
		if (mPosition >= mTokenList.size()) {
			return mTokenList.last();
		}

		return mTokenList.at(mPosition);
	}

	void consume()
	{
		++mPosition;
	}

	bool expect(TokenType token)
	{
		if (next().token() == token) {
			consume();
			return true;
		} else {
			mErrorList << Error(next().range().start()
					, QString("Expected %1, got %2").arg(static_cast<int>(token)).arg(static_cast<int>(next().token()))
					, ErrorType::syntaxError
					, Severity::error);

			return false;
		}
	}

	bool isEnd()
	{
		return mPosition == mTokenList.size();
	}

private:
	QList<Token<TokenType>> mTokenList;
	QList<Error> &mErrorList;
	int mPosition;
};

}
}

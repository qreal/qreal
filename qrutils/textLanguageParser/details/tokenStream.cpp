#include "textLanguageParser/details/tokenStream.h"

using namespace textLanguageParser;
using namespace textLanguageParser::details;

TokenStream::TokenStream(QList<details::Token> const &tokenList, QList<ParserError> &errorList)
	: mTokenList(tokenList), mErrorList(errorList), mPosition(0)
{
}

details::Token TokenStream::next()
{
	if (mPosition >= mTokenList.size()) {
		return mTokenList.last();
	}

	return mTokenList.at(mPosition);
}

void TokenStream::consume()
{
	++mPosition;
}

bool TokenStream::expect(TokenType token)
{
	if (next().token() == token) {
		consume();
		return true;
	} else {
		mErrorList << ParserError(next().range().start()
				, QString("Expected %1, got %2").arg(static_cast<int>(token)).arg(static_cast<int>(next().token()))
				, ErrorType::syntaxError
				, Severity::error);

		return false;
	}
}

bool TokenStream::isEnd()
{
	return mPosition == mTokenList.size();
}

#pragma once

#include <QtCore/QString>

#include "textLanguageParser/details/token.h"
#include "textLanguageParser/parserError.h"

namespace textLanguageParser {
namespace details {

class TokenStream {
public:
	TokenStream(QList<details::Token> const &tokenList, QList<ParserError> &errorList);
	details::Token next() const;
	void consume();
	bool expect(TokenType token);
	bool isEnd();

private:
	QList<details::Token> mTokenList;
	QList<ParserError> &mErrorList;
	int mPosition;
};

}
}

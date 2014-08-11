#pragma once

#include <QtCore/QDebug>

#include "textLanguageParser/details/parsers/parserInterface.h"
#include "textLanguageParser/tokenType.h"
#include "textLanguageParser/details/parserRef.h"

namespace textLanguageParser {
namespace details {

class AlternativeParser : public ParserInterface {
public:
	AlternativeParser(ParserRef parser1, ParserRef parser2)
		: mParser1(parser1), mParser2(parser2)
	{
	}

	QSharedPointer<ast::Node> parse(TokenStream &tokenStream, ParserContext &parserContext) const override
	{
		if (tokenStream.isEnd()) {
			parserContext.reportError("Unexpected end of file");
			return wrap(nullptr);
		}

		if (!(mParser1->first().intersect(mParser2->first())).isEmpty()) {
			parserContext.reportInternalError(
					"Parser can not decide which alternative to use on " + tokenStream.next().lexeme());
		}

		if (mParser1->first().contains(tokenStream.next().token())) {
			return mParser1->parse(tokenStream, parserContext);
		}

		if (mParser2->first().contains(tokenStream.next().token())) {
			return mParser2->parse(tokenStream, parserContext);
		}

		parserContext.reportError("Unexpected token");
		return wrap(nullptr);
	}

	QSet<TokenType> first() const override
	{
		return mParser1->first() + mParser2->first();
	}

private:
	ParserRef mParser1;
	ParserRef mParser2;
};

}
}

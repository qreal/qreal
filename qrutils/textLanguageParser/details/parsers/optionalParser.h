#pragma once

#include "textLanguageParser/details/parserRef.h"
#include "textLanguageParser/details/parsers/parserInterface.h"

namespace textLanguageParser {
namespace details {

class OptionalParser : public ParserInterface {
public:
	OptionalParser(ParserRef parser)
		: mParser(parser)
	{
	}

	QSharedPointer<ast::Node> parse(TokenStream &tokenStream, ParserContext &parserContext) const override
	{
		if (mParser->first().contains(tokenStream.next().token())) {
			return mParser->parse(tokenStream, parserContext);
		}

		return wrap(nullptr);
	}

	QSet<TokenType> first() const override
	{
		return mParser->first();
	}

private:
	ParserRef mParser;
};

}
}

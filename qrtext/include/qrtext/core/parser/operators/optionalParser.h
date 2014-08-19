#pragma once

#include "qrtext/core/parser/parserRef.h"
#include "qrtext/core/parser/operators/parserInterface.h"

namespace qrtext {
namespace core {
namespace parser {

template<typename TokenType>
class OptionalParser : public ParserInterface<TokenType> {
public:
	OptionalParser(ParserRef<TokenType> parser)
		: mParser(parser)
	{
	}

	QSharedPointer<ast::Node> parse(TokenStream<TokenType> &tokenStream
			, ParserContext<TokenType> &parserContext) const override
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
	ParserRef<TokenType> mParser;
};

}
}
}

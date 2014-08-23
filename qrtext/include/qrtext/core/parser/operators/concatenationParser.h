#pragma once

#include "qrtext/core/parser/parserRef.h"
#include "qrtext/core/parser/operators/parserInterface.h"
#include "qrtext/core/parser/temporaryNodes/temporaryPair.h"
#include "qrtext/core/parser/temporaryNodes/temporaryDiscardableNode.h"

namespace qrtext {
namespace core {

template<typename TokenType>
class ConcatenationParser : public ParserInterface<TokenType> {
public:
	ConcatenationParser(ParserRef<TokenType> const &parser1, ParserRef<TokenType> const &parser2)
		: mParser1(parser1), mParser2(parser2)
	{
	}

	QSharedPointer<ast::Node> parse(TokenStream<TokenType> &tokenStream
			, ParserContext<TokenType> &parserContext) const override
	{
		if (tokenStream.isEnd()) {
			parserContext.reportError("Unexpected end of file");
			return wrap(nullptr);
		}

		if (mParser1->first().contains(tokenStream.next().token())) {
			QSharedPointer<ast::Node> parser1Result = mParser1->parse(tokenStream, parserContext);
			QSharedPointer<ast::Node> parser2Result = mParser2->parse(tokenStream, parserContext);

			if (parser1Result && parser1Result->is<TemporaryDiscardableNode>()
					&& parser2Result && parser2Result->is<TemporaryDiscardableNode>())
			{
				auto result = wrap(new TemporaryDiscardableNode());
				result->connect(parser1Result);
				result->connect(parser2Result);
				return result;
			} else if (parser1Result && parser1Result->is<TemporaryDiscardableNode>()) {
				return parser2Result;
			} else if (parser2Result && parser2Result->is<TemporaryDiscardableNode>()) {
				return parser1Result;
			} else {
				auto result = wrap(new TemporaryPair(parser1Result, parser2Result));
				result->connect(parser1Result);
				result->connect(parser2Result);
				return result;
			}
		}

		parserContext.reportError("Unexpected token");
		return wrap(nullptr);
	}

	QSet<TokenType> first() const override
	{
		return mParser1->first();
	}

private:
	ParserRef<TokenType> mParser1;
	ParserRef<TokenType> mParser2;
};

}
}

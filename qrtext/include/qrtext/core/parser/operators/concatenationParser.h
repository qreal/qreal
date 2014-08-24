#pragma once

#include "qrtext/core/parser/parserRef.h"
#include "qrtext/core/parser/operators/parserInterface.h"
#include "qrtext/core/parser/temporaryNodes/temporaryPair.h"
#include "qrtext/core/parser/temporaryNodes/temporaryDiscardableNode.h"

namespace qrtext {
namespace core {

/// Elementary parser for parsing concatenations in form of A = B C. If one of parsers returns TemporaryDiscardableNode,
/// it will return result of the other parser (connection of discardable will be lost), if both are discardable,
/// it will return discardable with corrected connection, if both are not discardable, it will return TemporaryPair.
template<typename TokenType>
class ConcatenationParser : public ParserInterface<TokenType> {
public:
	/// Constructor. Takes parsers to concatenate.
	ConcatenationParser(ParserRef<TokenType> const &parser1, ParserRef<TokenType> const &parser2)
		: mParser1(parser1), mParser2(parser2)
	{
	}

	QSharedPointer<ast::Node> parse(TokenStream<TokenType> &tokenStream
			, ParserContext<TokenType> &parserContext) const override
	{
		if (tokenStream.isEnd()) {
			parserContext.reportError(QObject::tr("Unexpected end of file"));
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

		parserContext.reportError(QObject::tr("Unexpected token"));
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

#pragma once

#include "qrtext/core/parser/parserRef.h"
#include "qrtext/core/parser/operators/parserInterface.h"
#include "qrtext/core/parser/temporaryNodes/temporaryList.h"
#include "qrtext/core/parser/temporaryNodes/temporaryDiscardableNode.h"

namespace qrtext {
namespace core {

/// Elementary parser for parsing Kleene star, in form of A = B*. Temporary discardable nodes are discarded from
/// final output, but their connection preserved. Guaranteed to return TemporaryList (maybe empty).
template<typename TokenType>
class KleeneStarParser : public ParserInterface<TokenType> {
public:
	/// Constructor. Takes parser for which to build Kleene closure.
	KleeneStarParser(ParserRef<TokenType> const &parser)
		: mParser(parser)
	{
	}

	QSharedPointer<ast::Node> parse(TokenStream<TokenType> &tokenStream
			, ParserContext<TokenType> &parserContext) const override
	{
		auto temporaryList = QSharedPointer<TemporaryList>(new TemporaryList());
		while (!tokenStream.isEnd() && mParser->first().contains(tokenStream.next().token())) {
			QSharedPointer<ast::Node> result = mParser->parse(tokenStream, parserContext);
			if (!result) {
				break;
			}

			if (!result->is<TemporaryDiscardableNode>()) {
				temporaryList->list() << result;
			}

			temporaryList->connect(result);
		}

		return temporaryList;
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

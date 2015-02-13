#pragma once

#include "qrtext/core/parser/parserRef.h"
#include "qrtext/core/parser/operators/parserInterface.h"
#include "qrtext/core/parser/temporaryNodes/temporaryDiscardableNode.h"

namespace qrtext {
namespace core {

/// Elementary parser for parsing optional parts in form of A = B?. Returns result of a parser if optional part is
/// present or TemporaryDiscardableNode if not.
template<typename TokenType>
class OptionalParser : public ParserInterface<TokenType>
{
public:
	/// Constructor. Takes parser for optional part.
	OptionalParser(const ParserRef<TokenType> &parser)
		: mParser(parser)
	{
	}

	QSharedPointer<ast::Node> parse(TokenStream<TokenType> &tokenStream
			, ParserContext<TokenType> &parserContext) const override
	{
		if (mParser->first().contains(tokenStream.next().token())) {
			return mParser->parse(tokenStream, parserContext);
		}

		return wrap(new TemporaryDiscardableNode());
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

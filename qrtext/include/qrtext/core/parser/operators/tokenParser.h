#pragma once

#include "qrtext/core/parser/operators/parserInterface.h"
#include "qrtext/core/parser/temporaryNodes/temporaryToken.h"

namespace qrtext {
namespace core {

template<typename TokenType, typename SemanticAction>
class TokenParser : public ParserInterface<TokenType> {
public:
	explicit TokenParser(TokenType token, SemanticAction semanticAction)
		: mToken(token), mSemanticAction(semanticAction)
	{
	}

	QSharedPointer<ast::Node> parse(TokenStream<TokenType> &tokenStream
			, ParserContext<TokenType> &parserContext) const override
	{
		Q_UNUSED(parserContext);

		Token<TokenType> const token = tokenStream.next();
		if (!tokenStream.expect(mToken))
		{
			return wrap(nullptr);
		}

		auto node = wrap(mSemanticAction(token));
		if (node) {
			node->connect(token);
		}

		return node;
	}

	QSet<TokenType> first() const override
	{
		return {mToken};
	}

private:
	TokenType mToken;
	SemanticAction mSemanticAction;
};

}
}

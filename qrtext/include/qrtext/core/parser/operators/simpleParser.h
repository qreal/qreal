#pragma once

#include "qrtext/core/parser/operators/parserInterface.h"

namespace qrtext {
namespace core {
namespace parser {

template<typename TokenType, typename SemanticAction>
class SimpleParser : public ParserInterface<TokenType> {
public:
	explicit SimpleParser(TokenType token, SemanticAction semanticAction)
		: mToken(token), mSemanticAction(semanticAction)
	{
	}

	QSharedPointer<ast::Node> parse(TokenStream<TokenType> &tokenStream
			, ParserContext<TokenType> &parserContext) const override
	{
		Q_UNUSED(parserContext);

		lexer::Token<TokenType> const token = tokenStream.next();
		if (!tokenStream.expect(mToken)) {
			return wrap(nullptr);
		}

		auto const node = wrap(mSemanticAction());
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
}

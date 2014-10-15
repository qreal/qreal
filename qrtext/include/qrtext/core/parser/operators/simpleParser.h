#pragma once

#include "qrtext/core/parser/operators/parserInterface.h"

namespace qrtext {
namespace core {

/// Elementary parser that parses given token and executes given semantic action on a result. Semantic action takes
/// no parameters.
template<typename TokenType, typename SemanticAction>
class SimpleParser : public ParserInterface<TokenType>
{
public:
	/// Constructor. Takes token to parse and lambda function to execute if token is parsed successfully.
	SimpleParser(TokenType token, SemanticAction const &semanticAction)
		: mToken(token), mSemanticAction(semanticAction)
	{
	}

	QSharedPointer<ast::Node> parse(TokenStream<TokenType> &tokenStream
			, ParserContext<TokenType> &parserContext) const override
	{
		Q_UNUSED(parserContext);

		Token<TokenType> const token = tokenStream.next();
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

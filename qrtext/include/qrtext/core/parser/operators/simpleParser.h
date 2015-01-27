#pragma once

#include "qrtext/core/parser/operators/parserInterface.h"
#include "qrtext/core/parser/temporaryNodes/temporaryErrorNode.h"

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
			return wrap(new TemporaryErrorNode());
		}

		auto const node = wrap(mSemanticAction());
		if (node) {
			node->connect(token);
		} else {
			Q_ASSERT(false);
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

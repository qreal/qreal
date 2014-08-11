#pragma once

#include "textLanguageParser/details/parsers/parserInterface.h"
#include "textLanguageParser/tokenType.h"
#include "textLanguageParser/details/temporaryNodes/temporaryToken.h"

namespace textLanguageParser {
namespace details {

template<typename SemanticAction>
class TokenParser : public ParserInterface {
public:
	explicit TokenParser(TokenType token, SemanticAction semanticAction)
		: mToken(token), mSemanticAction(semanticAction)
	{
	}

	QSharedPointer<ast::Node> parse(TokenStream &tokenStream, ParserContext &parserContext) const override
	{
		Q_UNUSED(parserContext);

		Token const token = tokenStream.next();
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

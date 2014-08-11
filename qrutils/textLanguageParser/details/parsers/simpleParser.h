#pragma once

#include "textLanguageParser/details/parsers/parserInterface.h"
#include "textLanguageParser/tokenType.h"

namespace textLanguageParser {
namespace details {

template<typename SemanticAction>
class SimpleParser : public ParserInterface {
public:
	explicit SimpleParser(TokenType token, SemanticAction semanticAction)
		: mToken(token), mSemanticAction(semanticAction)
	{
	}

	QSharedPointer<ast::Node> parse(TokenStream &tokenStream, ParserContext &parserContext) const override
	{
		Q_UNUSED(parserContext);

		Token const token = tokenStream.next();
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

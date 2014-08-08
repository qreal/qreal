#pragma once

#include "textLanguageParser/details/parserInterface.h"
#include "textLanguageParser/tokenType.h"
#include "textLanguageParser/details/temporaryToken.h"

namespace textLanguageParser {
namespace details {

template<typename SemanticAction>
class TokenParser : public ParserInterface {
public:
	explicit TokenParser(TokenType token, SemanticAction semanticAction)
		: mToken(token), mSemanticAction(semanticAction)
	{
	}

	TextLanguageParserInterface::Result parse(TokenStream &tokenStream) const override
	{
		Token const token = tokenStream.next();
		tokenStream.expect(mToken);
		ast::Node * const node = mSemanticAction(token);
		node->connect(token);
		return TextLanguageParserInterface::Result(node, {});
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

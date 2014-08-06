#pragma once

#include "textLanguageParser/details/parserInterface.h"
#include "textLanguageParser/tokenType.h"
#include "textLanguageParser/ast/temporaryToken.h"

namespace textLanguageParser {
namespace details {

class SimpleParser : public ParserInterface {
public:
	explicit SimpleParser(TokenType token)
		: mToken(token)
	{
	}

	TextLanguageParserInterface::Result parse(TokenStream &tokenStream) const override
	{
		Token const token = tokenStream.next();
		tokenStream.expect(mToken);
		ast::TemporaryToken * const tokenNode = new ast::TemporaryToken(token);
		tokenNode->connect(token);
		return TextLanguageParserInterface::Result(tokenNode, {});
	}

	QSet<TokenType> first() const override
	{
		return {mToken};
	}

private:
	TokenType mToken;
};

}
}

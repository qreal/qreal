#pragma once

#include "textLanguageParser/details/parserInterface.h"
#include "textLanguageParser/tokenType.h"
#include "textLanguageParser/ast/temporaryPair.h"

namespace textLanguageParser {
namespace details {

template<typename P1, typename P2>
class ConcatenationParser : public ParserInterface {
public:
	ConcatenationParser(P1 parser1, P2 parser2)
		: mParser1(parser1), mParser2(parser2)
	{
	}

	TextLanguageParserInterface::Result parse(TokenStream &tokenStream) const override
	{
		if (tokenStream.isEnd()) {
			return TextLanguageParserInterface::Result(nullptr, {ParserError(tokenStream.next().range().end()
					, "Unexpected end of file", ErrorType::syntaxError, Severity::error)});
		}

		if (mParser1.first().contains(tokenStream.next().token())) {
			TextLanguageParserInterface::Result parser1Result = mParser1.parse(tokenStream);
			TextLanguageParserInterface::Result parser2Result = mParser2.parse(tokenStream);
			return TextLanguageParserInterface::Result(
					new ast::TemporaryPair(parser1Result.astRoot, parser2Result.astRoot)
					, parser1Result.errors << parser2Result.errors);
		}

		return TextLanguageParserInterface::Result(nullptr, {ParserError(tokenStream.next().range().end()
				, "Unexpected token", ErrorType::syntaxError, Severity::error)});
	}

	QSet<TokenType> first() const override
	{
		return mParser1.first();
	}

private:
	P1 mParser1;
	P2 mParser2;
};

}
}

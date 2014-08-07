#pragma once

#include "textLanguageParser/details/parserInterface.h"
#include "textLanguageParser/details/parserRef.h"
#include "textLanguageParser/tokenType.h"
#include "textLanguageParser/ast/temporaryPair.h"

namespace textLanguageParser {
namespace details {

class ConcatenationParser : public ParserInterface {
public:
	ConcatenationParser(ParserRef parser1, ParserRef parser2)
		: mParser1(parser1), mParser2(parser2)
	{
	}

	TextLanguageParserInterface::Result parse(TokenStream &tokenStream) const override
	{
		if (tokenStream.isEnd()) {
			return TextLanguageParserInterface::Result(nullptr, {ParserError(tokenStream.next().range().end()
					, "Unexpected end of file", ErrorType::syntaxError, Severity::error)});
		}

		if (mParser1->first().contains(tokenStream.next().token())) {
			TextLanguageParserInterface::Result parser1Result = mParser1->parse(tokenStream);
			TextLanguageParserInterface::Result parser2Result = mParser2->parse(tokenStream);
			if (!parser2Result.astRoot) {
				return parser1Result;
			} else if (!parser1Result.astRoot) {
				return parser2Result;
			} else {
				ast::TemporaryPair *temporaryPair
						= new ast::TemporaryPair(parser1Result.astRoot, parser2Result.astRoot);

				temporaryPair->connect(parser1Result.astRoot);
				temporaryPair->connect(parser2Result.astRoot);
				return TextLanguageParserInterface::Result(temporaryPair, parser1Result.errors << parser2Result.errors);
			}
		}

		return TextLanguageParserInterface::Result(nullptr, {ParserError(tokenStream.next().range().end()
				, "Unexpected token", ErrorType::syntaxError, Severity::error)});
	}

	QSet<TokenType> first() const override
	{
		return mParser1->first();
	}

private:
	ParserRef mParser1;
	ParserRef mParser2;
};

}
}

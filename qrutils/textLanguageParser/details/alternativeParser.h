#pragma once

#include "textLanguageParser/details/parserInterface.h"
#include "textLanguageParser/tokenType.h"

namespace textLanguageParser {
namespace details {

class AlternativeParser : public ParserInterface {
public:
	AlternativeParser(ParserInterface const &parser1, ParserInterface const &parser2)
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
			return mParser1.parse(tokenStream);
		}

		if (mParser2.first().contains(tokenStream.next().token())) {
			return mParser2.parse(tokenStream);
		}

		return TextLanguageParserInterface::Result(nullptr, {ParserError(tokenStream.next().range().end()
				, "Unexpected token", ErrorType::syntaxError, Severity::error)});
	}

	QSet<TokenType> first() const override
	{
		return mParser1.first() + mParser2.first();
	}

private:
	ParserInterface const &mParser1;
	ParserInterface const &mParser2;
};

}
}

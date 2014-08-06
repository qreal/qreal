#pragma once

#include "textLanguageParser/details/parserInterface.h"
#include "textLanguageParser/tokenType.h"
#include "textLanguageParser/details/parserRef.h"

namespace textLanguageParser {
namespace details {

class AlternativeParser : public ParserInterface {
public:
	AlternativeParser(ParserRef parser1, ParserRef parser2)
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
			return mParser1->parse(tokenStream);
		}

		if (mParser2->first().contains(tokenStream.next().token())) {
			return mParser2->parse(tokenStream);
		}

		return TextLanguageParserInterface::Result(nullptr, {ParserError(tokenStream.next().range().end()
				, "Unexpected token", ErrorType::syntaxError, Severity::error)});
	}

	QSet<TokenType> first() const override
	{
		return mParser1->first() + mParser2->first();
	}

private:
	ParserRef mParser1;
	ParserRef mParser2;
};

}
}

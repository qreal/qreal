#pragma once

#include "textLanguageParser/details/parserInterface.h"
#include "textLanguageParser/details/parserRef.h"

namespace textLanguageParser {
namespace details {

class OptionalParser : public ParserInterface {
public:
	OptionalParser(ParserRef parser)
		: mParser(parser)
	{
	}

	TextLanguageParserInterface::Result parse(TokenStream &tokenStream) const override
	{
		if (mParser->first().contains(tokenStream.next().token())) {
			return mParser->parse(tokenStream);
		}

		return TextLanguageParserInterface::Result(nullptr, {});
	}

	QSet<TokenType> first() const override
	{
		return mParser->first();
	}

private:
	ParserRef mParser;
};

}
}

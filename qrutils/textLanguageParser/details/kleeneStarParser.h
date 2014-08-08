#pragma once

#include "textLanguageParser/tokenType.h"
#include "textLanguageParser/details/parserInterface.h"
#include "textLanguageParser/details/parserRef.h"
#include "textLanguageParser/details/temporaryList.h"

namespace textLanguageParser {
namespace details {

class KleeneStarParser : public ParserInterface {
public:
	KleeneStarParser(ParserRef const &parser)
		: mParser(parser)
	{
	}

	TextLanguageParserInterface::Result parse(TokenStream &tokenStream) const override
	{
		QList<ParserError> errors;
		auto temporaryList = QSharedPointer<TemporaryList>(new TemporaryList());
		while (mParser->first().contains(tokenStream.next().token())) {
			auto result = mParser->parse(tokenStream);
			errors << result.errors;
			temporaryList->list() << result.astRoot;
		}

		return TextLanguageParserInterface::Result(temporaryList, errors);
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

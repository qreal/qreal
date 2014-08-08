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

	QSharedPointer<ast::Node> parse(TokenStream &tokenStream, ParserContext &parserContext) const override
	{
		auto temporaryList = QSharedPointer<TemporaryList>(new TemporaryList());
		while (mParser->first().contains(tokenStream.next().token())) {
			auto result = mParser->parse(tokenStream, parserContext);
			if (!result) {
				break;
			}

			temporaryList->list() << result;
		}

		return temporaryList;
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

#pragma once

#include "textLanguageParser/tokenType.h"
#include "textLanguageParser/details/parserRef.h"
#include "textLanguageParser/details/parsers/parserInterface.h"
#include "textLanguageParser/details/temporaryNodes/temporaryList.h"
#include "textLanguageParser/details/temporaryNodes/temporaryDiscardableNode.h"

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
		while (!tokenStream.isEnd() && mParser->first().contains(tokenStream.next().token())) {
			auto result = mParser->parse(tokenStream, parserContext);
			if (!result) {
				break;
			}

			if (!result->is<TemporaryDiscardableNode>()) {
				temporaryList->list() << result;
			}

			temporaryList->connect(result);
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

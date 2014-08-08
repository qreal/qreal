#pragma once

#include "textLanguageParser/tokenType.h"
#include "textLanguageParser/details/parserInterface.h"
#include "textLanguageParser/details/parserRef.h"
#include "textLanguageParser/details/temporaryPair.h"
#include "textLanguageParser/details/temporaryDiscardableNode.h"

namespace textLanguageParser {
namespace details {

class ConcatenationParser : public ParserInterface {
public:
	ConcatenationParser(ParserRef parser1, ParserRef parser2)
		: mParser1(parser1), mParser2(parser2)
	{
	}

	QSharedPointer<ast::Node> parse(TokenStream &tokenStream, ParserContext &parserContext) const override
	{
		if (tokenStream.isEnd()) {
			parserContext.reportError("Unexpected end of file");
			return wrap(nullptr);
		}

		if (mParser1->first().contains(tokenStream.next().token())) {
			auto parser1Result = mParser1->parse(tokenStream, parserContext);
			auto parser2Result = mParser2->parse(tokenStream, parserContext);

			if (parser1Result && parser1Result->is<TemporaryDiscardableNode>()
					&& parser2Result && parser2Result->is<TemporaryDiscardableNode>())
			{
				auto result = wrap(new TemporaryDiscardableNode());
				result->connect(parser1Result);
				result->connect(parser2Result);
				return result;
			} else if (parser1Result && parser1Result->is<TemporaryDiscardableNode>()) {
				return parser2Result;
			} else if (parser2Result && parser2Result->is<TemporaryDiscardableNode>()) {
				return parser1Result;
			} else {
				auto result = wrap(new TemporaryPair(parser1Result, parser2Result));
				result->connect(parser1Result);
				result->connect(parser2Result);
				return result;
			}
		}

		parserContext.reportError("Unexpected token");
		return wrap(nullptr);
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

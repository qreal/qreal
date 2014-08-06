#pragma once

#include <functional>

#include "textLanguageParser/details/parserInterface.h"
#include "textLanguageParser/details/functionTraits.h"

namespace textLanguageParser {
namespace details {


template<typename Transformation, typename P>
class TransformingParser : public ParserInterface {
public:
	explicit TransformingParser(P parser
			, Transformation transformation)
		: mTransformation(transformation), mParser(parser)
	{
	}

	TextLanguageParserInterface::Result parse(TokenStream &tokenStream) const override
	{
		typedef typename function_traits<Transformation>::template arg<0>::type NodeType;

		TextLanguageParserInterface::Result parserResult = mParser.parse(tokenStream);
		parserResult.astRoot.reset(mTransformation(static_cast<NodeType>(parserResult.astRoot.data())));
		return parserResult;
	}

	QSet<TokenType> first() const override
	{
		return mParser.first();
	}

private:
	Transformation mTransformation;
	P mParser;
};

}
}

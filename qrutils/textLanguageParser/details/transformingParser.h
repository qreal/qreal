#pragma once

#include <functional>

#include "textLanguageParser/details/parserInterface.h"
#include "textLanguageParser/details/functionTraits.h"

namespace textLanguageParser {
namespace details {


template<typename Transformation>
class TransformingParser : public ParserInterface {
public:
	explicit TransformingParser(ParserInterface const &parser
			, Transformation transformation)
		: mTransformation(transformation), mParser(parser)
	{
	}

	TextLanguageParserInterface::Result parse(TokenStream &tokenStream) const override
	{
		typedef typename function_traits<Transformation>::template arg<0>::type NodeType;

		TextLanguageParserInterface::Result parserResult = mParser.parse(tokenStream);
		ast::Node * temp = parserResult.astRoot;
		parserResult.astRoot = mTransformation(static_cast<NodeType>(temp));
		delete temp;
		return parserResult;
	}

	QSet<TokenType> first() const override
	{
		return mParser.first();
	}

private:
	Transformation mTransformation;
	ParserInterface const &mParser;
};

}
}

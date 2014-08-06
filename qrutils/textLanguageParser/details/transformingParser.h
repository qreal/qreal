#pragma once

#include <type_traits>

#include "textLanguageParser/details/parserInterface.h"
#include "textLanguageParser/details/parserRef.h"
#include "textLanguageParser/details/functionTraits.h"

namespace textLanguageParser {
namespace details {


template<typename Transformation>
class TransformingParser : public ParserInterface {
public:
	explicit TransformingParser(ParserRef parser, Transformation transformation)
		: mTransformation(transformation), mParser(parser)
	{
	}

	TextLanguageParserInterface::Result parse(TokenStream &tokenStream) const override
	{
		typedef typename function_traits<Transformation>::template arg<0>::type PointerToNodeType;
		typedef typename std::remove_pointer<PointerToNodeType>::type NodeType;

		TextLanguageParserInterface::Result parserResult = mParser->parse(tokenStream);
		QSharedPointer<NodeType> node = parserResult.astRoot.staticCast<NodeType>();
		parserResult.astRoot = mTransformation(node.data()).template staticCast<ast::Node>();
		parserResult.astRoot->connect(*node);
		return parserResult;
	}

	QSet<TokenType> first() const override
	{
		return mParser->first();
	}

private:
	Transformation mTransformation;
	ParserRef mParser;
};

}
}

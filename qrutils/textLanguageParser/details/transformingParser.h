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

	QSharedPointer<ast::Node> parse(TokenStream &tokenStream, ParserContext &parserContext) const override
	{
		typedef typename function_traits<Transformation>::template arg<0>::type PointerToNodeType;
		typedef decltype(&PointerToNodeType::operator *) DereferenceOperatorType;
		typedef typename function_traits<DereferenceOperatorType>::result_type NodeReference;
		typedef typename std::remove_reference<NodeReference>::type NodeType;

		auto parserResult = mParser->parse(tokenStream, parserContext);
		auto node = as<NodeType>(parserResult);
		parserResult = as<ast::Node>(mTransformation(node));
		if (parserResult) {
			parserResult->connect(node);
		}

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

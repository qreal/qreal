#pragma once

#include <type_traits>

#include "qrtext/core/parser/parserRef.h"
#include "qrtext/core/parser/operators/parserInterface.h"
#include "qrtext/core/parser/temporaryNodes/temporaryDiscardableNode.h"
#include "qrtext/core/parser/utils/functionTraits.h"

namespace qrtext {
namespace core {
namespace parser {

template<typename TokenType, typename Transformation>
class TransformingParser : public ParserInterface<TokenType> {
public:
	explicit TransformingParser(ParserRef<TokenType> parser, Transformation transformation)
		: mTransformation(transformation), mParser(parser)
	{
	}

	QSharedPointer<ast::Node> parse(TokenStream<TokenType> &tokenStream
			, ParserContext<TokenType> &parserContext) const override
	{
		typedef typename function_traits<Transformation>::template arg<0>::type PointerToNodeType;
		typedef decltype(&PointerToNodeType::operator *) DereferenceOperatorType;
		typedef typename function_traits<DereferenceOperatorType>::result_type NodeReference;
		typedef typename std::remove_reference<NodeReference>::type NodeType;

		auto parserResult = mParser->parse(tokenStream, parserContext);
		auto node = as<NodeType>(parserResult);
		parserResult = as<ast::Node>(mTransformation(node));
		if (!parserResult) {
			parserResult = wrap(new TemporaryDiscardableNode());
		}

		parserResult->connect(node);
		return parserResult;
	}

	QSet<TokenType> first() const override
	{
		return mParser->first();
	}

private:
	Transformation mTransformation;
	ParserRef<TokenType> mParser;
};

}
}
}

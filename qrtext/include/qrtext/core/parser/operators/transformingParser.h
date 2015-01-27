#pragma once

#include <type_traits>

#include "qrtext/core/parser/parserRef.h"
#include "qrtext/core/parser/operators/parserInterface.h"
#include "qrtext/core/parser/temporaryNodes/temporaryDiscardableNode.h"
#include "qrtext/core/parser/temporaryNodes/temporaryErrorNode.h"
#include "qrtext/core/parser/utils/functionTraits.h"

namespace qrtext {
namespace core {

/// Elementary parser that executes given semantic action on a result of parsing from given parser. Used to attach
/// semantic rules to a grammar. Semantic action may opt to discard a node, in such case it shall return nullptr.
/// Note that semantic action will be called even if parser failed, in such case nullptr will be passed.
template<typename TokenType, typename Transformation>
class TransformingParser : public ParserInterface<TokenType>
{
public:
	/// Constructor. Takes parser and semantic action to execute on a result of a parser.
	TransformingParser(ParserRef<TokenType> const &parser, Transformation const &transformation)
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

		QSharedPointer<ast::Node> parserResult = mParser->parse(tokenStream, parserContext);
		if (parserResult->is<TemporaryErrorNode>()) {
			return parserResult;
		}

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

#pragma once

#include <QtCore/QSet>

#include "qrtext/core/ast/node.h"
#include "qrtext/core/parser/tokenStream.h"
#include "qrtext/core/parser/parserContext.h"

namespace qrtext {
namespace core {

/// Interface for all elementary parsers.
template<typename TokenType>
class ParserInterface
{
public:
	virtual ~ParserInterface() {}

	/// Parses given token stream in given context and returns AST with result or nullptr if failed.
	virtual QSharedPointer<ast::Node> parse(TokenStream<TokenType> &tokenStream
			, ParserContext<TokenType> &parserContext) const = 0;

	/// Returns FIRST set for a parser.
	virtual QSet<TokenType> first() const = 0;
};

}
}

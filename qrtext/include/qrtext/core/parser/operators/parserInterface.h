#pragma once

#include <QtCore/QSet>

#include "qrtext/core/ast/node.h"
#include "qrtext/core/parser/tokenStream.h"
#include "qrtext/core/parser/parserContext.h"

namespace qrtext {
namespace core {

template<typename TokenType>
class ParserInterface {
public:
	virtual ~ParserInterface() {}

	virtual QSharedPointer<ast::Node> parse(TokenStream<TokenType> &tokenStream
			, ParserContext<TokenType> &parserContext) const = 0;

	virtual QSet<TokenType> first() const = 0;
};

}
}

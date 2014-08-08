#pragma once

#include <QtCore/QSet>

#include "textLanguageParser/textLanguageParserInterface.h"
#include "textLanguageParser/ast/node.h"
#include "textLanguageParser/details/tokenStream.h"
#include "textLanguageParser/details/parserContext.h"

namespace textLanguageParser {
namespace details {

class ParserInterface {
public:
	virtual ~ParserInterface() {}
	virtual QSharedPointer<ast::Node> parse(TokenStream &tokenStream, ParserContext &parserContext) const = 0;
	virtual QSet<TokenType> first() const = 0;
};

}
}

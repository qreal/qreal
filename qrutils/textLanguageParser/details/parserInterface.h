#pragma once

#include <QtCore/QSet>

#include "textLanguageParser/textLanguageParserInterface.h"
#include "textLanguageParser/ast/node.h"
#include "textLanguageParser/details/tokenStream.h"

namespace textLanguageParser {
namespace details {

class ParserInterface {
public:
	virtual TextLanguageParserInterface::Result parse(TokenStream &tokenStream) const = 0;
	virtual QSet<TokenType> first() const = 0;
};

}
}

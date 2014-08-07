#pragma once

#include "node.h"
#include "textLanguageParser/details/token.h"

namespace textLanguageParser {
namespace ast {

/// @todo Move it somewhere, it is not a part of AST.
class TemporaryToken : public Node {
public:
	TemporaryToken(details::Token token)
		: mToken(token)
	{
	}

	details::Token token() const
	{
		return mToken;
	}

private:
	details::Token mToken;
};

}
}

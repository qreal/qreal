#pragma once

#include "textLanguageParser/ast/node.h"
#include "textLanguageParser/details/token.h"

namespace textLanguageParser {
namespace details {

class TemporaryToken : public ast::Node {
public:
	TemporaryToken(Token token)
		: mToken(token)
	{
	}

	Token token() const
	{
		return mToken;
	}

private:
	Token mToken;
};

}
}

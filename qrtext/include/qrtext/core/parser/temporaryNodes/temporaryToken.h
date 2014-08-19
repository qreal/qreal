#pragma once

#include "qrtext/core/ast/node.h"
#include "qrtext/core/lexer/token.h"

namespace qrtext {
namespace core {
namespace parser {

template<typename TokenType>
class TemporaryToken : public ast::Node {
public:
	TemporaryToken(lexer::Token<TokenType> const &token)
		: mToken(token)
	{
	}

	lexer::Token<TokenType> const &token() const
	{
		return mToken;
	}

private:
	lexer::Token<TokenType> mToken;
};

}
}
}

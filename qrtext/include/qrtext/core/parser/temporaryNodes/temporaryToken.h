#pragma once

#include "qrtext/core/ast/node.h"
#include "qrtext/core/lexer/token.h"

namespace qrtext {
namespace core {

template<typename TokenType>
class TemporaryToken : public ast::Node {
public:
	TemporaryToken(Token<TokenType> const &token)
		: mToken(token)
	{
	}

	Token<TokenType> const &token() const
	{
		return mToken;
	}

private:
	Token<TokenType> mToken;
};

}
}

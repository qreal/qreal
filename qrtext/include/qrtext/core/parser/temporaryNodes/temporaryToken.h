#pragma once

#include "qrtext/core/ast/node.h"
#include "qrtext/core/lexer/token.h"

namespace qrtext {
namespace core {

/// Temporary node to store parsed token. Shall be removed from resulting tree.
template<typename TokenType>
class TemporaryToken : public ast::Node {
public:
	/// Constructor. Takes parsed token.
	TemporaryToken(Token<TokenType> const &token)
		: mToken(token)
	{
	}

	/// Returns token stored in this node.
	Token<TokenType> const &token() const
	{
		return mToken;
	}

private:
	Token<TokenType> mToken;
};

}
}

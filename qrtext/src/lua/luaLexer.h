#pragma once

#include <QtCore/QScopedPointer>

#include "qrtext/core/lexer/lexer.h"
#include "qrtext/src/lua/luaTokenTypes.h"

namespace qrtext {
namespace lua {
namespace details {

/// Lexer of something like Lua 5.3 based on regular expressions. Provides a list of tokens by given input string.
/// Allows Unicode input.
///
/// Now lexer (with default token patterns) follows Lua 5.3 specification with following exceptions:
/// - long brackets are not supported, either for string literals or for comments.
class LuaLexer: public core::lexer::Lexer<LuaTokenTypes> {
public:
	LuaLexer(QList<core::Error> &errors);

private:
	static core::lexer::TokenPatterns<LuaTokenTypes> initPatterns();
};

}
}
}

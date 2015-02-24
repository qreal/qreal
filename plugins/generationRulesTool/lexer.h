#pragma once

#include <QtCore/QScopedPointer>

#include <qrtext/core/lexer/lexer.h>
#include "tokenTypes.h"

namespace simpleParser {
/// Lexer of something like Lua 5.3 based on regular expressions. Provides a list of tokens by given input string.
/// Allows Unicode input.
///
/// Now lexer (with default token patterns) follows Lua 5.3 specification with following exceptions:
/// - long brackets are not supported, either for string literals or for comments.
class Lexer: public qrtext::core::Lexer<TokenTypes>
{
public:
	/// Constructor.
	/// @param errors - error stream to report errors to.
	Lexer(QList<qrtext::core::Error> &errors);

private:
	static qrtext::core::TokenPatterns<TokenTypes> initPatterns();
};
}

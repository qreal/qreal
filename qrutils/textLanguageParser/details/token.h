#pragma once

#include <QtCore/QString>

#include "qrutils/textLanguageParser/tokenType.h"
#include "qrutils/textLanguageParser/ast/range.h"

namespace textLanguageParser {
namespace details {

/// Token. Has type, location in source code and string representation.
class Token {
public:
	/// Constructor.
	/// @param tokenType - token type.
	/// @param range - token location in source code.
	/// @param token - string representation of a token.
	Token(TokenType tokenType, ast::Range const &range, QString const &lexeme);

	/// Returns type of the token.
	TokenType token() const;

	/// Returns location of the token.
	ast::Range const &range() const;

	/// Returns string representation of the token.
	QString const &lexeme() const;

private:
	TokenType mToken;
	ast::Range mRange;
	QString mLexeme;
};

}
}

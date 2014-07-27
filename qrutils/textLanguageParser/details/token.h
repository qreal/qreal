#pragma once

#include "textLanguageParser/lexemes.h"
#include "textLanguageParser/ast/range.h"

namespace textLanguageParser {
namespace details {

/// Token. Has type (lexeme), location in source code and string representation.
class Token {
public:
	/// Constructor.
	/// @param lexeme - token type.
	/// @param range - token location in source code.
	/// @param token - string representation of a token.
	Token(Lexemes::Type lexeme, ast::Range const &range, QString const &token);

	/// Returns type of the token.
	Lexemes::Type lexeme() const;

	/// Returns location of the token.
	ast::Range const &range() const;

	/// Returns string representation of the token.
	QString const &token() const;

private:
	Lexemes::Type mLexeme;
	ast::Range mRange;
	QString mToken;
};

}
}

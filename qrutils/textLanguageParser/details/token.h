#pragma once

#include "textLanguageParser/lexemes.h"
#include "textLanguageParser/ast/range.h"

namespace textLanguageParser {
namespace details {

class Token {
public:
	Token(Lexemes::Type lexeme, ast::Range const &range, QString const &token);

	Lexemes::Type lexeme() const;

	ast::Range const &range() const;

private:
	Lexemes::Type mLexeme;
	ast::Range mRange;
	QString mToken;
};

}
}

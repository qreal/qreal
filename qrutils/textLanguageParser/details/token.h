#pragma once

#include "textLanguageParser/lexemes.h"
#include "textLanguageParser/ast/connection.h"

namespace textLanguageParser {
namespace details {

class Token {
public:
	Token(Lexemes::Type lexeme, ast::Connection const &connection, QString const &token);

	Lexemes::Type lexeme() const;

private:
	Lexemes::Type mLexeme;
	ast::Connection mConnection;
	QString mToken;
};

}
}

#include "textLanguageParser/details/token.h"

using namespace textLanguageParser;
using namespace textLanguageParser::details;

Token::Token(Lexemes::Type lexeme, ast::Connection const &connection, QString const &token)
	: mLexeme(lexeme), mConnection(connection), mToken(token)
{
}

Lexemes::Type Token::lexeme() const
{
	return mLexeme;
}

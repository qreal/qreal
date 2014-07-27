#include "textLanguageParser/details/token.h"

using namespace textLanguageParser;
using namespace textLanguageParser::details;

Token::Token(Lexemes::Type lexeme, ast::Range const &range, QString const &token)
	: mLexeme(lexeme), mRange(range), mToken(token)
{
}

Lexemes::Type Token::lexeme() const
{
	return mLexeme;
}

ast::Range const &Token::range() const
{
	return mRange;
}

QString const &Token::token() const
{
	return mToken;
}

#include "textLanguageParser/details/token.h"

using namespace textLanguageParser;
using namespace textLanguageParser::details;

Token::Token(TokenType tokenType, ast::Range const &range, QString const &lexeme)
	: mToken(tokenType), mRange(range), mLexeme(lexeme)
{
}

TokenType Token::token() const
{
	return mToken;
}

ast::Range const &Token::range() const
{
	return mRange;
}

QString const &Token::lexeme() const
{
	return mLexeme;
}

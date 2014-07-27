#include "textLanguageParser/lexemes.h"

using namespace textLanguageParser;

Lexemes::Lexemes()
{
	mLexemes.insert(whitespace, QRegularExpression("[ \t]+"));
	mLexemes.insert(newline, QRegularExpression("[\n]"));
	mLexemes.insert(integer, QRegularExpression("\\d+"));
	mLexemes.insert(identifier, QRegularExpression("[a-zA-Z][a-zA-Z0-9_]+"));
}

void Lexemes::redefine(Type lexemeType, QRegularExpression const &regExp)
{
	mLexemes.insert(lexemeType, regExp);
}

QHash<Lexemes::Type, QRegularExpression> const &Lexemes::lexemes() const
{
	return mLexemes;
}

#include "textLanguageParser/lexemes.h"

using namespace textLanguageParser;

Lexemes::Lexemes()
{
	mLexemes.insert(whitespace, QRegularExpression("[ \t]+"));
	mLexemes.insert(newline, QRegularExpression("[\n]"));
	mLexemes.insert(identifier, QRegularExpression("[a-zA-Z_][a-zA-Z0-9_]*"));

	mKeywords.insert(andKeyword, "and");
	mKeywords.insert(breakKeyword, "break");
	mKeywords.insert(doKeyword, "do");
	mKeywords.insert(elseKeyword, "else");
	mKeywords.insert(elseifKeyword, "elseif");
	mKeywords.insert(endKeyword, "end");
	mKeywords.insert(falseKeyword, "false");
	mKeywords.insert(forKeyword, "for");
	mKeywords.insert(functionKeyword, "function");
	mKeywords.insert(gotoKeyword, "goto");
	mKeywords.insert(ifKeyword, "if");
	mKeywords.insert(inKeyword, "in");
	mKeywords.insert(localKeyword, "local");
	mKeywords.insert(nilKeyword, "nil");
	mKeywords.insert(notKeyword, "not");
	mKeywords.insert(orKeyword, "or");
	mKeywords.insert(repeatKeyword, "repeat");
	mKeywords.insert(returnKeyword, "return");
	mKeywords.insert(thenKeyword, "then");
	mKeywords.insert(trueKeyword, "true");
	mKeywords.insert(untilKeyword, "until");
	mKeywords.insert(whileKeyword, "while");

	mLexemes.insert(plus, QRegularExpression("\\+"));
	mLexemes.insert(minus, QRegularExpression("-"));
	mLexemes.insert(asterick, QRegularExpression("\\*"));
	mLexemes.insert(slash, QRegularExpression("/"));
	mLexemes.insert(percent, QRegularExpression("%"));
	mLexemes.insert(hat, QRegularExpression("\\^"));
	mLexemes.insert(sharp, QRegularExpression("#"));

	mLexemes.insert(ampersand, QRegularExpression("&"));
	mLexemes.insert(tilda, QRegularExpression("~"));
	mLexemes.insert(verticalLine, QRegularExpression("\\|"));
	mLexemes.insert(doubleLess, QRegularExpression("<<"));
	mLexemes.insert(doubleGreater, QRegularExpression(">>"));
	mLexemes.insert(doubleSlash, QRegularExpression("//"));

	mLexemes.insert(doubleEquals, QRegularExpression("=="));
	mLexemes.insert(tildaEquals, QRegularExpression("~="));
	mLexemes.insert(lessEquals, QRegularExpression("<="));
	mLexemes.insert(greaterEquals, QRegularExpression(">="));
	mLexemes.insert(less, QRegularExpression("<"));
	mLexemes.insert(greater, QRegularExpression(">"));
	mLexemes.insert(equals, QRegularExpression("="));

	mLexemes.insert(openingBracket, QRegularExpression("\\("));
	mLexemes.insert(closingBracket, QRegularExpression("\\)"));
	mLexemes.insert(openingCurlyBracket, QRegularExpression("{"));
	mLexemes.insert(closingCurlyBracket, QRegularExpression("}"));
	mLexemes.insert(openingSquareBracket, QRegularExpression("\\["));
	mLexemes.insert(closingSquareBracket, QRegularExpression("\\]"));
	mLexemes.insert(doubleColon, QRegularExpression("::"));

	mLexemes.insert(semicolon, QRegularExpression(";"));
	mLexemes.insert(colon, QRegularExpression(":"));
	mLexemes.insert(comma, QRegularExpression(","));
	mLexemes.insert(dot, QRegularExpression("\\."));
	mLexemes.insert(doubleDot, QRegularExpression("\\.\\."));
	mLexemes.insert(tripleDot, QRegularExpression("\\.\\.\\."));

	mLexemes.insert(string, QRegularExpression("(\"[^\"\\\\]*(\\\\(.|\\n)[^\"\\\\]*)*\")"
			"|('[^'\\\\]*(\\\\(.|\\n)[^'\\\\]*)*')"));

	mLexemes.insert(integerLiteral, QRegularExpression("(0[xX][0-9a-fA-F]+)|([0-9]+)"));
	mLexemes.insert(floatLiteral, QRegularExpression(
			"(0[xX][0-9a-fA-F]+("
					"(\\.[0-9a-fA-F]+[pP](([+-][0-9a-fA-F]+)|([0-9a-fA-F]*)))"
					"|(\\.[0-9a-fA-F]+)"
					"|([pP](([+-][0-9a-fA-F]+)|([0-9a-fA-F]*)))))"
			"|([0-9]+("
					"(\\.[0-9]+)[eE](([+-][0-9]+)|([0-9]*))"
					"|(\\.[0-9]+)"
					"|([eE](([+-][0-9]+)|([0-9]*)))))"
			));

	mLexemes.insert(comment, QRegularExpression("--.*"));
}

void Lexemes::redefine(Type lexemeType, QRegularExpression const &regExp)
{
	mLexemes.insert(lexemeType, regExp);
}

void Lexemes::redefineKeyword(Type keywordType, QString const &keyword)
{
	mKeywords.insert(keywordType, keyword);
}

QList<Lexemes::Type> Lexemes::allLexemes() const
{
	return mLexemes.keys();
}

QRegularExpression Lexemes::lexemeDefinition(Lexemes::Type type) const
{
	return mLexemes.value(type);
}

QList<Lexemes::Type> Lexemes::allKeywords() const
{
	return mKeywords.keys();
}

QString Lexemes::keywordDefinition(Lexemes::Type type) const
{
	return mKeywords.value(type);
}

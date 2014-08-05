#include "textLanguageParser/tokenPatterns.h"

using namespace textLanguageParser;

TokenPatterns::TokenPatterns()
{
	mPatterns.insert(TokenType::whitespace, QRegularExpression("[ \t]+"));
	mPatterns.insert(TokenType::newline, QRegularExpression("[\n]"));
	mPatterns.insert(TokenType::identifier, QRegularExpression("[a-zA-Z_][a-zA-Z0-9_]*"));

	mKeywords.insert(TokenType::andKeyword, "and");
	mKeywords.insert(TokenType::breakKeyword, "break");
	mKeywords.insert(TokenType::doKeyword, "do");
	mKeywords.insert(TokenType::elseKeyword, "else");
	mKeywords.insert(TokenType::elseifKeyword, "elseif");
	mKeywords.insert(TokenType::endKeyword, "end");
	mKeywords.insert(TokenType::falseKeyword, "false");
	mKeywords.insert(TokenType::forKeyword, "for");
	mKeywords.insert(TokenType::functionKeyword, "function");
	mKeywords.insert(TokenType::gotoKeyword, "goto");
	mKeywords.insert(TokenType::ifKeyword, "if");
	mKeywords.insert(TokenType::inKeyword, "in");
	mKeywords.insert(TokenType::localKeyword, "local");
	mKeywords.insert(TokenType::nilKeyword, "nil");
	mKeywords.insert(TokenType::notKeyword, "not");
	mKeywords.insert(TokenType::orKeyword, "or");
	mKeywords.insert(TokenType::repeatKeyword, "repeat");
	mKeywords.insert(TokenType::returnKeyword, "return");
	mKeywords.insert(TokenType::thenKeyword, "then");
	mKeywords.insert(TokenType::trueKeyword, "true");
	mKeywords.insert(TokenType::untilKeyword, "until");
	mKeywords.insert(TokenType::whileKeyword, "while");

	mPatterns.insert(TokenType::plus, QRegularExpression("\\+"));
	mPatterns.insert(TokenType::minus, QRegularExpression("-"));
	mPatterns.insert(TokenType::asterick, QRegularExpression("\\*"));
	mPatterns.insert(TokenType::slash, QRegularExpression("/"));
	mPatterns.insert(TokenType::percent, QRegularExpression("%"));
	mPatterns.insert(TokenType::hat, QRegularExpression("\\^"));
	mPatterns.insert(TokenType::sharp, QRegularExpression("#"));

	mPatterns.insert(TokenType::ampersand, QRegularExpression("&"));
	mPatterns.insert(TokenType::tilda, QRegularExpression("~"));
	mPatterns.insert(TokenType::verticalLine, QRegularExpression("\\|"));
	mPatterns.insert(TokenType::doubleLess, QRegularExpression("<<"));
	mPatterns.insert(TokenType::doubleGreater, QRegularExpression(">>"));
	mPatterns.insert(TokenType::doubleSlash, QRegularExpression("//"));

	mPatterns.insert(TokenType::doubleEquals, QRegularExpression("=="));
	mPatterns.insert(TokenType::tildaEquals, QRegularExpression("~="));
	mPatterns.insert(TokenType::lessEquals, QRegularExpression("<="));
	mPatterns.insert(TokenType::greaterEquals, QRegularExpression(">="));
	mPatterns.insert(TokenType::less, QRegularExpression("<"));
	mPatterns.insert(TokenType::greater, QRegularExpression(">"));
	mPatterns.insert(TokenType::equals, QRegularExpression("="));

	mPatterns.insert(TokenType::openingBracket, QRegularExpression("\\("));
	mPatterns.insert(TokenType::closingBracket, QRegularExpression("\\)"));
	mPatterns.insert(TokenType::openingCurlyBracket, QRegularExpression("{"));
	mPatterns.insert(TokenType::closingCurlyBracket, QRegularExpression("}"));
	mPatterns.insert(TokenType::openingSquareBracket, QRegularExpression("\\["));
	mPatterns.insert(TokenType::closingSquareBracket, QRegularExpression("\\]"));
	mPatterns.insert(TokenType::doubleColon, QRegularExpression("::"));

	mPatterns.insert(TokenType::semicolon, QRegularExpression(";"));
	mPatterns.insert(TokenType::colon, QRegularExpression(":"));
	mPatterns.insert(TokenType::comma, QRegularExpression(","));
	mPatterns.insert(TokenType::dot, QRegularExpression("\\."));
	mPatterns.insert(TokenType::doubleDot, QRegularExpression("\\.\\."));
	mPatterns.insert(TokenType::tripleDot, QRegularExpression("\\.\\.\\."));

	mPatterns.insert(TokenType::string, QRegularExpression("(\"[^\"\\\\]*(\\\\(.|\\n)[^\"\\\\]*)*\")"
			"|('[^'\\\\]*(\\\\(.|\\n)[^'\\\\]*)*')"));

	mPatterns.insert(TokenType::integerLiteral, QRegularExpression("(0[xX][0-9a-fA-F]+)|([0-9]+)"));
	mPatterns.insert(TokenType::floatLiteral, QRegularExpression(
			"(0[xX][0-9a-fA-F]+("
					"(\\.[0-9a-fA-F]+[pP](([+-][0-9a-fA-F]+)|([0-9a-fA-F]*)))"
					"|(\\.[0-9a-fA-F]+)"
					"|([pP](([+-][0-9a-fA-F]+)|([0-9a-fA-F]*)))))"
			"|([0-9]+("
					"(\\.[0-9]+)[eE](([+-][0-9]+)|([0-9]*))"
					"|(\\.[0-9]+)"
					"|([eE](([+-][0-9]+)|([0-9]*)))))"
			));

	mPatterns.insert(TokenType::comment, QRegularExpression("--.*"));
}

void TokenPatterns::redefine(TokenType lexemeType, QRegularExpression const &regExp)
{
	mPatterns.insert(lexemeType, regExp);
}

void TokenPatterns::redefineKeyword(TokenType keywordType, QString const &keyword)
{
	mKeywords.insert(keywordType, keyword);
}

QList<TokenType> TokenPatterns::allPatterns() const
{
	return mPatterns.keys();
}

QRegularExpression TokenPatterns::tokenPattern(TokenType type) const
{
	return mPatterns.value(type);
}

QList<TokenType> TokenPatterns::allKeywords() const
{
	return mKeywords.keys();
}

QString TokenPatterns::keywordPattern(TokenType type) const
{
	return mKeywords.value(type);
}

#include "qrtext/src/lua/luaLexer.h"

using namespace qrtext::lua;
using namespace qrtext::lua::details;
using namespace qrtext::core;

LuaLexer::LuaLexer(QList<core::Error> &errors)
	: Lexer<LuaTokenTypes>(initPatterns(), errors)
{
}

TokenPatterns<LuaTokenTypes> LuaLexer::initPatterns()
{
	TokenPatterns<LuaTokenTypes> tokenDefinitions;

	tokenDefinitions.defineToken(LuaTokenTypes::whitespace, QRegularExpression("[ \t]+"));
	tokenDefinitions.defineToken(LuaTokenTypes::newline, QRegularExpression("[\n]"));
	tokenDefinitions.defineToken(LuaTokenTypes::identifier, QRegularExpression(R"([\p{L}_][\p{L}0-9_]*)"));

	tokenDefinitions.defineKeyword(LuaTokenTypes::andKeyword, "and");
	tokenDefinitions.defineKeyword(LuaTokenTypes::breakKeyword, "break");
	tokenDefinitions.defineKeyword(LuaTokenTypes::doKeyword, "do");
	tokenDefinitions.defineKeyword(LuaTokenTypes::elseKeyword, "else");
	tokenDefinitions.defineKeyword(LuaTokenTypes::elseifKeyword, "elseif");
	tokenDefinitions.defineKeyword(LuaTokenTypes::endKeyword, "end");
	tokenDefinitions.defineKeyword(LuaTokenTypes::falseKeyword, "false");
	tokenDefinitions.defineKeyword(LuaTokenTypes::forKeyword, "for");
	tokenDefinitions.defineKeyword(LuaTokenTypes::functionKeyword, "function");
	tokenDefinitions.defineKeyword(LuaTokenTypes::gotoKeyword, "goto");
	tokenDefinitions.defineKeyword(LuaTokenTypes::ifKeyword, "if");
	tokenDefinitions.defineKeyword(LuaTokenTypes::inKeyword, "in");
	tokenDefinitions.defineKeyword(LuaTokenTypes::localKeyword, "local");
	tokenDefinitions.defineKeyword(LuaTokenTypes::nilKeyword, "nil");
	tokenDefinitions.defineKeyword(LuaTokenTypes::notKeyword, "not");
	tokenDefinitions.defineKeyword(LuaTokenTypes::orKeyword, "or");
	tokenDefinitions.defineKeyword(LuaTokenTypes::repeatKeyword, "repeat");
	tokenDefinitions.defineKeyword(LuaTokenTypes::returnKeyword, "return");
	tokenDefinitions.defineKeyword(LuaTokenTypes::thenKeyword, "then");
	tokenDefinitions.defineKeyword(LuaTokenTypes::trueKeyword, "true");
	tokenDefinitions.defineKeyword(LuaTokenTypes::untilKeyword, "until");
	tokenDefinitions.defineKeyword(LuaTokenTypes::whileKeyword, "while");

	tokenDefinitions.defineToken(LuaTokenTypes::plus, QRegularExpression("\\+"));
	tokenDefinitions.defineToken(LuaTokenTypes::minus, QRegularExpression("-"));
	tokenDefinitions.defineToken(LuaTokenTypes::asterick, QRegularExpression("\\*"));
	tokenDefinitions.defineToken(LuaTokenTypes::slash, QRegularExpression("/"));
	tokenDefinitions.defineToken(LuaTokenTypes::percent, QRegularExpression("%"));
	tokenDefinitions.defineToken(LuaTokenTypes::hat, QRegularExpression("\\^"));
	tokenDefinitions.defineToken(LuaTokenTypes::sharp, QRegularExpression("#"));

	tokenDefinitions.defineToken(LuaTokenTypes::ampersand, QRegularExpression("&"));
	tokenDefinitions.defineToken(LuaTokenTypes::tilda, QRegularExpression("~"));
	tokenDefinitions.defineToken(LuaTokenTypes::verticalLine, QRegularExpression("\\|"));
	tokenDefinitions.defineToken(LuaTokenTypes::doubleLess, QRegularExpression("<<"));
	tokenDefinitions.defineToken(LuaTokenTypes::doubleGreater, QRegularExpression(">>"));
	tokenDefinitions.defineToken(LuaTokenTypes::doubleSlash, QRegularExpression("//"));

	tokenDefinitions.defineToken(LuaTokenTypes::doubleEquals, QRegularExpression("=="));
	tokenDefinitions.defineToken(LuaTokenTypes::tildaEquals, QRegularExpression("~="));
	tokenDefinitions.defineToken(LuaTokenTypes::exclamationMarkEquals, QRegularExpression("!="));
	tokenDefinitions.defineToken(LuaTokenTypes::lessEquals, QRegularExpression("<="));
	tokenDefinitions.defineToken(LuaTokenTypes::greaterEquals, QRegularExpression(">="));
	tokenDefinitions.defineToken(LuaTokenTypes::less, QRegularExpression("<"));
	tokenDefinitions.defineToken(LuaTokenTypes::greater, QRegularExpression(">"));
	tokenDefinitions.defineToken(LuaTokenTypes::equals, QRegularExpression("="));

	tokenDefinitions.defineToken(LuaTokenTypes::openingBracket, QRegularExpression("\\("));
	tokenDefinitions.defineToken(LuaTokenTypes::closingBracket, QRegularExpression("\\)"));
	tokenDefinitions.defineToken(LuaTokenTypes::openingCurlyBracket, QRegularExpression("{"));
	tokenDefinitions.defineToken(LuaTokenTypes::closingCurlyBracket, QRegularExpression("}"));
	tokenDefinitions.defineToken(LuaTokenTypes::openingSquareBracket, QRegularExpression("\\["));
	tokenDefinitions.defineToken(LuaTokenTypes::closingSquareBracket, QRegularExpression("\\]"));
	tokenDefinitions.defineToken(LuaTokenTypes::doubleColon, QRegularExpression("::"));

	tokenDefinitions.defineToken(LuaTokenTypes::semicolon, QRegularExpression(";"));
	tokenDefinitions.defineToken(LuaTokenTypes::colon, QRegularExpression(":"));
	tokenDefinitions.defineToken(LuaTokenTypes::comma, QRegularExpression(","));
	tokenDefinitions.defineToken(LuaTokenTypes::dot, QRegularExpression("\\."));
	tokenDefinitions.defineToken(LuaTokenTypes::doubleDot, QRegularExpression("\\.\\."));
	tokenDefinitions.defineToken(LuaTokenTypes::tripleDot, QRegularExpression("\\.\\.\\."));

	tokenDefinitions.defineToken(LuaTokenTypes::string, QRegularExpression(R"(("[^"\\]*(\\(.|\n)[^"\\]*)*"))"
			R"(|('[^'\\]*(\\(.|\n)[^'\\]*)*'))"));

	tokenDefinitions.defineToken(LuaTokenTypes::integerLiteral, QRegularExpression("(0[xX][0-9a-fA-F]+)|([0-9]+)"));
	tokenDefinitions.defineToken(LuaTokenTypes::floatLiteral, QRegularExpression(
			"(0[xX][0-9a-fA-F]+("
					"(\\.[0-9a-fA-F]+[pP](([+-][0-9a-fA-F]+)|([0-9a-fA-F]*)))"
					"|(\\.[0-9a-fA-F]+)"
					"|([pP](([+-][0-9a-fA-F]+)|([0-9a-fA-F]*)))))"
			"|([0-9]+("
					"(\\.[0-9]+)[eE](([+-][0-9]+)|([0-9]*))"
					"|(\\.[0-9]+)"
					"|([eE](([+-][0-9]+)|([0-9]*)))))"
			));

	tokenDefinitions.defineToken(LuaTokenTypes::comment, QRegularExpression("--.*"));

	return tokenDefinitions;
}

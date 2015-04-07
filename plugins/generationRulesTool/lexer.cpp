#include "lexer.h"

using namespace qrtext::core;
using namespace simpleParser;

simpleParser::Lexer::Lexer(QList<Error> &errors)
	: qrtext::core::Lexer<TokenTypes>(initPatterns(), errors)
{
}

TokenPatterns<TokenTypes> simpleParser::Lexer::initPatterns()
{
	TokenPatterns<TokenTypes> tokenDefinitions;

	tokenDefinitions.defineToken(TokenTypes::whitespace, QRegularExpression("[ \t]+"), QObject::tr("whitespace"));
	tokenDefinitions.defineToken(TokenTypes::newline, QRegularExpression("[\n]"), QObject::tr("newline"));
	tokenDefinitions.defineToken(TokenTypes::identifier
			, QRegularExpression(R"([\p{L}_][\p{L}0-9_]*)")
			, QObject::tr("identifier"));

	tokenDefinitions.defineToken(TokenTypes::openingBracket, QRegularExpression("\\("), "(");
	tokenDefinitions.defineToken(TokenTypes::closingBracket, QRegularExpression("\\)"), ")");
	tokenDefinitions.defineToken(TokenTypes::openingCurlyBracket, QRegularExpression("{"), "{");
	tokenDefinitions.defineToken(TokenTypes::closingCurlyBracket, QRegularExpression("}"), "}");

	tokenDefinitions.defineToken(TokenTypes::dot, QRegularExpression("\\."), ".");
	tokenDefinitions.defineToken(TokenTypes::comma, QRegularExpression("\\,"), ",");

	tokenDefinitions.defineKeyword(TokenTypes::newlineKeyword, "newline");
	tokenDefinitions.defineKeyword(TokenTypes::foreachKeyword, "foreach");
	tokenDefinitions.defineKeyword(TokenTypes::tabKeyword, "tab");
	tokenDefinitions.defineKeyword(TokenTypes::inKeyword, "in");

	tokenDefinitions.defineKeyword(TokenTypes::callGeneratorForKeyword, "callGeneratorFor");

	tokenDefinitions.defineToken(TokenTypes::text,
			QRegularExpression(R"(('[^'\\]*(\\(.|\n)[^'\\]*)*'))")
			, QObject::tr("text"));

	tokenDefinitions.defineKeyword(TokenTypes::outcomingLinksKeyword, "outcomingLinks");
	tokenDefinitions.defineKeyword(TokenTypes::incomingLinksKeyword, "incomingLinks");
	tokenDefinitions.defineKeyword(TokenTypes::linksKeyword, "links");

	tokenDefinitions.defineKeyword(TokenTypes::transitionEndKeyword, "transitionEnd");

	return tokenDefinitions;
}


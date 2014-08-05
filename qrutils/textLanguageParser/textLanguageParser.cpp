#include "textLanguageParser.h"

#include "details/lexer.h"
#include "tokenType.h"
#include "textLanguageParser/details/simpleParser.h"
#include "textLanguageParser/details/parserCombinators.h"
#include "textLanguageParser/ast/number.h"
#include "textLanguageParser/ast/temporaryToken.h"

using namespace textLanguageParser;
using namespace textLanguageParser::details;

TextLanguageParserInterface::Result TextLanguageParser::parse(QString const &code)
{
	TokenPatterns lexemes;
	details::Lexer lexer(lexemes);

	details::Lexer::Result lexerResult = lexer.tokenize(code);
	mErrors = lexerResult.errors;
	mTokenStream.reset(new details::TokenStream(lexerResult.tokens, mErrors));

//	Result result(nullptr, mErrors);

	// unop ::= ‘-’ | not | ‘#’ | ‘~’
//	AlternativeParser unop = -TokenType::minus | -TokenType::notKeyword | -TokenType::sharp | -TokenType::tilda;

	// primary ::= nil | false | true | Number | String | ‘...’ | prefixexp | tableconstructor | unop exp

	auto number = (-TokenType::integerLiteral | -TokenType::floatLiteral) >> [](ast::TemporaryToken *node) {
		return new ast::Number(node->token().lexeme());
	};

//	AlternativeParser primary = -TokenType::nilKeyword | -TokenType::falseKeyword | -TokenType::trueKeyword
//			| number | -TokenType::string | -TokenType::tripleDot | (unop + number);

	return number.parse(*mTokenStream);
}

void TextLanguageParser::reportError(QString const &message)
{
	auto connection = !mTokenStream->isEnd()
			? mTokenStream->next().range().start()
			: mTokenStream->next().range().end();

	mErrors << ParserError(connection, message, ErrorType::syntaxError, Severity::error);
}

//void TextLanguageParser::primary()
//{
//	if (mTokenStream->isEnd()) {
//		reportError("Unexpected end of file");
//		return;
//	}

//	switch (mTokenStream->next().token()) {
//	case TokenType::integerLiteral:
//		break;
//	default:
//		break;
//	}
//}

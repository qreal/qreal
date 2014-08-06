#include "textLanguageParser.h"

#include "details/lexer.h"
#include "tokenType.h"
#include "textLanguageParser/details/simpleParser.h"
#include "textLanguageParser/details/parserCombinators.h"
#include "textLanguageParser/ast/number.h"
#include "textLanguageParser/ast/temporaryToken.h"
#include "textLanguageParser/ast/temporaryPair.h"
#include "textLanguageParser/ast/unaryOperator.h"

using namespace textLanguageParser;
using namespace textLanguageParser::details;

TextLanguageParserInterface::Result TextLanguageParser::parse(QString const &code)
{
	TokenPatterns lexemes;
	details::Lexer lexer(lexemes);

	details::Lexer::Result lexerResult = lexer.tokenize(code);
	mErrors = lexerResult.errors;
	mTokenStream.reset(new details::TokenStream(lexerResult.tokens, mErrors));

	// unop ::= ‘-’ | not | ‘#’ | ‘~’
	auto unop = -TokenType::minus | -TokenType::notKeyword | -TokenType::sharp | -TokenType::tilda;

	// Number is a helper production to avoid differing between integer and float. It will be removed when type
	// inference and interpretation will be needed.
	auto number = (-TokenType::integerLiteral | -TokenType::floatLiteral) >> [](ast::TemporaryToken *node) {
		return new ast::Number(node->token().lexeme());
	};

	// primary ::= nil | false | true | Number | String | ‘...’ | prefixexp | tableconstructor | unop exp
	auto primary = -TokenType::nilKeyword
			| -TokenType::falseKeyword
			| -TokenType::trueKeyword
			| number
			| -TokenType::string
			| -TokenType::tripleDot
			| ((unop + number) >> [](ast::TemporaryPair *node) {
						QSharedPointer<ast::TemporaryToken> temporaryToken
								= node->left().dynamicCast<ast::TemporaryToken>();
						ast::UnaryOperator::Type type = ast::UnaryOperator::Type::minus;
						switch (temporaryToken->token().token()) {
						case TokenType::minus:
							type = ast::UnaryOperator::Type::minus;
							break;
						case TokenType::notKeyword:
							type = ast::UnaryOperator::Type::notOperator;
							break;
						case TokenType::sharp:
							type = ast::UnaryOperator::Type::sharp;
							break;
						case TokenType::tilda:
							type = ast::UnaryOperator::Type::tilda;
							break;
						default:
							/// @todo Report internal parser error.
							break;
						}

						return new ast::UnaryOperator(type, node->right());
					})
			;

	return primary.parse(*mTokenStream);
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

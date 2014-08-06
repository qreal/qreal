#include "textLanguageParser.h"

#include "details/lexer.h"
#include "tokenType.h"
#include "textLanguageParser/details/simpleParser.h"
#include "textLanguageParser/details/expressionParser.h"
#include "textLanguageParser/details/parserCombinators.h"
#include "textLanguageParser/details/parserRef.h"
#include "textLanguageParser/ast/number.h"
#include "textLanguageParser/ast/temporaryToken.h"
#include "textLanguageParser/ast/temporaryPair.h"
#include "textLanguageParser/ast/unaryOperator.h"

#include "textLanguageParser/ast/addition.h"
#include "textLanguageParser/ast/subtraction.h"
#include "textLanguageParser/ast/multiplication.h"
#include "textLanguageParser/ast/division.h"
#include "textLanguageParser/ast/exponentiation.h"

#include "textLanguageParser/ast/unaryMinus.h"
#include "textLanguageParser/ast/not.h"
#include "textLanguageParser/ast/length.h"
#include "textLanguageParser/ast/bitwiseNegation.h"

using namespace textLanguageParser;
using namespace textLanguageParser::details;

TextLanguageParserInterface::Result TextLanguageParser::parse(QString const &code)
{
	TokenPatterns lexemes;
	details::Lexer lexer(lexemes);

	details::Lexer::Result lexerResult = lexer.tokenize(code);
	mErrors = lexerResult.errors;
	mTokenStream.reset(new details::TokenStream(lexerResult.tokens, mErrors));

	ParserRef primary;
	ParserRef binOp;

	// exp(precedence) ::= primary { binop exp(newPrecedence) }
	auto exp = ParserRef(new ExpressionParser(false, primary, binOp));

	auto createTemporary = [] (Token token) { return new ast::TemporaryToken(token); };

	// unop ::= ‘-’ | not | ‘#’ | ‘~’
	auto unop = TokenType::minus >> [] (Token token) { Q_UNUSED(token); return new ast::UnaryMinus(); }
			| TokenType::notKeyword >> [] (Token token) { Q_UNUSED(token); return new ast::Not(); }
			| TokenType::sharp >> [] (Token token) { Q_UNUSED(token); return new ast::Length(); }
			| TokenType::tilda >> [] (Token token) { Q_UNUSED(token); return new ast::BitwiseNegation(); }
			;

	// binop ::= ‘+’ | ‘-’ | ‘*’ | ‘/’ | ‘//’ | ‘^’ | ‘%’ | ‘&’ | ‘~’ | ‘|’ | ‘>>’ | ‘<<’ | ‘..’
	//           | ‘<’ | ‘<=’ | ‘>’ | ‘>=’ | ‘==’ | ‘~=’ | and | or
	binOp = TokenType::plus >> [] (Token token) { Q_UNUSED(token); return new ast::Addition(); }
			| TokenType::minus >> [] (Token token) { Q_UNUSED(token); return new ast::Subtraction(); }
			| TokenType::asterick >> [] (Token token) { Q_UNUSED(token); return new ast::Multiplication(); }
			| TokenType::slash >> [] (Token token) { Q_UNUSED(token); return new ast::Division(); }
			| TokenType::hat >> [] (Token token) { Q_UNUSED(token); return new ast::Exponentiation(); }
			;

	// Number is a helper production to avoid differing between integer and float. It will be removed when type
	// inference and interpretation will be needed.
	auto number = TokenType::integerLiteral >> [] (Token token) { return new ast::Number(token.lexeme()); }
			| TokenType::floatLiteral >> [] (Token token) { return new ast::Number(token.lexeme()); }
			;

	// primary ::= nil | false | true | Number | String | ‘...’ | prefixexp | tableconstructor | unop exp
	primary =
			TokenType::nilKeyword >> createTemporary
			| TokenType::falseKeyword >> createTemporary
			| TokenType::trueKeyword >> createTemporary
			| number
			| TokenType::string >> createTemporary
			| TokenType::tripleDot >> createTemporary
			| ((unop + ParserRef(new ExpressionParser(true, primary, binOp)))
					>> [](ast::TemporaryPair *node) {
						QSharedPointer<ast::UnaryOperator> unOp = node->left().dynamicCast<ast::UnaryOperator>();
						unOp->setOperand(node->right());
						return unOp;
					})
			;

	return exp->parse(*mTokenStream);
}

void TextLanguageParser::reportError(QString const &message)
{
	auto connection = !mTokenStream->isEnd()
			? mTokenStream->next().range().start()
			: mTokenStream->next().range().end();

	mErrors << ParserError(connection, message, ErrorType::syntaxError, Severity::error);
}

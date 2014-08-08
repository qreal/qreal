#include "textLanguageParser.h"

#include "details/lexer.h"
#include "tokenType.h"
#include "textLanguageParser/details/simpleParser.h"
#include "textLanguageParser/details/expressionParser.h"
#include "textLanguageParser/details/parserCombinators.h"
#include "textLanguageParser/details/parserRef.h"
#include "textLanguageParser/details/temporaryToken.h"
#include "textLanguageParser/details/temporaryPair.h"
#include "textLanguageParser/details/temporaryList.h"

#include "textLanguageParser/ast/expression.h"
#include "textLanguageParser/ast/unaryOperator.h"
#include "textLanguageParser/ast/fieldInitialization.h"
#include "textLanguageParser/ast/tableConstructor.h"

#include "textLanguageParser/ast/addition.h"
#include "textLanguageParser/ast/subtraction.h"
#include "textLanguageParser/ast/multiplication.h"
#include "textLanguageParser/ast/division.h"
#include "textLanguageParser/ast/exponentiation.h"

#include "textLanguageParser/ast/unaryMinus.h"
#include "textLanguageParser/ast/not.h"
#include "textLanguageParser/ast/length.h"
#include "textLanguageParser/ast/bitwiseNegation.h"

#include "textLanguageParser/ast/integerNumber.h"
#include "textLanguageParser/ast/floatNumber.h"
#include "textLanguageParser/ast/string.h"
#include "textLanguageParser/ast/nil.h"
#include "textLanguageParser/ast/true.h"
#include "textLanguageParser/ast/false.h"

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

	ParserRef tableconstructor;
	ParserRef fieldlist;
	ParserRef field;
	ParserRef fieldsep;
	ParserRef binop;
	ParserRef unop;

	// exp(precedence) ::= primary { binop exp(newPrecedence) }
	auto exp = ParserRef(new ExpressionParser(false, primary, binop));

	auto reportUnsupported = [this] (Token const &token) {
		mErrors << ParserError(token.range().start()
				, "This construction is not supported yet"
				, ErrorType::syntaxError
				, Severity::error);

		return new ast::Node();
	};

	// primary ::= nil | false | true | Number | String | ‘...’ | prefixexp | tableconstructor | unop exp
	primary =
			TokenType::nilKeyword >> [] { return new ast::Nil(); }
			| TokenType::falseKeyword >> [] { return new ast::True(); }
			| TokenType::trueKeyword >> [] { return new ast::False(); }
			| TokenType::integerLiteral >> [] (Token token) { return new ast::IntegerNumber(token.lexeme()); }
			| TokenType::floatLiteral >> [] (Token token) { return new ast::FloatNumber(token.lexeme()); }
			| TokenType::string >> [] (Token token) { return new ast::String(token.lexeme()); }
			| TokenType::tripleDot >> reportUnsupported
			// | prefixexp
			| tableconstructor
			| (unop & ParserRef(new ExpressionParser(true, primary, binop)))
					>> [] (QSharedPointer<TemporaryPair> node) {
						auto unOp = as<ast::UnaryOperator>(node->left());
						unOp->setOperand(node->right());
						return unOp;
					}
			;

	// tableconstructor ::= ‘{’ [fieldlist] ‘}’
	tableconstructor = (!TokenType::openingCurlyBracket & ~fieldlist & !TokenType::closingCurlyBracket)
			>> [] (QSharedPointer<TemporaryList> fieldList) {
				if (!fieldList) {
					return wrap(new ast::TableConstructor({}));
				} else {
					return wrap(new ast::TableConstructor(as<ast::FieldInitialization>(fieldList->list())));
				}
			};

	// fieldlist ::= field {fieldsep field} [fieldsep]
	fieldlist = (field & *(!fieldsep & field) & !~fieldsep)
		 >> [] (QSharedPointer<TemporaryPair> node) {
			auto firstField = as<ast::FieldInitialization>(node->left());
			auto temporaryList = as<TemporaryList>(node->right());
			temporaryList->list() << firstField;
			return temporaryList;
		};

	field = (!TokenType::openingSquareBracket & exp & !TokenType::closingSquareBracket & !TokenType::equals & exp)
					>> [] (QSharedPointer<TemporaryPair> pair) {
						auto initializer = as<ast::Expression>(pair->right());
						auto indexer = as<ast::Expression>(pair->left());
						return wrap(new ast::FieldInitialization(initializer, indexer));
					}
			| (exp & ~(!TokenType::equals & exp))
					>> [] (QSharedPointer<TemporaryPair> node) {
							auto const left = as<ast::Expression>(node->left());
							if (!node->right()) {
								return wrap(new ast::FieldInitialization(left));
							} else {
								auto const initializer = as<ast::Expression>(node->right());
								/// @todo Report error if "left" is something different from Name.
								return wrap(new ast::FieldInitialization(left, initializer));
							}
						}
			;

	// fieldsep ::= ‘,’ | ‘;’
	fieldsep = !TokenType::comma
			| !TokenType::semicolon
			;

	// binop ::= ‘+’ | ‘-’ | ‘*’ | ‘/’ | ‘//’ | ‘^’ | ‘%’ | ‘&’ | ‘~’ | ‘|’ | ‘>>’ | ‘<<’ | ‘..’
	//           | ‘<’ | ‘<=’ | ‘>’ | ‘>=’ | ‘==’ | ‘~=’ | and | or
	binop = TokenType::plus >> [] { return new ast::Addition(); }
			| TokenType::minus >> [] { return new ast::Subtraction(); }
			| TokenType::asterick >> [] { return new ast::Multiplication(); }
			| TokenType::slash >> [] { return new ast::Division(); }
			| TokenType::hat >> [] { return new ast::Exponentiation(); }
			;

	// unop ::= ‘-’ | not | ‘#’ | ‘~’
	unop = TokenType::minus >> [] { return new ast::UnaryMinus(); }
			| TokenType::notKeyword >> [] { return new ast::Not(); }
			| TokenType::sharp >> [] { return new ast::Length(); }
			| TokenType::tilda >> [] { return new ast::BitwiseNegation(); }
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

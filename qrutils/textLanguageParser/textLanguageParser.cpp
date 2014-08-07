#include "textLanguageParser.h"

#include "details/lexer.h"
#include "tokenType.h"
#include "textLanguageParser/details/simpleParser.h"
#include "textLanguageParser/details/expressionParser.h"
#include "textLanguageParser/details/parserCombinators.h"
#include "textLanguageParser/details/parserRef.h"
#include "textLanguageParser/ast/temporaryToken.h"
#include "textLanguageParser/ast/temporaryPair.h"
#include "textLanguageParser/ast/temporaryList.h"

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

#include "textLanguageParser/ast/number.h"

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

	auto discard = [] (Token token) { return new ast::TemporaryToken(token); };

	// Number is a helper production to avoid differing between integer and float. It will be removed when type
	// inference and interpretation will be needed.
	auto number = TokenType::integerLiteral >> [] (Token token) { return new ast::Number(token.lexeme()); }
			| TokenType::floatLiteral >> [] (Token token) { return new ast::Number(token.lexeme()); }
			;

	// primary ::= nil | false | true | Number | String | ‘...’ | prefixexp | tableconstructor | unop exp
	primary =
			TokenType::nilKeyword >> discard
			| TokenType::falseKeyword >> discard
			| TokenType::trueKeyword >> discard
			| number
			| TokenType::string >> discard
			| TokenType::tripleDot >> discard
			// | prefixexp
			| tableconstructor
			| ((unop + ParserRef(new ExpressionParser(true, primary, binop)))
					>> [] (QSharedPointer<ast::TemporaryPair> node) {
						QSharedPointer<ast::UnaryOperator> unOp = node->left().dynamicCast<ast::UnaryOperator>();
						unOp->setOperand(node->right());
						return unOp;
					})
			;

	// tableconstructor ::= ‘{’ [fieldlist] ‘}’
	tableconstructor = ((TokenType::openingCurlyBracket >> discard)
			+ ~fieldlist
			+ (TokenType::closingCurlyBracket >> discard))
			>> [] (QSharedPointer<ast::TemporaryPair> node) {
				auto outerPair = node->left().dynamicCast<ast::TemporaryPair>();
				if (!outerPair) {
					return QSharedPointer<ast::TableConstructor>(new ast::TableConstructor({}));
				} else {
					auto fieldList = outerPair->right().dynamicCast<ast::TemporaryList>();
					QList<QSharedPointer<ast::FieldInitialization>> initializersList;
					for (auto initializer : fieldList->list()) {
						initializersList << initializer.staticCast<ast::FieldInitialization>();
					}

					return QSharedPointer<ast::TableConstructor>(new ast::TableConstructor(initializersList));
				}
			};

	fieldlist = (field + *(fieldsep + field) + ~fieldsep)
		 >> [] (QSharedPointer<ast::Node> node) {
			auto outerPair = node.dynamicCast<ast::TemporaryPair>();
			if (!outerPair->right().dynamicCast<ast::TemporaryToken>().isNull()) {
				auto innerPair = outerPair->left().dynamicCast<ast::TemporaryPair>();
				outerPair = innerPair;
			}

			auto firstField = outerPair->left().dynamicCast<ast::FieldInitialization>();
			auto temporaryList = outerPair->right().dynamicCast<ast::TemporaryList>();
			temporaryList->list() << firstField;
			return temporaryList;
		};

	// field ::= ‘[’ exp(0) ‘]’ ‘=’ exp(0) | exp(0) [ ‘=’ exp(0) ]
	field = (
				(((TokenType::openingSquareBracket >> discard)
					+ exp
					+ (TokenType::closingSquareBracket >> discard)
				 ) >> [] (QSharedPointer<ast::TemporaryPair> node) { return node->left(); } )
				+ (((TokenType::equals >> discard)
					+ exp
				) >> [] (QSharedPointer<ast::TemporaryPair> node) { return node->right(); } )
			) >> [] (QSharedPointer<ast::TemporaryPair> node) {
				return QSharedPointer<ast::FieldInitialization>(new ast::FieldInitialization(
						node->left().staticCast<ast::Expression>()
						, node->right().staticCast<ast::Expression>()
					));
			}
			| (exp + ~(((TokenType::equals >> discard) + exp)
						>> [] (QSharedPointer<ast::TemporaryPair> node) { return node->right(); }
					)
				) >> [] (QSharedPointer<ast::Node> node) {
							auto const expression = node.dynamicCast<ast::Expression>();
							if (expression) {
								return QSharedPointer<ast::FieldInitialization>(
										new ast::FieldInitialization(QSharedPointer<ast::Expression>(expression))
										);
							} else {
								/// @todo Report error if node->left() is something different from Name.
								auto pair = node.dynamicCast<ast::TemporaryPair>();
								return QSharedPointer<ast::FieldInitialization>(new ast::FieldInitialization(
											pair->left().staticCast<ast::Expression>()
											, pair->right().staticCast<ast::Expression>()
											));
							}
					}
			;

	// fieldsep ::= ‘,’ | ‘;’
	fieldsep = TokenType::comma >> discard
			| TokenType::semicolon >> discard
			;

	// binop ::= ‘+’ | ‘-’ | ‘*’ | ‘/’ | ‘//’ | ‘^’ | ‘%’ | ‘&’ | ‘~’ | ‘|’ | ‘>>’ | ‘<<’ | ‘..’
	//           | ‘<’ | ‘<=’ | ‘>’ | ‘>=’ | ‘==’ | ‘~=’ | and | or
	binop = TokenType::plus >> [] (Token token) { Q_UNUSED(token); return new ast::Addition(); }
			| TokenType::minus >> [] (Token token) { Q_UNUSED(token); return new ast::Subtraction(); }
			| TokenType::asterick >> [] (Token token) { Q_UNUSED(token); return new ast::Multiplication(); }
			| TokenType::slash >> [] (Token token) { Q_UNUSED(token); return new ast::Division(); }
			| TokenType::hat >> [] (Token token) { Q_UNUSED(token); return new ast::Exponentiation(); }
			;

	// unop ::= ‘-’ | not | ‘#’ | ‘~’
	unop = TokenType::minus >> [] (Token token) { Q_UNUSED(token); return new ast::UnaryMinus(); }
			| TokenType::notKeyword >> [] (Token token) { Q_UNUSED(token); return new ast::Not(); }
			| TokenType::sharp >> [] (Token token) { Q_UNUSED(token); return new ast::Length(); }
			| TokenType::tilda >> [] (Token token) { Q_UNUSED(token); return new ast::BitwiseNegation(); }
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

#include "textLanguageParser/textLanguageParser.h"

#include "textLanguageParser/tokenType.h"
#include "textLanguageParser/details/lexer.h"
#include "textLanguageParser/details/parserRef.h"
#include "textLanguageParser/details/parsers/expressionParser.h"
#include "textLanguageParser/details/parsers/parserCombinators.h"

#include "textLanguageParser/ast/nodes/expression.h"
#include "textLanguageParser/ast/nodes/unaryOperator.h"
#include "textLanguageParser/ast/nodes/fieldInitialization.h"
#include "textLanguageParser/ast/nodes/tableConstructor.h"
#include "textLanguageParser/ast/nodes/functionCall.h"
#include "textLanguageParser/ast/nodes/methodCall.h"
#include "textLanguageParser/ast/nodes/indexingExpression.h"
#include "textLanguageParser/ast/nodes/assignment.h"
#include "textLanguageParser/ast/nodes/block.h"

#include "textLanguageParser/ast/nodes/addition.h"
#include "textLanguageParser/ast/nodes/subtraction.h"
#include "textLanguageParser/ast/nodes/multiplication.h"
#include "textLanguageParser/ast/nodes/division.h"
#include "textLanguageParser/ast/nodes/integerDivision.h"
#include "textLanguageParser/ast/nodes/exponentiation.h"

#include "textLanguageParser/ast/nodes/unaryMinus.h"
#include "textLanguageParser/ast/nodes/not.h"
#include "textLanguageParser/ast/nodes/length.h"
#include "textLanguageParser/ast/nodes/bitwiseNegation.h"

#include "textLanguageParser/ast/nodes/integerNumber.h"
#include "textLanguageParser/ast/nodes/floatNumber.h"
#include "textLanguageParser/ast/nodes/string.h"
#include "textLanguageParser/ast/nodes/nil.h"
#include "textLanguageParser/ast/nodes/true.h"
#include "textLanguageParser/ast/nodes/false.h"

using namespace textLanguageParser;
using namespace textLanguageParser::details;

TextLanguageParserInterface::Result TextLanguageParser::parse(QString const &code)
{
	TokenPatterns lexemes;
	details::Lexer lexer(lexemes);

	details::Lexer::Result lexerResult = lexer.tokenize(code);
	mErrors = lexerResult.errors;
	mTokenStream.reset(new details::TokenStream(lexerResult.tokens, mErrors));

	ParserContext context(mErrors, *mTokenStream);

	ParserRef stat;
	ParserRef explist;
	ParserRef exp;
	ParserRef primary;
	ParserRef prefixexp;
	ParserRef varpart;
	ParserRef functioncallpart;
	ParserRef prefixterm;
	ParserRef args;
	ParserRef tableconstructor;
	ParserRef fieldlist;
	ParserRef field;
	ParserRef fieldsep;
	ParserRef binop;
	ParserRef unop;

	auto reportUnsupported = [&context] (Token const &token) {
		context.reportError(token, "This construction is not supported yet");
		return new ast::Node();
	};

	// Additional production that parses unqualified identifier.
	auto identifier = TokenType::identifier >> [] (Token const &token) { return new ast::Identifier(token.lexeme()); };

	// block ::= {stat}
	auto block = *stat
			>> [] (QSharedPointer<TemporaryList> statList) {
				QList<QSharedPointer<ast::Node>> result;
				for (auto const &stat : statList->list()) {
					if (stat->is<TemporaryList>()) {
						// It is a list of assignments.
						for (auto const &assignment : as<TemporaryList>(stat)->list()) {
							result << assignment;
						}
					} else {
						// It is expression.
						result << stat;
					}
				}

				if (result.size() == 1) {
					// Do not create Block node for simple expression.
					return result.first();
				} else {
					return wrap(new ast::Block(result));
				}
			};

	// stat ::= ‘;’ | explist [‘=’ explist]
	stat = (!TokenType::semicolon | (explist & ~(!TokenType::equals & explist)))
			>> [&context] (QSharedPointer<TemporaryPair> pair) {
				if (!pair) {
					// It is semicolon, just discard it.
					return wrap(nullptr);
				}

				if (!pair->right()) {
					auto expList = as<TemporaryList>(pair->left());
					if (expList->list().size() != 1) {
						// It is a list of expressions which we translate as table constructor, to support convenient
						// lists syntax, for example, for lists of ports in robots.
						QList<QSharedPointer<ast::Node>> fields;
						for (auto exp : expList->list()) {
							fields << wrap(new ast::FieldInitialization(as<ast::Expression>(exp)));
						}

						return wrap(new ast::TableConstructor(as<ast::FieldInitialization>(fields)));
					} else {
						return expList->list().first();
					}
				}

				auto variables = as<TemporaryList>(pair->left())->list();
				auto values = as<TemporaryList>(pair->right())->list();
				if (variables.size() != values.size()) {
					context.reportError(pair, QObject::tr(
							"Number of variables in assignment shall be equal to the number of assigned values"));

					return wrap(nullptr);
				}

				auto result = QSharedPointer<TemporaryList>(new TemporaryList());

				for (int i = 0; i < variables.size(); ++i) {
					auto variable = as<ast::Expression>(variables[i]);

					if (variable->is<ast::FunctionCall>()) {
						context.reportError(pair, QObject::tr("Assignment to function call is impossible"));
						continue;
					}

					auto value = as<ast::Expression>(values[i]);
					result->list() << wrap(new ast::Assignment(variable, value));
				}

				return as<ast::Node>(result);
			};

	// explist ::= exp(0) {‘,’ exp(0)}
	explist = (exp & *(!TokenType::comma & exp))
			>> [] (QSharedPointer<TemporaryPair> node) {
				auto firstExp = as<ast::Expression>(node->left());
				auto temporaryList = as<TemporaryList>(node->right());
				temporaryList->list().prepend(firstExp);
				return temporaryList;
			};

	// exp(precedence) ::= primary { binop exp(newPrecedence) }
	exp = ParserRef(new ExpressionParser(false, primary, binop));

	// primary ::= nil | false | true | Number | String | ‘...’ | prefixexp | tableconstructor | unop exp
	primary =
			TokenType::nilKeyword >> [] { return new ast::Nil(); }
			| TokenType::falseKeyword >> [] { return new ast::True(); }
			| TokenType::trueKeyword >> [] { return new ast::False(); }
			| TokenType::integerLiteral >> [] (Token token) { return new ast::IntegerNumber(token.lexeme()); }
			| TokenType::floatLiteral >> [] (Token token) { return new ast::FloatNumber(token.lexeme()); }
			| TokenType::string >> [] (Token token) { return new ast::String(token.lexeme()); }
			| TokenType::tripleDot >> reportUnsupported
			| prefixexp
			| tableconstructor
			| (unop & ParserRef(new ExpressionParser(true, primary, binop)))
					>> [] (QSharedPointer<TemporaryPair> node) {
						auto unOp = as<ast::UnaryOperator>(node->left());
						unOp->setOperand(node->right());
						return unOp;
					}
			;

	// prefixexp ::= prefixterm { functioncallpart | varpart }
	prefixexp = (prefixterm & *(functioncallpart | varpart))
			>> [] (QSharedPointer<TemporaryPair> node) {
				auto result = as<ast::Expression>(node->left());
				for (auto const part : as<TemporaryList>(node->right())->list()) {
					if (part->is<ast::Expression>()) {
						// It is varpart (indexing expression)
						result = QSharedPointer<ast::Expression>(
								new ast::IndexingExpression(result, as<ast::Expression>(part)));
					} else if (part->is<TemporaryPair>()) {
						// It is functioncallpart (method call)
						auto const methodName = as<ast::Identifier>(as<TemporaryPair>(part)->left());
						auto const args = as<ast::Expression>(
								as<TemporaryList>(as<TemporaryPair>(part)->right())->list());

						result = QSharedPointer<ast::Expression>(new ast::MethodCall(result, methodName, args));
					} else if (part->is<TemporaryList>()) {
						// It is functioncallpart (function call)
						result = QSharedPointer<ast::Expression>(
								new ast::FunctionCall(result, as<ast::Expression>(as<TemporaryList>(part)->list())));
					}
				}

				return result;
			};

	// varpart ::= ‘[’ exp(0) ‘]’ | ‘.’ Name
	varpart = (!TokenType::openingSquareBracket & exp & !TokenType::closingSquareBracket)
			| (!TokenType::dot & TokenType::identifier >> [] (Token const &token) {
						return new ast::String(token.lexeme());
					}
			)
			;

	// functioncallpart :: = args | ‘:’ Name args
	functioncallpart = args | (!TokenType::colon & identifier & args);

	// prefixterm ::= Name | ‘(’ exp(0) ‘)’
	prefixterm = identifier
			| (!TokenType::openingBracket & exp & !TokenType::closingBracket)
			;

	// args ::= ‘(’ [explist] ‘)’ | tableconstructor | String
	args = ((!TokenType::openingBracket & ~explist & !TokenType::closingBracket)
			| tableconstructor
			| TokenType::string >> [] (Token token) { return new ast::String(token.lexeme()); }
			) >> [] (QSharedPointer<ast::Node> node) {
					if (node && node->is<TemporaryList>()) {
						return node;
					} else {
						auto result = QSharedPointer<TemporaryList>(new TemporaryList());
						if (node) {
							result->list() << as<ast::Expression>(node);
						}

						return as<ast::Node>(result);
					}
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
				temporaryList->list().prepend(firstField);
				return temporaryList;
			};

	// field ::= ‘[’ exp(0) ‘]’ ‘=’ exp(0) | exp(0) [ ‘=’ exp(0) ]
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
			| TokenType::doubleSlash >> [] { return new ast::IntegerDivision(); }
			| TokenType::hat >> [] { return new ast::Exponentiation(); }
			;

	// unop ::= ‘-’ | not | ‘#’ | ‘~’
	unop = TokenType::minus >> [] { return new ast::UnaryMinus(); }
			| TokenType::notKeyword >> [] { return new ast::Not(); }
			| TokenType::sharp >> [] { return new ast::Length(); }
			| TokenType::tilda >> [] { return new ast::BitwiseNegation(); }
			;

	return Result(block->parse(*mTokenStream, context), mErrors);
}

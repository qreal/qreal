/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "qrtext/src/lua/luaParser.h"

#include "qrtext/core/parser/parserRef.h"
#include "qrtext/core/parser/operators/parserCombinators.h"
#include "qrtext/core/parser/operators/expressionParser.h"

#include "qrtext/lua/ast/expression.h"

#include "qrtext/lua/ast/addition.h"
#include "qrtext/lua/ast/assignment.h"
#include "qrtext/lua/ast/bitwiseAnd.h"
#include "qrtext/lua/ast/bitwiseLeftShift.h"
#include "qrtext/lua/ast/bitwiseNegation.h"
#include "qrtext/lua/ast/bitwiseOr.h"
#include "qrtext/lua/ast/bitwiseRightShift.h"
#include "qrtext/lua/ast/bitwiseXor.h"
#include "qrtext/lua/ast/block.h"
#include "qrtext/lua/ast/concatenation.h"
#include "qrtext/lua/ast/division.h"
#include "qrtext/lua/ast/equality.h"
#include "qrtext/lua/ast/exponentiation.h"
#include "qrtext/lua/ast/false.h"
#include "qrtext/lua/ast/fieldInitialization.h"
#include "qrtext/lua/ast/floatNumber.h"
#include "qrtext/lua/ast/functionCall.h"
#include "qrtext/lua/ast/greaterOrEqual.h"
#include "qrtext/lua/ast/greaterThan.h"
#include "qrtext/lua/ast/identifier.h"
#include "qrtext/lua/ast/indexingExpression.h"
#include "qrtext/lua/ast/inequality.h"
#include "qrtext/lua/ast/integerDivision.h"
#include "qrtext/lua/ast/integerNumber.h"
#include "qrtext/lua/ast/length.h"
#include "qrtext/lua/ast/lessOrEqual.h"
#include "qrtext/lua/ast/lessThan.h"
#include "qrtext/lua/ast/logicalAnd.h"
#include "qrtext/lua/ast/logicalOr.h"
#include "qrtext/lua/ast/methodCall.h"
#include "qrtext/lua/ast/modulo.h"
#include "qrtext/lua/ast/multiplication.h"
#include "qrtext/lua/ast/nil.h"
#include "qrtext/lua/ast/not.h"
#include "qrtext/lua/ast/number.h"
#include "qrtext/lua/ast/string.h"
#include "qrtext/lua/ast/subtraction.h"
#include "qrtext/lua/ast/tableConstructor.h"
#include "qrtext/lua/ast/true.h"
#include "qrtext/lua/ast/unaryMinus.h"

using namespace qrtext::lua;
using namespace qrtext::lua::details;
using namespace qrtext::core;

LuaParser::LuaParser(QList<Error> &errors)
	: Parser<LuaTokenTypes>(grammar(), errors)
{
}

QSharedPointer<ParserInterface<LuaTokenTypes>> LuaParser::grammar()
{
	QSharedPointer<core::PrecedenceTable<LuaTokenTypes>> precedenceTable(new LuaPrecedenceTable());

	ParserRef<LuaTokenTypes> stat;
	ParserRef<LuaTokenTypes> explist;
	ParserRef<LuaTokenTypes> exp;
	ParserRef<LuaTokenTypes> primary;
	ParserRef<LuaTokenTypes> prefixexp;
	ParserRef<LuaTokenTypes> varpart;
	ParserRef<LuaTokenTypes> functioncallpart;
	ParserRef<LuaTokenTypes> prefixterm;
	ParserRef<LuaTokenTypes> args;
	ParserRef<LuaTokenTypes> tableconstructor;
	ParserRef<LuaTokenTypes> fieldlist;
	ParserRef<LuaTokenTypes> field;
	ParserRef<LuaTokenTypes> fieldsep;
	ParserRef<LuaTokenTypes> binop;
	ParserRef<LuaTokenTypes> unop;

	auto reportUnsupported = [this] (Token<LuaTokenTypes> const &token) {
		context().reportError(token, "This construction is not supported yet");
		return new TemporaryErrorNode();
	};

	// Additional production that parses unqualified identifier.
	auto identifier = LuaTokenTypes::identifier
			>> [] (Token<LuaTokenTypes> const &token) { return new ast::Identifier(token.lexeme()); }
			/= "identifier"
			;

	// block ::= {stat}
	auto block = *stat
			>> [] (QSharedPointer<TemporaryList> statList) {
				QList<QSharedPointer<ast::Node>> result;
				for (const auto &stat : statList->list()) {
					if (stat->is<TemporaryList>()) {
						// It is a list of assignments.
						for (const auto &assignment : as<TemporaryList>(stat)->list()) {
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
			}
			/= "block"
			;

	// stat ::= ‘;’ | explist [‘=’ explist]
	stat = (-LuaTokenTypes::semicolon | (explist & ~(-LuaTokenTypes::equals & explist)))
			>> [this] (QSharedPointer<ast::Node> node) {
				if (node->is<TemporaryDiscardableNode>()) {
					// It is semicolon, just discard it.
					return node;
				}

				if (node->is<TemporaryList>()) {
					// There is no optional part.
					auto expList = as<TemporaryList>(node);
					if (expList->list().size() != 1) {
						// It is a list of expressions which we translate as table constructor, to support convenient
						// lists syntax, for example, for lists of ports in robots.
						QList<QSharedPointer<ast::Node>> fields;
						for (const auto &exp : expList->list()) {
							fields << wrap(new ast::FieldInitialization(as<ast::Expression>(exp)));
						}

						return wrap(new ast::TableConstructor(as<ast::FieldInitialization>(fields)));
					} else {
						return expList->list().first();
					}
				}

				const auto pair = as<TemporaryPair>(node);
				if (!pair) {
					context().reportInternalError(QObject::tr("node in 'stat' semantic action is of unexpected type"));
					return wrap(new TemporaryErrorNode());
				}

				const auto variables = as<TemporaryList>(pair->left())->list();
				const auto values = as<TemporaryList>(pair->right())->list();
				if (variables.size() != values.size()) {
					context().reportError(pair, QObject::tr(
							"Number of variables in assignment shall be equal to the number of assigned values"));

					return wrap(new TemporaryErrorNode());
				}

				const auto result = QSharedPointer<TemporaryList>(new TemporaryList());

				for (int i = 0; i < variables.size(); ++i) {
					const auto variable = as<ast::Expression>(variables[i]);

					if (variable->is<ast::FunctionCall>()) {
						context().reportError(pair, QObject::tr("Assignment to function call is impossible"));
						continue;
					}

					const auto value = as<ast::Expression>(values[i]);
					result->list() << wrap(new ast::Assignment(variable, value));
				}

				return as<ast::Node>(result);
			}
			/= "stat"
			;

	// explist ::= exp(0) {‘,’ exp(0)}
	explist = (exp & *(-LuaTokenTypes::comma & exp))
			>> [] (QSharedPointer<TemporaryPair> node) {
				const auto firstExp = as<ast::Expression>(node->left());
				const auto temporaryList = as<TemporaryList>(node->right());
				temporaryList->list().prepend(firstExp);
				return temporaryList;
			}
			/= "explist"
			;

	// exp(precedence) ::= primary { binop exp(newPrecedence) }
	exp = ParserRef<LuaTokenTypes>(new ExpressionParser<LuaTokenTypes>(precedenceTable, primary, binop)) /= "exp";

	// primary ::= nil | false | true | Number | String | ‘...’ | prefixexp | tableconstructor | unop exp
	primary =
			LuaTokenTypes::nilKeyword >> [] { return new ast::Nil(); }
			| LuaTokenTypes::falseKeyword >> [] { return new ast::False(); }
			| LuaTokenTypes::trueKeyword >> [] { return new ast::True(); }
			| LuaTokenTypes::integerLiteral
					>> [] (Token<LuaTokenTypes> token) { return new ast::IntegerNumber(token.lexeme()); }
			| LuaTokenTypes::floatLiteral
					>> [] (Token<LuaTokenTypes> token) { return new ast::FloatNumber(token.lexeme()); }
			| LuaTokenTypes::string >> [] (Token<LuaTokenTypes> token) {
					QString string = token.lexeme();
					// Cut off quotes.
					string.remove(0, 1);
					string.chop(1);
					return new ast::String(string);

				}
			| LuaTokenTypes::tripleDot >> reportUnsupported
			| prefixexp
			| tableconstructor
			// Here we pass LuaTokenTypes::minus as starting precedence of exp production because all unary operators
			// in Lua have the same precedence, so we do not care what unop will yield. Otherwise we may include
			// exp in unop production, there we will know what unary operator is actually parsed before calling exp.
			| (unop & ParserRef<LuaTokenTypes>(new ExpressionParser<LuaTokenTypes>(
					precedenceTable, LuaTokenTypes::minus, primary, binop))
					)
					>> [] (QSharedPointer<TemporaryPair> node) {
						const auto unOp = as<ast::UnaryOperator>(node->left());
						unOp->setOperand(node->right());
						return unOp;
					}
			/= "primary"
			;

	// prefixexp ::= prefixterm { functioncallpart | varpart }
	prefixexp = (prefixterm & *(functioncallpart | varpart))
			>> [] (QSharedPointer<TemporaryPair> node) {
				auto result = as<ast::Expression>(node->left());
				for (const auto &part : as<TemporaryList>(node->right())->list()) {
					if (part->is<ast::Expression>()) {
						// It is varpart (indexing expression)
						result = QSharedPointer<ast::Expression>(
								new ast::IndexingExpression(result, as<ast::Expression>(part)));
					} else if (part->is<TemporaryPair>()) {
						// It is functioncallpart (method call)
						const auto methodName = as<ast::Identifier>(as<TemporaryPair>(part)->left());
						const auto args = as<ast::Expression>(
								as<TemporaryList>(as<TemporaryPair>(part)->right())->list());

						result = QSharedPointer<ast::Expression>(new ast::MethodCall(result, methodName, args));
					} else if (part->is<TemporaryList>()) {
						// It is functioncallpart (function call)
						result = QSharedPointer<ast::Expression>(
								new ast::FunctionCall(result, as<ast::Expression>(as<TemporaryList>(part)->list())));
					}
				}

				return result;
			}
			/= "prefixexp"
			;

	// varpart ::= ‘[’ exp(0) ‘]’ | ‘.’ Name
	varpart = (-LuaTokenTypes::openingSquareBracket & exp & -LuaTokenTypes::closingSquareBracket)
			| (-LuaTokenTypes::dot & LuaTokenTypes::identifier >> [] (Token<LuaTokenTypes> const &token) {
						return new ast::String(token.lexeme());
					}
			)
			/= "varpart"
			;

	// functioncallpart :: = args | ‘:’ Name args
	functioncallpart = args | (-LuaTokenTypes::colon & identifier & args) /= "functioncallpart";

	// prefixterm ::= Name | ‘(’ exp(0) ‘)’
	prefixterm = identifier
			| (-LuaTokenTypes::openingBracket & exp & -LuaTokenTypes::closingBracket)
			/= "prefixterm"
			;

	// args ::= ‘(’ [explist] ‘)’ | tableconstructor | String
	args = ((-LuaTokenTypes::openingBracket & ~explist & -LuaTokenTypes::closingBracket)
			| tableconstructor
			| LuaTokenTypes::string >> [] (Token<LuaTokenTypes> token) { return new ast::String(token.lexeme()); }
			) >> [this] (QSharedPointer<ast::Node> node) {
					if (node->is<TemporaryList>()) {
						return node;
					} else {
						const auto result = QSharedPointer<TemporaryList>(new TemporaryList());
						if (node->is<ast::Expression>()) {
							result->list() << as<ast::Expression>(node);
							return as<ast::Node>(result);
						} else if (node->is<TemporaryDiscardableNode>()) {
							return as<ast::Node>(result);
						} else {
							context().reportInternalError(QObject::tr("In 'args' semantic action node is "
									"of incorrect type"));
							return wrap(new TemporaryErrorNode());
						}
					}
				}
			/= "args"
			;

	// tableconstructor ::= ‘{’ [fieldlist] ‘}’
	tableconstructor = (-LuaTokenTypes::openingCurlyBracket & ~fieldlist & -LuaTokenTypes::closingCurlyBracket)
			>> [this] (QSharedPointer<ast::Node> fieldList) {
				if (fieldList->is<TemporaryDiscardableNode>()) {
					return wrap(new ast::TableConstructor({}));
				} else {
					const auto list = as<TemporaryList>(fieldList);
					if (!list) {
						context().reportInternalError(QObject::tr("In 'table constructor' semantic action fieldList is "
								"of incorrect type"));
						return wrap(new TemporaryErrorNode());
					}

					return wrap(new ast::TableConstructor(as<ast::FieldInitialization>(list->list())));
				}
			}
			/= "tableconstructor"
			;

	// fieldlist ::= field {fieldsep field} [fieldsep]
	fieldlist = (field & *(-fieldsep & field) & -~fieldsep)
			 >> [] (QSharedPointer<TemporaryPair> node) {
				auto firstField = as<ast::FieldInitialization>(node->left());
				auto temporaryList = as<TemporaryList>(node->right());
				temporaryList->list().prepend(firstField);
				return temporaryList;
			}
			/= "fieldlist"
			;

	// field ::= ‘[’ exp(0) ‘]’ ‘=’ exp(0) | exp(0) [ ‘=’ exp(0) ]
	field = (-LuaTokenTypes::openingSquareBracket
			& exp
			& -LuaTokenTypes::closingSquareBracket
			& -LuaTokenTypes::equals & exp)
					>> [] (QSharedPointer<TemporaryPair> pair) {
						auto initializer = as<ast::Expression>(pair->right());
						auto indexer = as<ast::Expression>(pair->left());
						return wrap(new ast::FieldInitialization(indexer, initializer));
					}
			| (exp & ~(-LuaTokenTypes::equals & exp))
					>> [this] (QSharedPointer<ast::Node> node) {
							if (node->is<TemporaryPair>()) {
								const auto pair = as<TemporaryPair>(node);
								const auto left = as<ast::Expression>(pair->left());
								const auto initializer = as<ast::Expression>(pair->right());
								/// @todo Report error if "left" is something different from Name.
								return wrap(new ast::FieldInitialization(left, initializer));
							} else if (node->is<ast::Expression>()) {
								return wrap(new ast::FieldInitialization(as<ast::Expression>(node)));
							} else {
								context().reportInternalError(QObject::tr("In 'field' semantic action node is "
										"of incorrect type"));
								return wrap(new TemporaryErrorNode());
							}
						}
			/= "field"
			;

	// fieldsep ::= ‘,’ | ‘;’
	fieldsep = -LuaTokenTypes::comma
			| -LuaTokenTypes::semicolon
			/= "fieldsep"
			;

	// binop ::= ‘+’ | ‘-’ | ‘*’ | ‘/’ | ‘//’ | ‘^’ | ‘%’ | ‘&’ | ‘~’ | ‘|’ | ‘>>’ | ‘<<’ | ‘..’
	//           | ‘<’ | ‘<=’ | ‘>’ | ‘>=’ | ‘==’ | ‘~=’ | ‘!=’ | and | or
	binop = LuaTokenTypes::plus >> [] { return new ast::Addition(); }
			| LuaTokenTypes::minus >> [] { return new ast::Subtraction(); }
			| LuaTokenTypes::asterick >> [] { return new ast::Multiplication(); }
			| LuaTokenTypes::slash >> [] { return new ast::Division(); }
			| LuaTokenTypes::doubleSlash >> [] { return new ast::IntegerDivision(); }
			| LuaTokenTypes::hat >> [] { return new ast::Exponentiation(); }
			| LuaTokenTypes::percent >> [] { return new ast::Modulo(); }
			| LuaTokenTypes::ampersand >> [] { return new ast::BitwiseAnd(); }
			| LuaTokenTypes::tilda >> [] { return new ast::BitwiseXor(); }
			| LuaTokenTypes::verticalLine >> [] { return new ast::BitwiseOr(); }
			| LuaTokenTypes::doubleGreater >> [] { return new ast::BitwiseRightShift(); }
			| LuaTokenTypes::doubleLess >> [] { return new ast::BitwiseLeftShift(); }
			| LuaTokenTypes::doubleDot >> [] { return new ast::Concatenation(); }
			| LuaTokenTypes::less >> [] { return new ast::LessThan(); }
			| LuaTokenTypes::lessEquals >> [] { return new ast::LessOrEqual(); }
			| LuaTokenTypes::greater >> [] { return new ast::GreaterThan(); }
			| LuaTokenTypes::greaterEquals >> [] { return new ast::GreaterOrEqual(); }
			| LuaTokenTypes::doubleEquals >> [] { return new ast::Equality(); }
			| LuaTokenTypes::tildaEquals >> [] { return new ast::Inequality(); }
			| LuaTokenTypes::exclamationMarkEquals >> [] { return new ast::Inequality(); }
			| LuaTokenTypes::andKeyword >> [] { return new ast::LogicalAnd(); }
			| LuaTokenTypes::orKeyword >> [] { return new ast::LogicalOr(); }
			| LuaTokenTypes::doubleAmpersand >> [] { return new ast::LogicalAnd(); }
			| LuaTokenTypes::doubleVerticalLine >> [] { return new ast::LogicalOr(); }
			/= "binop"
			;

	// unop ::= ‘-’ | not | ‘#’ | ‘~’
	unop = LuaTokenTypes::minus >> [] { return new ast::UnaryMinus(); }
			| LuaTokenTypes::notKeyword >> [] { return new ast::Not(); }
			| LuaTokenTypes::sharp >> [] { return new ast::Length(); }
			| LuaTokenTypes::tilda >> [] { return new ast::BitwiseNegation(); }
			/= "unop"
			;

	return block.parser();
}

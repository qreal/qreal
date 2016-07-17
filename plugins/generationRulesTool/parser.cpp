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

#include "parser.h"

#include "ast/callGeneratorFor.h"
#include "ast/complexIdentifier.h"
#include "ast/elementIdentifier.h"
#include "ast/generator.h"
#include "ast/foreach.h"
#include "ast/identifier.h"
#include "ast/incomingLinks.h"
#include "ast/links.h"
#include "ast/newline.h"
#include "ast/node.h"
#include "ast/outcomingLinks.h"
#include "ast/program.h"
#include "ast/tab.h"
#include "ast/text.h"
#include "ast/this.h"
#include "ast/transitionEnd.h"
#include "ast/transitionStart.h"
#include "ast/list.h"
#include "ast/generateToFile.h"
#include "ast/string.h"
#include "ast/equal.h"
#include "ast/notEqual.h"
#include "ast/condition.h"
#include "ast/ifNode.h"

using namespace simpleParser;

using namespace qrtext::core;

simpleParser::Parser::Parser(QList<qrtext::core::Error> &errors)
	: qrtext::core::Parser<TokenTypes>(grammar(), errors)
{
}

QSharedPointer<qrtext::core::ParserInterface<TokenTypes>> simpleParser::Parser::grammar()
{
	ParserRef<TokenTypes> statement;

	auto program = *statement
			>> [] (QSharedPointer<TemporaryList> statementList) {
				QList<QSharedPointer<ast::Node>> result;
				for (auto const &stat : statementList->list()) {
					if (stat->is<TemporaryList>()) {
						for (auto const &assignment : qrtext::as<TemporaryList>(stat)->list()) {
							result << assignment;
						}
					} else {
						// It is expression.
						result << stat;
					}
				}

				return qrtext::wrap(new ast::Program(result));
	};

	auto identifier = TokenTypes::identifier
			>> [] (Token<TokenTypes> const &token) {
				return new ast::Identifier(token.lexeme());
	};

	auto transitionStartIdentifier = TokenTypes::transitionStartKeyword
			>> [] (Token<TokenTypes> const &token) {
				Q_UNUSED(token);
				return new ast::TransitionStart();
	};

	auto transitionEndIdentifier = TokenTypes::transitionEndKeyword
			>> [] (Token<TokenTypes> const &token) {
				Q_UNUSED(token);
				return new ast::TransitionEnd();
	};

	auto thisIdentifier = TokenTypes::thisKeyword
			>> [] (Token<TokenTypes> const &token) {
				Q_UNUSED(token);
				return new ast::This();
	};

	auto elementIdentifier = ((identifier | thisIdentifier) &
				~(-TokenTypes::dot & (transitionEndIdentifier | transitionStartIdentifier)))
			>> [] (QSharedPointer<ast::Node> elementIdentifierNode) {
				if (elementIdentifierNode->is<TemporaryPair>()) {
					auto asPair = qrtext::as<TemporaryPair>(elementIdentifierNode);

					auto identifierPart = asPair->left();
					auto linkPart = asPair->right();

					return qrtext::wrap(new ast::ElementIdentifier(identifierPart, linkPart));
				} else {
					return qrtext::wrap(new ast::ElementIdentifier(elementIdentifierNode));
				}
	};

	auto complexIdentifier = (elementIdentifier & -TokenTypes::arrow & identifier)
			>> [] (QSharedPointer<TemporaryPair> identifierAndProperty) {
				auto identifierPart = identifierAndProperty->left();
				auto propertyPart = identifierAndProperty->right();

				return qrtext::wrap(new ast::ComplexIdentifier(identifierPart, propertyPart));
	};

	auto outcomingLinksIdentifier = (-TokenTypes::outcomingLinksKeyword & -TokenTypes::openingBracket
				& identifier & -TokenTypes::closingBracket)
			>> [] (QSharedPointer<ast::Node> linkTypeNode) {
				return qrtext::wrap(new ast::OutcomingLinks(linkTypeNode));
	};

	auto incomingLinksIdentifier = (-TokenTypes::incomingLinksKeyword& -TokenTypes::openingBracket
				& identifier & -TokenTypes::closingBracket)
			>> [] (QSharedPointer<ast::Node> linkTypeNode) {
				return qrtext::wrap(new ast::IncomingLinks(linkTypeNode));
	};

	auto linksIdentifier = (TokenTypes::linksKeyword & -TokenTypes::openingBracket
							& identifier & -TokenTypes::closingBracket)
			>> [] (QSharedPointer<ast::Node> linkTypeNode) {
				return qrtext::wrap(new ast::Links(linkTypeNode));
	};

	auto listIdentifier = (elementIdentifier & ~(-TokenTypes::doubleColon
				& (outcomingLinksIdentifier | incomingLinksIdentifier | linksIdentifier)))
			>> [] (QSharedPointer<ast::Node> identifierAndOptionalLink) {
				if (identifierAndOptionalLink->is<TemporaryPair>()) {
					auto asPair = qrtext::as<TemporaryPair>(identifierAndOptionalLink);
					auto identifierPart = asPair->left();
					auto linkPart = asPair->right();

					return qrtext::wrap(new ast::List(identifierPart, linkPart));
				} else {
					return qrtext::wrap(new ast::List(identifierAndOptionalLink));
				}
	};

	auto text = TokenTypes::text
			>> [] (Token<TokenTypes> const &token) {
				return new ast::Text(token.lexeme());
	};

	auto foreachExcludeStatement = (-TokenTypes::foreachExcludeKeyword
				& -TokenTypes::openingBracket & identifier
				& -TokenTypes::inKeyword & listIdentifier
				& -TokenTypes::comma & -TokenTypes::excludeKeyword & text
				& -TokenTypes::closingBracket
				& -TokenTypes::openingCurlyBracket & program & -TokenTypes::closingCurlyBracket)
			>> [] (QSharedPointer<TemporaryPair> statementPair) {
				const auto identifierListAndText = qrtext::as<TemporaryPair>(statementPair->left());

				const auto identifierAndList = qrtext::as<TemporaryPair>(identifierListAndText->left());

				const auto identifierPart = identifierAndList->left();
				const auto listPart = identifierAndList->right();

				const auto excludedSymbol = identifierListAndText->right();

				const auto program = statementPair->right();

				return qrtext::wrap(new ast::Foreach(identifierPart, listPart, program, excludedSymbol));
	};

	auto foreachStatement = (-TokenTypes::foreachKeyword
				& -TokenTypes::openingBracket & identifier
				& -TokenTypes::inKeyword & listIdentifier
				& -TokenTypes::closingBracket
				& -TokenTypes::openingCurlyBracket & program & -TokenTypes::closingCurlyBracket)
			>> [] (QSharedPointer<TemporaryPair> statementPair) {
				auto identifierAndList = qrtext::as<TemporaryPair>(statementPair->left());

				auto identifierPart = identifierAndList->left();
				auto listPart = identifierAndList->right();

				auto program = statementPair->right();

				return qrtext::wrap(new ast::Foreach(identifierPart, listPart, program));
	};

	auto callGeneratorForStatement = (-TokenTypes::callGeneratorForKeyword & -TokenTypes::openingBracket
				& elementIdentifier
				& ~(-TokenTypes::comma & identifier)
				& -TokenTypes::closingBracket)
			>> [] (QSharedPointer<ast::Node> temporaryNode) {
				if (temporaryNode->is<TemporaryPair>()) {
					auto pair = qrtext::as<TemporaryPair>(temporaryNode);

					return qrtext::wrap(new ast::CallGeneratorFor(pair->left(), pair->right()));
				} else {
					return qrtext::wrap(new ast::CallGeneratorFor(temporaryNode));
				}
	};

	auto generatorStatement = (-TokenTypes::generatorKeyword & identifier & -TokenTypes::openingCurlyBracket
				& program & -TokenTypes::closingCurlyBracket)
			>> [] (QSharedPointer<TemporaryPair> nameAndProgram) {
				auto generatorName = nameAndProgram->left();
				auto program = nameAndProgram->right();

				return qrtext::wrap(new ast::Generator(generatorName, program));
	};

	auto string = (complexIdentifier & -TokenTypes::plus & text)
			>> [] (QSharedPointer<TemporaryPair> temporaryPair) {
				auto identifierPart = temporaryPair->left();
				auto textPart = temporaryPair->right();

				return qrtext::wrap(new ast::String(identifierPart, textPart));
	};

	auto generateToFileStatement = (-TokenTypes::generateToFileKeyword & -TokenTypes::openingBracket
				& (string | text) & -TokenTypes::closingBracket
				& -TokenTypes::openingCurlyBracket & program & -TokenTypes::closingCurlyBracket)
			>> [] (QSharedPointer<TemporaryPair> temporaryNode) {
				auto fileNamePart = temporaryNode->left();
				auto programPart = temporaryNode->right();

				return qrtext::wrap(new ast::GenerateToFile(fileNamePart, programPart));
	};

	auto comparator = TokenTypes::equal >> [] { return new ast::Equal(); }
				| TokenTypes::notEqual >> [] { return new ast::NotEqual(); };

	auto condition = (complexIdentifier
				& ~(comparator & text))
			>> [] (QSharedPointer<ast::Node> temporaryNode) {
				if (temporaryNode->is<TemporaryPair>()) {
					auto asPair = qrtext::as<TemporaryPair>(temporaryNode);

					auto firstPart = asPair->left();

					auto temporarySecondPart = qrtext::as<TemporaryPair>(asPair->right());
					auto comparatorNode = temporarySecondPart->left();
					auto secondPart = temporarySecondPart->right();

					return qrtext::wrap(new ast::Condition(firstPart, comparatorNode, secondPart));
				} else {
					return qrtext::wrap(new ast::Condition(temporaryNode));
				}
	};

	auto ifExpression = (-TokenTypes::ifKeyword & -TokenTypes::openingBracket & condition & -TokenTypes::closingBracket
				& -TokenTypes::openingCurlyBracket & program & -TokenTypes::closingCurlyBracket
				& ~(-TokenTypes::elseKeyword & -TokenTypes::openingCurlyBracket & program
				& -TokenTypes::closingCurlyBracket))
			>> [] (QSharedPointer<TemporaryPair> temporaryPair) {
					auto leftPart = temporaryPair->left();
					auto rightPart = temporaryPair->right();

					if (leftPart->is<TemporaryPair>()) {
						auto asPair = qrtext::as<TemporaryPair>(leftPart);
						auto conditionNode = asPair->left();
						auto trueBranchNode = asPair->right();

						return qrtext::wrap(new ast::IfNode(conditionNode, trueBranchNode, rightPart));
					} else {
						return qrtext::wrap(new ast::IfNode(leftPart, rightPart));
					}
	};

	auto newline = TokenTypes::newlineKeyword
			>> [] (Token<TokenTypes> const &token) {
				Q_UNUSED(token);
				return new ast::Newline();
	};

	auto tab = TokenTypes::tabKeyword
			>> [] (Token<TokenTypes> const &token) {
				Q_UNUSED(token);
				return new ast::Tab();
	};

	statement = text | tab | newline | complexIdentifier | foreachStatement | foreachExcludeStatement
			| callGeneratorForStatement | generateToFileStatement | generatorStatement | ifExpression;

	return program.parser();
}

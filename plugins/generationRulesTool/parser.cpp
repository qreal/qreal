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
#include "ast/partOfComplexIdentifier.h"
#include "ast/program.h"
#include "ast/tab.h"
#include "ast/text.h"
#include "ast/transitionEnd.h"
#include "ast/transitionStart.h"
#include "ast/list.h"

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

				if (result.size() == 1) {
					// Do not create Program node for simple expression.
					return result.first();
				} else {
					return qrtext::wrap(new ast::Program(result));
				}
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

	auto elementIdentifier = (identifier &
				~(-TokenTypes::dot & (identifier | transitionEndIdentifier | transitionStartIdentifier)))
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

	auto complexIdentifier = (elementIdentifier & -TokenTypes::dot & identifier)
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

	auto incomingLinksIdentifier = (TokenTypes::incomingLinksKeyword& -TokenTypes::openingBracket
				& identifier & -TokenTypes::closingBracket)
			>> [] (QSharedPointer<ast::Node> linkTypeNode) {
				return qrtext::wrap(new ast::IncomingLinks(linkTypeNode));
	};

	auto linksIdentifier = (TokenTypes::linksKeyword & -TokenTypes::openingBracket
							& identifier & -TokenTypes::closingBracket)
			>> [] (QSharedPointer<ast::Node> linkTypeNode) {
				return qrtext::wrap(new ast::Links(linkTypeNode));
	};

	auto listIdentifier = (elementIdentifier & ~(-TokenTypes::dot
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

	auto foreachStatement = (-TokenTypes::foreachKeyword & -TokenTypes::openingBracket & identifier
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

	auto text = TokenTypes::text
			>> [] (Token<TokenTypes> const &token) {
				return new ast::Text(token.lexeme());
	};

	auto newline = TokenTypes::newlineKeyword
			>> [] (Token<TokenTypes> const &token) {
				Q_UNUSED(token);
				return new ast::Newline();
	};

	auto tab = TokenTypes::tabKeyword
			>> [] (Token<TokenTypes> const &token) {
				return new ast::Tab();
	};

	statement = text | tab | newline | complexIdentifier | foreachStatement | callGeneratorForStatement | generatorStatement;

	return program.parser();
}

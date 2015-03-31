#include "parser.h"

#include "ast/callGeneratorFor.h"
#include "ast/complexIdentifier.h"
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

	auto transitionEndIdentifier = (-TokenTypes::transitionEndKeyword & -TokenTypes::dot & identifier)
			>> [] (QSharedPointer<ast::Node> partOfComplexIdentifier) {
				return qrtext::wrap(new ast::TransitionEnd(partOfComplexIdentifier));
	};

	auto complexIdentifier = (identifier & -TokenTypes::dot & (transitionEndIdentifier | identifier))
			>> [] (QSharedPointer<TemporaryPair> tokens) {
				auto firstIdentifier = tokens->left();
				auto secondIdentifier = tokens->right();

				return qrtext::wrap(new ast::ComplexIdentifier(firstIdentifier, secondIdentifier));
	};

	auto outcomingLinksIdentifier = (-TokenTypes::outcomingLinksKeyword & -TokenTypes::openingBracket
				& identifier & -TokenTypes::closingBracket)
			>> [] (QSharedPointer<ast::Node> linkTypeNode) {
				return qrtext::wrap(new ast::OutcomingLinks(linkTypeNode));
	};

	auto incomingLinksIdentifier = (TokenTypes::incomingLinksKeyword)
			>> [] (Token<TokenTypes> const &token) {
				return new ast::IncomingLinks();
	};

	auto linksIdentifier = (TokenTypes::linksKeyword)
			>> [] (Token<TokenTypes> const &token) {
				return new ast::Links();
	};

	auto foreachStatement = (-TokenTypes::foreachKeyword & -TokenTypes::openingBracket & identifier
				& -TokenTypes::inKeyword & identifier
				& ~(-TokenTypes::dot & outcomingLinksIdentifier)
				& -TokenTypes::closingBracket
				& -TokenTypes::openingCurlyBracket & program & -TokenTypes::closingCurlyBracket)
			>> [] (QSharedPointer<TemporaryPair> statementPair) {
				auto identifierTypeAndLinks = qrtext::as<TemporaryPair>(statementPair->left());
				auto temporaryIdentifier = identifierTypeAndLinks->left();
				auto temporaryType = identifierTypeAndLinks->right();

				auto program = statementPair->right();

				if (temporaryIdentifier->is<TemporaryPair>()) {
					auto identifierAndType = qrtext::as<TemporaryPair>(temporaryIdentifier);

					auto type = identifierAndType->right();
					auto identifier = identifierAndType->left();

					auto links = temporaryType;

					return qrtext::wrap(new ast::Foreach(identifier, type, program, links));
				}

				return qrtext::wrap(new ast::Foreach(temporaryIdentifier, temporaryType, program));
	};

	auto callGeneratorForStatement = (-TokenTypes::callGeneratorForKeyword & -TokenTypes::openingBracket
				& identifier & -TokenTypes::closingBracket)
			>> [] (QSharedPointer<ast::Node> identifierNode) {
				return qrtext::wrap(new ast::CallGeneratorFor(identifierNode));
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

	statement = text | tab | newline | complexIdentifier | foreachStatement | callGeneratorForStatement;

	return program.parser();
}

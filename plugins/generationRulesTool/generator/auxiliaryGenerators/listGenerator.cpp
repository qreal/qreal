#include "listGenerator.h"

#include "ast/identifier.h"
#include "ast/incomingLinks.h"
#include "ast/outcomingLinks.h"

#include "incomingLinksListGenerator.h"
#include "outcomingLinksListGenerator.h"
#include "simpleTypeListGenerator.h"

using namespace generationRules::generator;

qReal::IdList ListGenerator::listOfIds(
		QSharedPointer<simpleParser::ast::List> listNode
		, qReal::LogicalModelAssistInterface *logicalModelInterface
		, const qReal::Id elementId)
{
	auto mainPartOfListIdentifier = qrtext::as<simpleParser::ast::Identifier>(listNode->identifier());
	auto optionalLinkPart = listNode->optionalLinkPart();

	if (!optionalLinkPart) {
		return SimpleTypeListGenerator::generatedList(mainPartOfListIdentifier, logicalModelInterface, elementId);
	} else {
		if (optionalLinkPart->is<simpleParser::ast::IncomingLinks>()) {
			return IncomingLinksListGenerator::generatedList(mainPartOfListIdentifier, logicalModelInterface, elementId);
		} else {
			return OutcomingLinksListGenerator::generatedList(mainPartOfListIdentifier, logicalModelInterface, elementId);
		}
	}
}

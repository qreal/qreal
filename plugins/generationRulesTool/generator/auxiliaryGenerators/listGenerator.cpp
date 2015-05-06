#include "listGenerator.h"

#include "ast/identifier.h"
#include "ast/incomingLinks.h"
#include "ast/outcomingLinks.h"
#include "ast/elementIdentifier.h"

#include "incomingLinksListGenerator.h"
#include "outcomingLinksListGenerator.h"
#include "simpleTypeListGenerator.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

qReal::IdList ListGenerator::listOfIds(
		QSharedPointer<List> listNode
		, qReal::LogicalModelAssistInterface *logicalModelInterface
		, const qReal::Id elementId)
{
	auto mainPartOfListIdentifier = qrtext::as<ElementIdentifier>(listNode->identifier());
	auto identifierPart = qrtext::as<Identifier>(mainPartOfListIdentifier->identifierPart());
	auto optionalLinkPart = listNode->optionalLinkPart();

	if (!optionalLinkPart) {
		// listNode is element node => listNode is identifier node (without transitionEnd or TransitionStart)
		return SimpleTypeListGenerator::generatedList(identifierPart, logicalModelInterface, elementId);
	} else {
		if (optionalLinkPart->is<IncomingLinks>()) {
			auto linkType = qrtext::as<IncomingLinks>(optionalLinkPart)->linkType();
			auto asIdentifier = qrtext::as<Identifier>(linkType);
			return IncomingLinksListGenerator::generatedList(identifierPart, asIdentifier, logicalModelInterface, elementId);
		} else {
			return OutcomingLinksListGenerator::generatedList(identifierPart, logicalModelInterface, elementId);
		}
	}
}

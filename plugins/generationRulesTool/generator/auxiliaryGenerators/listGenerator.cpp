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
	auto firstPart = qrtext::as<Identifier>(mainPartOfListIdentifier->identifierPart());
	auto optionalLinkPart = listNode->optionalLinkPart();

	if (!optionalLinkPart) {
		return SimpleTypeListGenerator::generatedList(firstPart, logicalModelInterface, elementId);
	} else {
		if (optionalLinkPart->is<IncomingLinks>()) {
			return IncomingLinksListGenerator::generatedList(firstPart, logicalModelInterface, elementId);
		} else {
			return OutcomingLinksListGenerator::generatedList(firstPart, logicalModelInterface, elementId);
		}
	}
}

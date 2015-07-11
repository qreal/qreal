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

qReal::IdList ListGenerator::listOfIds(const QSharedPointer<List> &listNode
		, qReal::LogicalModelAssistInterface *logicalModelInterface
		, VariablesTable &variablesTable
		, const CurrentScope &currentScope)
{
	const auto mainPartOfListIdentifier = qrtext::as<ElementIdentifier>(listNode->identifier());
	// first part can be identifier or "this"
	const auto firstPart = mainPartOfListIdentifier->identifierPart();
	const auto optionalLinkPart = listNode->optionalLinkPart();

	if (!optionalLinkPart) {
		// listNode is element node => listNode is identifier node (without transitionEnd or TransitionStart)
		const auto identifierPart = qrtext::as<Identifier>(firstPart);
		return SimpleTypeListGenerator::generatedList(identifierPart, logicalModelInterface);
	} else {
		if (optionalLinkPart->is<IncomingLinks>()) {
			const auto linkType = qrtext::as<Identifier>(qrtext::as<IncomingLinks>(optionalLinkPart)->linkType());

			return IncomingLinksListGenerator::generatedList(firstPart, linkType, logicalModelInterface, variablesTable, currentScope);
		} else {
			const auto linkType = qrtext::as<Identifier>(qrtext::as<OutcomingLinks>(optionalLinkPart)->linkType());

			return OutcomingLinksListGenerator::generatedList(firstPart, linkType, logicalModelInterface, variablesTable, currentScope);
		}
	}
}

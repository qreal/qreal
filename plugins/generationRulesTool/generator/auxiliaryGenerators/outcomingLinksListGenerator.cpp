#include "outcomingLinksListGenerator.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

qReal::IdList OutcomingLinksListGenerator::generatedList(QSharedPointer<Node> linksIdentifierNode
		, QSharedPointer<Identifier> linksTypeNode
		, qReal::LogicalModelAssistInterface *logicalModelInterface
		, VariablesTable variablesTable
		, CurrentScope currentScope)
{
	qReal::Id elementId;
	if (linksIdentifierNode->is<Identifier>()) {
		auto elementName = qrtext::as<Identifier>(linksIdentifierNode)->name();
		elementId = variablesTable.currentId(elementName);
	} else {
		elementId = currentScope.currentId();
	}

	auto linksType = linksTypeNode->name();

	auto allOutcomingLinks = logicalModelInterface->logicalRepoApi().outgoingLinks(elementId);
	qReal::IdList linksList;

	for (const auto currentLink : allOutcomingLinks) {
		if (currentLink.element() == linksType) {
			linksList << currentLink;
		}
	}

	return linksList;
}

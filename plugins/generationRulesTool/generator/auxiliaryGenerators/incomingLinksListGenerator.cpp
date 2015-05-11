#include "incomingLinksListGenerator.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

qReal::IdList IncomingLinksListGenerator::generatedList(
		QSharedPointer<Node> linksIdentifierNode
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

	auto allIncomingLinks = logicalModelInterface->logicalRepoApi().incomingLinks(elementId);
	qReal::IdList linksList;

	for (const auto currentLink : allIncomingLinks) {
		if (currentLink.element() == linksType) {
			linksList << currentLink;
		}
	}

	return linksList;
}

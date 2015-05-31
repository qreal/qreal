#include "linksListGenerator.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

qReal::IdList LinksListGenerator::generatedList(
		QSharedPointer<Node> linksIdentifierNode
		, QSharedPointer<Identifier> linksTypeNode
		, qReal::LogicalModelAssistInterface *logicalModelInterface
		, VariablesTable variablesTable
		, CurrentScope currentScope)
{
	const auto neededElementId = elementId(linksIdentifierNode, variablesTable, currentScope);
	const auto allLinks = logicalModelInterface->logicalRepoApi().links(neededElementId);
	return linksOfNeededType(linksTypeNode, allLinks);
}

qReal::Id LinksListGenerator::elementId(QSharedPointer<Node> linksIdentifierNode
		, VariablesTable variablesTable
		, CurrentScope currentScope)
{
	qReal::Id elementId;
	if (linksIdentifierNode->is<Identifier>()) {
		const auto elementName = qrtext::as<Identifier>(linksIdentifierNode)->name();
		elementId = variablesTable.currentId(elementName);
	} else {
		elementId = currentScope.currentId();
	}

	return elementId;
}

qReal::IdList LinksListGenerator::linksOfNeededType(QSharedPointer<Identifier> linksTypeNode
		, qReal::IdList allLinks)
{
	qReal::IdList linksList;

	const auto linksType = linksTypeNode->name();

	for (const auto &currentLink : allLinks) {
		if (currentLink.element() == linksType) {
			linksList << currentLink;
		}
	}

	return linksList;
}

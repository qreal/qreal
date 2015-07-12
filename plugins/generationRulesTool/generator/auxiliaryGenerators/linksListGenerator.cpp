#include "linksListGenerator.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

qReal::IdList LinksListGenerator::generatedList(
		const QSharedPointer<Node> &linksIdentifierNode
		, const QSharedPointer<Identifier> &linksTypeNode
		, qReal::LogicalModelAssistInterface *logicalModelInterface
		, VariablesTable &variablesTable
		, const CurrentScope &currentScope)
{
	const auto neededElementId = elementId(linksIdentifierNode, variablesTable, currentScope);
	const auto allLinks = logicalModelInterface->logicalRepoApi().links(neededElementId);
	return linksOfNeededType(linksTypeNode, allLinks);
}

qReal::Id LinksListGenerator::elementId(const QSharedPointer<Node> &linksIdentifierNode
		, VariablesTable &variablesTable
		, const CurrentScope &currentScope)
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

qReal::IdList LinksListGenerator::linksOfNeededType(const QSharedPointer<Identifier> &linksTypeNode
		, const qReal::IdList &allLinks)
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

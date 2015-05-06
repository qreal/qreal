#include "outcomingLinksListGenerator.h"

using namespace generationRules::generator;

qReal::IdList OutcomingLinksListGenerator::generatedList(
		QSharedPointer<simpleParser::ast::Identifier> linksIdentifierNode
		, QSharedPointer<simpleParser::ast::Identifier> linksTypeNode
		, qReal::LogicalModelAssistInterface *logicalModelInterface
		, VariablesTable variablesTable)
{
	auto elementName = linksIdentifierNode->name();
	auto elementId = variablesTable.currentId(elementName);

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

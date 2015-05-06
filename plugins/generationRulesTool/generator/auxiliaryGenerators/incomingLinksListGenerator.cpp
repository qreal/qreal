#include "incomingLinksListGenerator.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

qReal::IdList IncomingLinksListGenerator::generatedList(
		QSharedPointer<Identifier> linksIdentifierNode
		, QSharedPointer<Identifier> linksTypeNode
		, qReal::LogicalModelAssistInterface *logicalModelInterface
		, VariablesTable variablesTable)
{
	auto elementName = linksIdentifierNode->name();
	auto elementId = variablesTable.currentId(elementName);

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

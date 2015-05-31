#include "outcomingLinksListGenerator.h"
#include "linksListGenerator.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

qReal::IdList OutcomingLinksListGenerator::generatedList(QSharedPointer<Node> linksIdentifierNode
		, QSharedPointer<Identifier> linksTypeNode
		, qReal::LogicalModelAssistInterface *logicalModelInterface
		, VariablesTable variablesTable
		, CurrentScope currentScope)
{
	const auto neededElementId = LinksListGenerator::elementId(linksIdentifierNode, variablesTable, currentScope);
	const auto allIncomingLinks = logicalModelInterface->logicalRepoApi().outgoingLinks(neededElementId);
	return LinksListGenerator::linksOfNeededType(linksTypeNode, allIncomingLinks);
}

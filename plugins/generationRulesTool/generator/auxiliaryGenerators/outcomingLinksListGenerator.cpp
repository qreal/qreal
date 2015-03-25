#include "outcomingLinksListGenerator.h"

using namespace generationRules::generator;

qReal::IdList OutcomingLinksListGenerator::generatedList(
		QSharedPointer<simpleParser::ast::Identifier> linksIdentifierNode
		, qReal::LogicalModelAssistInterface *logicalModelInterface
		, const qReal::Id elementId)
{
	return logicalModelInterface->logicalRepoApi().outgoingLinks(elementId);
}

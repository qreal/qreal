#include "incomingLinksListGenerator.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

qReal::IdList IncomingLinksListGenerator::generatedList(
		QSharedPointer<Identifier> linksIdentifierNode
		, qReal::LogicalModelAssistInterface *logicalModelInterface
		, const qReal::Id elementId)
{
	return logicalModelInterface->logicalRepoApi().incomingLinks(elementId);
}

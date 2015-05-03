#include "simpleTypeListGenerator.h"

using namespace generationRules::generator;
using namespace simpleParser;

qReal::IdList SimpleTypeListGenerator::generatedList(
		QSharedPointer<ast::Identifier> simpleIdentifierNode
		, qReal::LogicalModelAssistInterface *logicalModelInterface
		, const qReal::Id elementId)
{
	Q_UNUSED(elementId);

	qReal::IdList listOfElements;
	for (const qReal::Id elementId : logicalModelInterface->children(qReal::Id::rootId())) {
		if (elementId.element() == simpleIdentifierNode->name()) {
			listOfElements << elementId;
		}
	}

	return listOfElements;
}

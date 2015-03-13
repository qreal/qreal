#include "simpleTypeListGenerator.h"

using namespace generationRules::generator;
using namespace simpleParser;

qReal::IdList SimpleTypeListGenerator::generatedList(
		QSharedPointer<ast::Identifier> linksIdentifierNode
		, qReal::LogicalModelAssistInterface *logicalModelInterface
		, const qReal::Id elementId)
{
	Q_UNUSED(elementId);

	qReal::IdList listOfElements;
	for (const qReal::Id elementId : logicalModelInterface->children(qReal::Id::rootId())) {
		if (elementId.element() == linksIdentifierNode->name()) {
			listOfElements << elementId;
		}
	}

	return listOfElements;
}

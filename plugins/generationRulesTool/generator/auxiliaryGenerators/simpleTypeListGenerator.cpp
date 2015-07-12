#include "simpleTypeListGenerator.h"

using namespace generationRules::generator;
using namespace simpleParser;

qReal::IdList SimpleTypeListGenerator::generatedList(const QSharedPointer<ast::Identifier> &simpleIdentifierNode
		, qReal::LogicalModelAssistInterface *logicalModelInterface)
{
	qReal::IdList listOfElements;
	for (const auto &elementId : logicalModelInterface->children(qReal::Id::rootId())) {
		if (elementId.element() == simpleIdentifierNode->name()) {
			listOfElements << elementId;
		}
	}

	return listOfElements;
}

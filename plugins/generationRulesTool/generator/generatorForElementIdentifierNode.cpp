#include "generatorForElementIdentifierNode.h"

#include <ast/transitionEnd.h>
#include <ast/transitionStart.h>
#include <ast/identifier.h>

using namespace generationRules::generator;

qReal::IdList GeneratorForElementIdentifierNode::neededElementId(
		QSharedPointer<simpleParser::ast::ElementIdentifier> elementIdentifierNode
		, qReal::LogicalModelAssistInterface *logicalModelInterface
		, VariablesTable tableOfVariables
		, qReal::EditorManagerInterface *editorManagerInterface
		, const qReal::Id elementId
		, const QString &elementType
		, const QString &elementName)
{
	Q_UNUSED(tableOfVariables);
	Q_UNUSED(editorManagerInterface);
	Q_UNUSED(elementType);
	Q_UNUSED(elementName);

	auto firstPart = elementIdentifierNode->identifierPart();
	auto secondPart = elementIdentifierNode->optionalTransitionPart();

	if (secondPart) {
		// we have to return transitionEnd or transitionStart for this element
		if (secondPart->is<simpleParser::ast::TransitionEnd>()) {
			return {logicalModelInterface->to(elementId)};
		} else if (secondPart->is<simpleParser::ast::TransitionStart>()) {
			return {logicalModelInterface->from(elementId)};
		} else {
			// TODO: throw exception
			qDebug() << "ERROR";
			return {qReal::Id::rootId()};
		}
	} else {
		// we need all elements in model with given name
		auto typeName = qrtext::as<simpleParser::ast::Identifier>(firstPart)->name();
		qReal::IdList listOfElementIds;

		for (const qReal::Id elementId : logicalModelInterface->children(qReal::Id::rootId())) {
			if (elementId.element() == typeName) {
				listOfElementIds << elementId;
			}
		}

		return listOfElementIds;
	}
}

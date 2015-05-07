#include "generatorForElementIdentifierNode.h"

#include <ast/transitionEnd.h>
#include <ast/transitionStart.h>
#include <ast/identifier.h>

using namespace generationRules::generator;
using namespace simpleParser::ast;

qReal::Id GeneratorForElementIdentifierNode::neededElementId(
		QSharedPointer<ElementIdentifier> elementIdentifierNode
		, GeneratorConfigurer generatorConfigurer)
{
	auto identifierPart = qrtext::as<Identifier>(elementIdentifierNode->identifierPart());
	auto optionalTransitionPart = elementIdentifierNode->optionalTransitionPart();

	auto logicalModelInterface = generatorConfigurer.logicalModelInterface();
	auto elementId = generatorConfigurer.variablesTable().currentId(identifierPart->name());

	if (optionalTransitionPart) {
		// we have to return transitionEnd or transitionStart for this element
		if (optionalTransitionPart->is<TransitionEnd>()) {
			return logicalModelInterface->to(elementId);
		} else if (optionalTransitionPart->is<TransitionStart>()) {
			return logicalModelInterface->from(elementId);
		} else {
			// TODO: throw exception
			qDebug() << "ERROR";
			return qReal::Id::rootId();
		}
	} else {
		return elementId;
	}
}

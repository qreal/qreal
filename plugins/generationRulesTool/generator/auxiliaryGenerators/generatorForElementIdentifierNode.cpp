#include "generatorForElementIdentifierNode.h"

#include <ast/this.h>
#include <ast/transitionEnd.h>
#include <ast/transitionStart.h>
#include <ast/identifier.h>

using namespace generationRules::generator;
using namespace simpleParser::ast;

qReal::Id GeneratorForElementIdentifierNode::neededElementId(
		QSharedPointer<ElementIdentifier> elementIdentifierNode
		, GeneratorConfigurer generatorConfigurer)
{
	const auto identifierPart = elementIdentifierNode->identifierPart();
	const auto optionalTransitionPart = elementIdentifierNode->optionalTransitionPart();

	const auto logicalModelInterface = generatorConfigurer.logicalModelInterface();

	qReal::Id elementId;
	if (identifierPart->is<Identifier>()) {
		elementId = generatorConfigurer.variablesTable().currentId(qrtext::as<Identifier>(identifierPart)->name());
	} else {
		elementId = generatorConfigurer.currentScope().currentId();
	}

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

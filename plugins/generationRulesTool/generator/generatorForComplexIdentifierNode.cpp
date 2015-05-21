#include "generatorForComplexIdentifierNode.h"
#include "generatorForElementIdentifierNode.h"

#include "ast/identifier.h"
#include "ast/elementIdentifier.h"
#include "ast/transitionEnd.h"

#include "generatorForElementIdentifierNode.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

QVariant GeneratorForComplexIdentifierNode::generatedResult(QSharedPointer<ComplexIdentifier> complexIdentifierNode
		, GeneratorConfigurer generatorConfigurer)
{
	auto identifierPart = qrtext::as<ElementIdentifier>(complexIdentifierNode->identifierPart());
	auto elementId = GeneratorForElementIdentifierNode::neededElementId(identifierPart, generatorConfigurer);
	auto propertyPart = qrtext::as<Identifier>(complexIdentifierNode->propertyPart());

	auto logicalModelInterface = generatorConfigurer.logicalModelInterface();
	return logicalModelInterface->propertyByRoleName(elementId, propertyPart->name());
}

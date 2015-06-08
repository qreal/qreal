#include "generatorForComplexIdentifierNode.h"
#include "auxiliaryGenerators/generatorForElementIdentifierNode.h"

#include "ast/identifier.h"
#include "ast/elementIdentifier.h"
#include "ast/transitionEnd.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

QVariant GeneratorForComplexIdentifierNode::generatedResult(const QSharedPointer<ComplexIdentifier> &complexIdentifierNode
		, GeneratorConfigurer generatorConfigurer)
{
	const auto identifierPart = qrtext::as<ElementIdentifier>(complexIdentifierNode->identifierPart());
	const auto elementId = GeneratorForElementIdentifierNode::neededElementId(identifierPart, generatorConfigurer);
	const auto propertyPart = qrtext::as<Identifier>(complexIdentifierNode->propertyPart());

	const auto logicalModelInterface = generatorConfigurer.logicalModelInterface();
	return logicalModelInterface->propertyByRoleName(elementId, propertyPart->name());
}

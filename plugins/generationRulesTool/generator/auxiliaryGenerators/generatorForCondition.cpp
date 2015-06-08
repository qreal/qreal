#include "generatorForCondition.h"

#include <plugins/pluginManager/editorManagerInterface.h>

#include "ast/complexIdentifier.h"
#include "ast/equal.h"
#include "ast/notEqual.h"
#include "ast/text.h"
#include "generator/generatorForComplexIdentifierNode.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

bool ConditionGenerator::conditionSatisfied(const QSharedPointer<Condition> &conditionNode
		, GeneratorConfigurer generatorConfigurer)
{
	const auto complexIdentifierNode = qrtext::as<ComplexIdentifier>(conditionNode->firstPart());

	// TODO: find property type in metamodel and convert from QVariant into this type.
	const auto generatedResultForComplexIdentifier = GeneratorForComplexIdentifierNode::generatedResult(complexIdentifierNode, generatorConfigurer);
	const auto comparedTextNode = qrtext::as<Text>(conditionNode->secondPart());

	// it is bool identifier
	if (!comparedTextNode) {
		return generatedResultForComplexIdentifier.toBool();
	}

	const auto comparedString = comparedTextNode->text();

	const auto stringRepresentation = generatedResultForComplexIdentifier.toString();
	const auto comparator = conditionNode->comparator();

	if (comparator->is<Equal>()) {
		return stringRepresentation == comparedString;
	} else {
		return stringRepresentation != comparedString;
	}
}

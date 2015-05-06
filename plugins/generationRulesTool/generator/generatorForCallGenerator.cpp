#include "generatorForCallGenerator.h"

#include "commonGenerator.h"
#include "generatorForGeneratorNode.h"

#include "ast/identifier.h"

#include "treeGeneratorFromString.h"

#include <qrgui/plugins/pluginManager/editorManagerInterface.h>

using namespace generationRules::generator;

QString GeneratorForCallGenerator::generatedResult(QSharedPointer<simpleParser::ast::CallGeneratorFor> callGeneratorForNode
		, GeneratorConfigurer generatorConfigurer
		, const QString &generatorName)
{
	auto identifier = qrtext::as<simpleParser::ast::Identifier>(callGeneratorForNode->identifier())->name();
	auto elementId = generatorConfigurer.variablesTable().currentId(identifier);

	auto generatorNameNode = callGeneratorForNode->generatorName();

	// another stupid hack
	// we need to have current editor id and diagram id
	// and it can be metaEntityEdge
	auto diagramId = generatorConfigurer.diagramId();
	auto editorManagerInterface = generatorConfigurer.editorManagerInterface();
	qReal::Id currentElementId = editorManagerInterface->elementsWithTheSameName(diagramId, identifier, "MetaEntityNode").first();

	auto generationRuleForCurrentElement = editorManagerInterface->generationRule(currentElementId);
	QSharedPointer<simpleParser::ast::Node> generatedTree = TreeGeneratorFromString::generatedTreeFromString(generationRuleForCurrentElement);

	auto logicalModelInterface = generatorConfigurer.logicalModelInterface();
	// now we have to iterate all elements in the model with "identifier" type
	qReal::IdList listOfElements;
	for (const qReal::Id element : logicalModelInterface->children(elementId)) {
		if (element.element() == identifier) {
			listOfElements << element;
		}
	}

	if (!generatorNameNode) {
		QString result = "";

		for (auto element : listOfElements) {
			result += CommonGenerator::generatedResult(generatedTree, generatorConfigurer, generatorName);
		}

		return result;
	} else {
		QString generatorName = qrtext::as<simpleParser::ast::Identifier>(generatorNameNode)->name();
		QString result = "";

		for (auto element : listOfElements) {
			result += CommonGenerator::generatedResult(generatedTree, generatorConfigurer);
		}
	}
}

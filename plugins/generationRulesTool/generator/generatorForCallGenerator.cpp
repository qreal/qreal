#include "generatorForCallGenerator.h"

#include "commonGenerator.h"
#include "generatorForGeneratorNode.h"

#include "ast/identifier.h"
#include "ast/elementIdentifier.h"

#include "treeGeneratorFromString.h"
#include "generatorForElementIdentifierNode.h"
#include "typeQualifier.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

QString GeneratorForCallGenerator::generatedResult(QSharedPointer<CallGeneratorFor> callGeneratorForNode
		, GeneratorConfigurer generatorConfigurer)
{
	auto calledIdentifier = qrtext::as<ElementIdentifier>(callGeneratorForNode->identifier());
	auto currentElementId = GeneratorForElementIdentifierNode::neededElementId(calledIdentifier, generatorConfigurer);
	auto currentElementType = TypeQualifier::elementIdentifierType(calledIdentifier, generatorConfigurer);

	auto diagramId = generatorConfigurer.diagramId();
	auto editorManagerInterface = generatorConfigurer.editorManagerInterface();

	auto generatorNameNode = callGeneratorForNode->generatorName();

	qReal::Id elementIdInMetamodel = idInMetamodel(editorManagerInterface, currentElementType, diagramId);

	auto generationRuleForCurrentElement = editorManagerInterface->generationRule(elementIdInMetamodel);
	QSharedPointer<Node> generatedTree = TreeGeneratorFromString::generatedTreeFromString(generationRuleForCurrentElement);

	generatorConfigurer.currentScope().changeCurrentId(currentElementId);

	QString resultOfGeneration = "";
	if (!generatorNameNode) {
		resultOfGeneration = CommonGenerator::generatedResult(generatedTree, generatorConfigurer);
	} else {
		QString generatorName = qrtext::as<Identifier>(generatorNameNode)->name();
		generatorConfigurer.currentScope().changeCurrentGeneratorName(generatorName);

		resultOfGeneration = CommonGenerator::generatedResult(generatedTree, generatorConfigurer);
	}

	generatorConfigurer.currentScope().removeLastCurrentId();

	return resultOfGeneration;
}

qReal::Id GeneratorForCallGenerator::idInMetamodel(qReal::EditorManagerInterface *editorManagerInterface
		, const QString &elementName
		, const qReal::Id &diagramId)
{
	qReal::Id elementId;

	if (!editorManagerInterface->elementsWithTheSameName(diagramId, elementName, "MetaEntityNode").isEmpty()) {
		elementId = editorManagerInterface->elementsWithTheSameName(diagramId, elementName, "MetaEntityNode").first();
	} else {
		if (!editorManagerInterface->elementsWithTheSameName(diagramId, elementName, "MetaEntityEdge").isEmpty()) {
			elementId = editorManagerInterface->elementsWithTheSameName(diagramId, elementName, "MetaEntityEdge").first();
		} else {
			qDebug() << "Element " + elementName + " in metamodel for callGeneratorFor not found!";
			elementId = qReal::Id::rootId();
		}
	}

	return elementId;
}

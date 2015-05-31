#include "generatorForCallGenerator.h"

#include "commonGenerator.h"
#include "generatorForGeneratorNode.h"

#include "ast/identifier.h"

#include "treeGeneratorFromString.h"
#include "auxiliaryGenerators/generatorForElementIdentifierNode.h"
#include "commonInfo/typeQualifier.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

QString GeneratorForCallGenerator::generatedResult(QSharedPointer<CallGeneratorFor> callGeneratorForNode
		, GeneratorConfigurer generatorConfigurer)
{
	const auto calledIdentifier = qrtext::as<ElementIdentifier>(callGeneratorForNode->identifier());
	const auto generatorNameNode = qrtext::as<Identifier>(callGeneratorForNode->generatorName());

	return commonGeneratedString(calledIdentifier, generatorNameNode, generatorConfigurer);
}

QString GeneratorForCallGenerator::commonGeneratedString(
		QSharedPointer<ElementIdentifier> calledIdentifier
		, QSharedPointer<Identifier> generatorNameNode
		, GeneratorConfigurer generatorConfigurer)
{
	const auto currentElementId = GeneratorForElementIdentifierNode::neededElementId(calledIdentifier, generatorConfigurer);
	const auto currentElementType = TypeQualifier::elementIdentifierType(calledIdentifier, generatorConfigurer);

	const auto diagramId = generatorConfigurer.diagramId();
	const auto editorManagerInterface = generatorConfigurer.editorManagerInterface();

	const auto elementIdInMetamodel = idInMetamodel(editorManagerInterface, currentElementType, diagramId);

	const auto generationRuleForCurrentElement = editorManagerInterface->generationRule(elementIdInMetamodel);
	QSharedPointer<Node> generatedTree = TreeGeneratorFromString::generatedTreeFromString(generationRuleForCurrentElement);

	generatorConfigurer.currentScope().changeCurrentId(currentElementId);

	QString resultOfGeneration = "";
	if (!generatorNameNode) {
		resultOfGeneration = CommonGenerator::generatedResult(generatedTree, generatorConfigurer);
	} else {
		QString generatorName = generatorNameNode->name();
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

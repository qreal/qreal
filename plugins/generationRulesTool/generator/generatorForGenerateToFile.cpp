#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "generatorForGenerateToFile.h"

#include "ast/elementIdentifier.h"
#include "ast/string.h"

#include "generatorForElementIdentifierNode.h"
#include "commonGenerator.h"
#include "typeQualifier.h"
#include "treeGeneratorFromString.h"
#include "auxiliaryGenerators/stringGenerator.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

QString GeneratorForGenerateToFile::generatedResult(
		QSharedPointer<GenerateToFile> generateToFileNode
		, GeneratorConfigurer generatorConfigurer)
{
	auto calledIdentifier = qrtext::as<ElementIdentifier>(generateToFileNode->identifier());
	auto currentElementId = GeneratorForElementIdentifierNode::neededElementId(calledIdentifier, generatorConfigurer);
	auto currentElementType = TypeQualifier::elementIdentifierType(calledIdentifier, generatorConfigurer);

	auto diagramId = generatorConfigurer.diagramId();
	auto editorManagerInterface = generatorConfigurer.editorManagerInterface();

	auto fileNameNode = qrtext::as<String>(generateToFileNode->fileName());
	auto fileName = StringGenerator::generatedString(fileNameNode, generatorConfigurer);
	auto pathToCode = generatorConfigurer.pathToGeneratedCode();

	auto generatorNameNode = generateToFileNode->generatorName();

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

	writeToFile(resultOfGeneration, fileName, pathToCode);

	return resultOfGeneration;
}

void GeneratorForGenerateToFile::writeToFile(
		const QString &resultOfGeneration
		, const QString &fileName
		, const QString &pathToCode)
{
	QFile outputFile(pathToCode + "/" + fileName);
	qDebug() << pathToCode + "/" + fileName;

	if (outputFile.open(QIODevice::WriteOnly)) {
		QTextStream stream(&outputFile);
		stream << resultOfGeneration;

		outputFile.close();
	}
}

qReal::Id GeneratorForGenerateToFile::idInMetamodel(qReal::EditorManagerInterface *editorManagerInterface
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

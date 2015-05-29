#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "generatorForGenerateToFile.h"

#include "ast/elementIdentifier.h"
#include "ast/string.h"

#include "auxiliaryGenerators/generatorForElementIdentifierNode.h"
#include "commonGenerator.h"
#include "commonInfo/typeQualifier.h"
#include "treeGeneratorFromString.h"
#include "auxiliaryGenerators/stringGenerator.h"

#include "generatorForCallGenerator.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

QString GeneratorForGenerateToFile::generatedResult(
		QSharedPointer<GenerateToFile> generateToFileNode
		, GeneratorConfigurer generatorConfigurer)
{
	const auto calledIdentifier = qrtext::as<ElementIdentifier>(generateToFileNode->identifier());
	const auto generatorNameNode = qrtext::as<Identifier>(generateToFileNode->generatorName());

	const auto fileNameNode = qrtext::as<String>(generateToFileNode->fileName());
	const auto fileName = StringGenerator::generatedString(fileNameNode, generatorConfigurer);
	const auto pathToCode = generatorConfigurer.pathToGeneratedCode();

	const auto resultOfGeneration = GeneratorForCallGenerator::commonGeneratedString(calledIdentifier, generatorNameNode, generatorConfigurer);

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

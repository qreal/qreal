#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "generatorForGenerateToFile.h"

#include "ast/elementIdentifier.h"
#include "ast/string.h"
#include "ast/text.h"
#include "ast/program.h"

#include "auxiliaryGenerators/generatorForElementIdentifierNode.h"
#include "commonGenerator.h"
#include "commonInfo/typeQualifier.h"
#include "treeGeneratorFromString.h"
#include "auxiliaryGenerators/stringGenerator.h"

#include "generatorForProgramNode.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

QString GeneratorForGenerateToFile::generatedResult(
		const QSharedPointer<GenerateToFile> &generateToFileNode
		, const GeneratorConfigurer &generatorConfigurer
		, ScopeInfo &scopeInfo)
{
	const auto programNode = qrtext::as<Program>(generateToFileNode->program());
	const auto fileNameNode = generateToFileNode->fileName();

	QString fileName;
	if (fileNameNode->is<String>()) {
		fileName = StringGenerator::generatedString(qrtext::as<String>(fileNameNode), generatorConfigurer
				, scopeInfo);
	} else {
		fileName = qrtext::as<Text>(fileNameNode)->text();
	}

	const auto pathToCode = generatorConfigurer.pathToGeneratedCode();

	const auto resultOfGeneration = GeneratorForProgramNode::generatedResult(programNode, generatorConfigurer
			, scopeInfo);

	writeToFile(resultOfGeneration, fileName, pathToCode);

	return resultOfGeneration;
}

void GeneratorForGenerateToFile::writeToFile(
		const QString &resultOfGeneration
		, const QString &fileName
		, const QString &pathToCode)
{
	QFile outputFile(pathToCode + "/" + fileName);

	if (outputFile.open(QIODevice::WriteOnly)) {
		QTextStream stream(&outputFile);
		stream << resultOfGeneration;

		outputFile.close();
	}
}

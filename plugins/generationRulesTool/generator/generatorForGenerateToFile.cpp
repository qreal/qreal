/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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

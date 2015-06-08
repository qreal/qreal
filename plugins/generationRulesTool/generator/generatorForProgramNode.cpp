#include "generatorForProgramNode.h"
#include "commonGenerator.h"

#include "ast/node.h"
#include "ast/generateToFile.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

QString GeneratorForProgramNode::generatedResult(const QSharedPointer<Program> &programNode
		, const GeneratorConfigurer &generatorConfigurer)
{
	QString result;

	const auto statements = programNode->children();
	for (const auto statement : statements) {
		const auto currentResult = CommonGenerator::generatedResult(statement, generatorConfigurer);
		if (!statement->is<GenerateToFile>()) {
			result += currentResult;
		}
	}

	return result;
}

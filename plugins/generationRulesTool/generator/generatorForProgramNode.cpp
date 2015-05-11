#include "generatorForProgramNode.h"
#include "commonGenerator.h"

#include "ast/node.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

QString GeneratorForProgramNode::generatedResult(QSharedPointer<Program> programNode
		, GeneratorConfigurer generatorConfigurer)
{
	QString result;

	QList<QSharedPointer<Node>> statements = programNode->children();
	for (QSharedPointer<Node> statement : statements) {
		result += CommonGenerator::generatedResult(statement, generatorConfigurer);
	}

	return result;
}

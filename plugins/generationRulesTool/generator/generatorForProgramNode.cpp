#include "generatorForProgramNode.h"
#include "commonGenerator.h"

#include "ast/node.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

QString GeneratorForProgramNode::generatedResult(QSharedPointer<Program> programNode
		, GeneratorConfigurer generatorConfigurer
		, const QString &generatorName
		, qReal::Id const elementId
		, const QString &elementType
		, const QString &elementName)
{
	QString result;

	QList<QSharedPointer<Node>> statements = programNode->children();
	for (QSharedPointer<Node> statement : statements) {
		result += CommonGenerator::generatedResult(statement, generatorConfigurer, generatorName
				, elementId, elementType, elementName);
	}

	return result;
}

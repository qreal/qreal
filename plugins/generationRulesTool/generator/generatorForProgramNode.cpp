#include "generatorForProgramNode.h"
#include "commonGenerator.h"

#include "ast/node.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

QString GeneratorForProgramNode::generatedResult(QSharedPointer<Program> programNode
		, qrRepo::LogicalRepoApi *metamodelRepoApi
		, qReal::Id const elementId)
{
	QString result;

	QList<QSharedPointer<Node>> statements = programNode->children();
	for (QSharedPointer<Node> statement : statements) {
		QString newResult = CommonGenerator::generatedResult(statement, metamodelRepoApi, elementId);
		qDebug() << newResult;
		result += newResult;//CommonGenerator::generatedResult(statement, metamodelRepoApi, elementId);
		result += "\n";
	}

	return result;
}

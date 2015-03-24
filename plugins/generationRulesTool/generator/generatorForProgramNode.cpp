#include "generatorForProgramNode.h"
#include "commonGenerator.h"

#include "ast/node.h"

#include "variablesTable.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

QString GeneratorForProgramNode::generatedResult(QSharedPointer<Program> programNode
		, qrRepo::LogicalRepoApi *metamodelRepoApi
		, qrRepo::RepoControlInterface *modelRepo
		, qReal::LogicalModelAssistInterface *logicalModelInterface
		, VariablesTable tableOfVariables
		, qReal::Id const elementId
		, const QString &elementType
		, const QString &elementName)
{
	QString result;

	QList<QSharedPointer<Node>> statements = programNode->children();
	for (QSharedPointer<Node> statement : statements) {
		result += CommonGenerator::generatedResult(statement, metamodelRepoApi
				, modelRepo, logicalModelInterface, tableOfVariables, elementId, elementType, elementName);
	}

	return result;
}

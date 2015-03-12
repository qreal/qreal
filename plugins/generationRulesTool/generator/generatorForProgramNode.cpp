#include "generatorForProgramNode.h"
#include "commonGenerator.h"

#include "ast/node.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

QString GeneratorForProgramNode::generatedResult(QSharedPointer<Program> programNode
		, qrRepo::LogicalRepoApi *metamodelRepoApi
		, qrRepo::RepoControlInterface *modelRepo
		, qReal::LogicalModelAssistInterface *logicalModelInterface
		, qReal::Id const elementId)
{
	QString result;

	QList<QSharedPointer<Node>> statements = programNode->children();
	for (QSharedPointer<Node> statement : statements) {
		result += CommonGenerator::generatedResult(statement, metamodelRepoApi
				, modelRepo, logicalModelInterface, elementId);
	}

	return result;
}

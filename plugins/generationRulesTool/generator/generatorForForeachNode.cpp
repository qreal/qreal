#include "generatorForForeachNode.h"
#include "generatorForProgramNode.h"

#include "ast/identifier.h"
#include "ast/program.h"

using namespace generationRules::generator;
using namespace simpleParser;

QString GeneratorForForeachNode::generatedResult(QSharedPointer<ast::Foreach> foreachNode
			, qrRepo::LogicalRepoApi *metamodelRepoApi)
{
	QSharedPointer<ast::Identifier> type = qrtext::as<ast::Identifier>(foreachNode->identifier());
	QString result;

	QList<qReal::Id> listOfElements = metamodelRepoApi->elementsByType(type->name());
	for (const qReal::Id element : listOfElements) {
		QSharedPointer<ast::Program> programNode = qrtext::as<ast::Program>(foreachNode->program());
		result += GeneratorForProgramNode::generatedResult(programNode, metamodelRepoApi, element);
	}
}

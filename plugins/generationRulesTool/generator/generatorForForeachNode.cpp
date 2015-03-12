#include "generatorForForeachNode.h"
#include "generatorForProgramNode.h"

#include "ast/identifier.h"
#include "ast/program.h"

#include "plugins/pluginManager/editorManagerInterface.h"

using namespace generationRules::generator;
using namespace simpleParser;

QString GeneratorForForeachNode::generatedResult(QSharedPointer<ast::Foreach> foreachNode
			, qrRepo::LogicalRepoApi *metamodelRepoApi
			, qrRepo::RepoControlInterface *modelRepo
			, qReal::LogicalModelAssistInterface *logicalModelInterface)
{
	QSharedPointer<ast::Identifier> identifier = qrtext::as<ast::Identifier>(foreachNode->identifier());
	QSharedPointer<ast::Identifier> type = qrtext::as<ast::Identifier>(foreachNode->type());

	QList<qReal::Id> listOfElements;
	for (const qReal::Id elementId : logicalModelInterface->children(qReal::Id::rootId())) {
		if (elementId.element() == type->name()) {
			listOfElements << elementId;
		}
	}

	QString result;
	for (const qReal::Id element : listOfElements) {
		QSharedPointer<ast::Program> programNode = qrtext::as<ast::Program>(foreachNode->program());
		result += GeneratorForProgramNode::generatedResult(programNode, metamodelRepoApi
				, modelRepo, logicalModelInterface, element);
	}

	return result;
}

#include "generatorForForeachNode.h"
#include "generatorForProgramNode.h"

#include "ast/identifier.h"
#include "ast/outcomingLinks.h"
#include "ast/program.h"

#include "plugins/pluginManager/editorManagerInterface.h"

#include "auxiliaryGenerators/outcomingLinksListGenerator.h"
#include "auxiliaryGenerators/simpleTypeListGenerator.h"

using namespace generationRules::generator;
using namespace simpleParser;

QString GeneratorForForeachNode::generatedResult(QSharedPointer<ast::Foreach> foreachNode
			, qrRepo::LogicalRepoApi *metamodelRepoApi
			, qrRepo::RepoControlInterface *modelRepo
			, qReal::LogicalModelAssistInterface *logicalModelInterface
			, const qReal::Id elementId)
{
	QSharedPointer<ast::Node> identifierName = foreachNode->identifier();
	QSharedPointer<ast::Node> identifierToIterate = foreachNode->type();
	QSharedPointer<ast::Node> optionalLinkPart = foreachNode->optionalLinkPart();

	qReal::IdList listOfElements;
	if (optionalLinkPart.isNull()) {
		listOfElements = SimpleTypeListGenerator::generatedList(qrtext::as<ast::Identifier>(identifierToIterate)
				, logicalModelInterface, elementId);
	} else {
		listOfElements = OutcomingLinksListGenerator::generatedList(qrtext::as<ast::Identifier>(identifierToIterate)
				, logicalModelInterface, elementId);
	}

	QString result;
	for (const qReal::Id element : listOfElements) {
		QSharedPointer<ast::Program> programNode = qrtext::as<ast::Program>(foreachNode->program());
		result += GeneratorForProgramNode::generatedResult(programNode, metamodelRepoApi
				, modelRepo, logicalModelInterface, element);
	}

	return result;
}

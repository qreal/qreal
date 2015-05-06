#include "generatorForForeachNode.h"
#include "generatorForProgramNode.h"

#include "ast/identifier.h"
#include "ast/outcomingLinks.h"
#include "ast/program.h"
#include "ast/list.h"

#include "plugins/pluginManager/editorManagerInterface.h"

#include "auxiliaryGenerators/listGenerator.h"
#include "auxiliaryGenerators/identifierTypeGenerator.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

QString GeneratorForForeachNode::generatedResult(QSharedPointer<Foreach> foreachNode
			, GeneratorConfigurer generatorConfigurer
			, const QString &generatorName)
{
	auto identifierPart = qrtext::as<Identifier>(foreachNode->identifier());
	auto identifierName = identifierPart->name();

	auto listPart = qrtext::as<List>(foreachNode->listPart());
	auto logicalModelInterface = generatorConfigurer.logicalModelInterface();

	auto identifierType = IdentifierTypeGenerator::variableType(listPart);

	qReal::IdList listOfElements = ListGenerator::listOfIds(listPart, logicalModelInterface, generatorConfigurer.variablesTable());

	QString result;
	generatorConfigurer.variablesTable().addNewVariable(identifierName, identifierType, listOfElements);

	for (const qReal::Id element : listOfElements) {
		QSharedPointer<Program> programNode = qrtext::as<Program>(foreachNode->program());
		result += GeneratorForProgramNode::generatedResult(programNode, generatorConfigurer, generatorName);
	}

	generatorConfigurer.variablesTable().removeVariable(identifierName);

	return result;
}

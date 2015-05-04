#include "generatorForForeachNode.h"
#include "generatorForProgramNode.h"

#include "ast/identifier.h"
#include "ast/outcomingLinks.h"
#include "ast/program.h"
#include "ast/list.h"

#include "plugins/pluginManager/editorManagerInterface.h"

#include "auxiliaryGenerators/listGenerator.h"

using namespace generationRules::generator;
using namespace simpleParser;

QString GeneratorForForeachNode::generatedResult(QSharedPointer<ast::Foreach> foreachNode
			, GeneratorConfigurer generatorConfigurer
			, const QString &generatorName
			, const qReal::Id elementId
			, const QString &basicElementType
			, const QString &basicElementName)
{
	// TODO: do something in these situations: foreach () {foreach () {...}}
	Q_UNUSED(basicElementType);
	Q_UNUSED(basicElementName);

	QSharedPointer<ast::Identifier> identifier = qrtext::as<ast::Identifier>(foreachNode->identifier());
	auto elementName = identifier->name();

	QString elementType = "";

	auto listNode = qrtext::as<simpleParser::ast::List>(foreachNode->listPart());
	auto logicalModelInterface = generatorConfigurer.logicalModelInterface();

	qReal::IdList listOfElements = ListGenerator::listOfIds(listNode, logicalModelInterface, elementId);

	QString result;
	for (const qReal::Id element : listOfElements) {
		QSharedPointer<ast::Program> programNode = qrtext::as<ast::Program>(foreachNode->program());
		result += GeneratorForProgramNode::generatedResult(programNode, generatorConfigurer, generatorName, element, elementType, elementName);
	}

	generatorConfigurer.variablesTable().removeVariable(elementName);

	return result;
}

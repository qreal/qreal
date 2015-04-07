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
			, qReal::LogicalModelAssistInterface *logicalModelInterface
			, VariablesTable tableOfVariables
			, qReal::EditorManagerInterface *editorManagerInterface
			, const qReal::Id elementId
			, const QString &basicElementType
			, const QString &basicElementName)
{
	// TODO: do something in these situations: foreach () {foreach () {...}}
	Q_UNUSED(basicElementType);

	QSharedPointer<ast::Identifier> identifier = qrtext::as<ast::Identifier>(foreachNode->identifier());
	auto elementName = identifier->name();

	QSharedPointer<ast::Node> identifierToIterate = foreachNode->type();
	QSharedPointer<ast::Node> optionalLinkPart = foreachNode->optionalLinkPart();

	QString elementType;

	qReal::IdList listOfElements;
	if (optionalLinkPart.isNull()) {
		listOfElements = SimpleTypeListGenerator::generatedList(qrtext::as<ast::Identifier>(identifierToIterate)
				, logicalModelInterface, elementId);

		elementType = qrtext::as<ast::Identifier>(identifierToIterate)->name();
	} else {
		listOfElements = OutcomingLinksListGenerator::generatedList(qrtext::as<ast::Identifier>(identifierToIterate)
				, logicalModelInterface, elementId);

		auto linkTypeIdentifier = qrtext::as<ast::Identifier>(
				qrtext::as<ast::OutcomingLinks>(optionalLinkPart)->linkType());
		elementType = linkTypeIdentifier->name();
	}

	tableOfVariables.addNewVariable(elementName, elementType);

	QString result;
	for (const qReal::Id element : listOfElements) {
		QSharedPointer<ast::Program> programNode = qrtext::as<ast::Program>(foreachNode->program());
		result += GeneratorForProgramNode::generatedResult(programNode, logicalModelInterface, tableOfVariables
				, editorManagerInterface, element, elementType, elementName);
	}

	tableOfVariables.removeVariable(elementName);

	return result;
}

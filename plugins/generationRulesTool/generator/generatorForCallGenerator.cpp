#include "generatorForCallGenerator.h"

#include "commonGenerator.h"
#include "ast/identifier.h"

#include "treeGeneratorFromString.h"

#include <qrgui/plugins/pluginManager/editorManagerInterface.h>

using namespace generationRules::generator;

QString GeneratorForCallGenerator::generatedResult(QSharedPointer<simpleParser::ast::CallGeneratorFor> callGeneratorForNode
		, qReal::LogicalModelAssistInterface *logicalModelInterface
		, VariablesTable tableOfVariables
		, qReal::EditorManagerInterface *editorManagerInterface
		, qReal::Id const elementId
		, const QString &elementType
		, const QString &elementName
		)
{
	Q_UNUSED(elementType);
	Q_UNUSED(elementName);

	auto identifier = qrtext::as<simpleParser::ast::Identifier>(callGeneratorForNode->identifier())->name();
	auto generatorNameNode = callGeneratorForNode->generatorName();

	// another stupid hack
	// we need to have current editor id and diagram id
	// and it can be metaEntityEdge
	auto diagramId = editorManagerInterface->diagrams(editorManagerInterface->editors().first()).first();
	qReal::Id currentElementId = editorManagerInterface->elementsWithTheSameName(diagramId, identifier, "MetaEntityNode").first();

	auto generationRuleForCurrentElement = editorManagerInterface->generationRule(currentElementId);
	QSharedPointer<simpleParser::ast::Node> generatedTree = TreeGeneratorFromString::generatedTreeFromString(generationRuleForCurrentElement);

	// now we have to iterate all elements in the model with "identifier" type
	qReal::IdList listOfElements;
	for (const qReal::Id element : logicalModelInterface->children(elementId)) {
		if (element.element() == identifier) {
			listOfElements << element;
		}
	}

	if (!generatorNameNode) {
		QString result = "";

		for (auto element : listOfElements) {
			result += CommonGenerator::generatedResult(generatedTree, logicalModelInterface, tableOfVariables
					, editorManagerInterface, element, identifier, identifier);
		}

		return result;
	} else {
		return "Not supported yet";
	}
}

#include "commonGenerator.h"

#include "generatorForProgramNode.h"
#include "generatorForComplexIdentifierNode.h"
#include "generatorForForeachNode.h"
#include "generatorForCallGenerator.h"

#include "ast/program.h"
#include "ast/complexIdentifier.h"
#include "ast/foreach.h"
#include "ast/identifier.h"
#include "ast/newline.h"
#include "ast/text.h"
#include "ast/callGeneratorFor.h"
#include "ast/tab.h"

using namespace generationRules::generator;
using namespace simpleParser;

QString CommonGenerator::generatedResult(QSharedPointer<ast::Node> node
		, qReal::LogicalModelAssistInterface *logicalModelInterface
		, VariablesTable tableOfVariables
		, qReal::EditorManagerInterface *editorManagerInterface
		, const qReal::Id elementId
		, const QString &elementType
		, const QString &elementName)
{
	QString result;

	if (node->is<ast::Program>()) {
		QSharedPointer<ast::Program> program = qrtext::as<ast::Program>(node);
		result = GeneratorForProgramNode::generatedResult(program, logicalModelInterface
				, tableOfVariables, editorManagerInterface, elementId, elementType, elementName);
	}
	else if (node->is<ast::ComplexIdentifier>()) {
		QSharedPointer<ast::ComplexIdentifier> complexIdentifier = qrtext::as<ast::ComplexIdentifier>(node);
		result = GeneratorForComplexIdentifierNode::generatedResult(complexIdentifier, logicalModelInterface
				, tableOfVariables, editorManagerInterface, elementId, elementType, elementName);
	}
	else if (node->is<ast::Foreach>()) {
		QSharedPointer<ast::Foreach> foreachNode = qrtext::as<ast::Foreach>(node);
		result = GeneratorForForeachNode::generatedResult(foreachNode, logicalModelInterface, tableOfVariables
				, editorManagerInterface, elementId, elementType, elementName);
	}
	else if (node->is<ast::CallGeneratorFor>()) {
		QSharedPointer<ast::CallGeneratorFor> callGeneratorNode = qrtext::as<ast::CallGeneratorFor>(node);
		result = GeneratorForCallGenerator::generatedResult(callGeneratorNode, logicalModelInterface
				, tableOfVariables, editorManagerInterface, elementId, elementType, elementName);
	}
	else if (node->is<ast::Newline>()) {
		result = "\n";
	}
	else if (node->is<ast::Text>()) {
		QSharedPointer<ast::Text> text = qrtext::as<ast::Text>(node);
		result = text->text();
	}
	else if (node->is<ast::Tab>()) {
		result += "	";
	}
	else {
		qDebug() << "Something went wrong";
	}

	return result;
}

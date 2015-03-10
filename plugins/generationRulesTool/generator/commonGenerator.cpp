#include "commonGenerator.h"

#include "generatorForProgramNode.h"
#include "generatorForComplexIdentifierNode.h"
#include "generatorForForeachNode.h"

#include "ast/program.h"
#include "ast/complexIdentifier.h"
#include "ast/foreach.h"
#include "ast/identifier.h"
#include "ast/newline.h"
#include "ast/text.h"

using namespace generationRules::generator;
using namespace simpleParser;

QString CommonGenerator::generatedResult(QSharedPointer<ast::Node> node
		, qrRepo::LogicalRepoApi *metamodelRepoApi
		, const qReal::Id elementId)
{
	QString result;

	if (node->is<ast::Program>()) {
		QSharedPointer<ast::Program> program = qrtext::as<ast::Program>(node);
		result = GeneratorForProgramNode::generatedResult(program, metamodelRepoApi, elementId);
	}
	else if (node->is<ast::ComplexIdentifier>()) {
		QSharedPointer<ast::ComplexIdentifier> complexIdentifier = qrtext::as<ast::ComplexIdentifier>(node);
		result = GeneratorForComplexIdentifierNode::generatedResult(complexIdentifier, metamodelRepoApi, elementId);
	}
	else if (node->is<ast::Foreach>()) {
		QSharedPointer<ast::Foreach> foreachNode = qrtext::as<ast::Foreach>(node);
		result = GeneratorForForeachNode::generatedResult(foreachNode, metamodelRepoApi);
	}
	else if (node->is<ast::Newline>()) {
		result = "\n";
	}
	else if (node->is<ast::Text>()) {
		QSharedPointer<ast::Text> text = qrtext::as<ast::Text>(node);
		result = text->text();
	}
	else {
		qDebug() << "Something went wrong";
	}

	return result;
}

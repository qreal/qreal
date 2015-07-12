#include "generatorForGeneratorNode.h"

#include "ast/identifier.h"
#include "ast/program.h"
#include "generatorForProgramNode.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

QString GeneratorForGeneratorNode::generatedResult(
		const QSharedPointer<Generator> &generatorNode
		, const GeneratorConfigurer &generatorConfigurer
		, ScopeInfo &scopeInfo)
{
	const auto generatorNameNode = qrtext::as<Identifier>(generatorNode->identifier());
	const auto generatorName = generatorNameNode->name();

	const auto programNode = qrtext::as<Program>(generatorNode->program());

	if (scopeInfo.currentScope().currentGeneratorName() == generatorName) {
		return GeneratorForProgramNode::generatedResult(programNode, generatorConfigurer, scopeInfo);
	} else {
		return "";
	}
}

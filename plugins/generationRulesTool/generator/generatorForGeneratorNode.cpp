#include "generatorForGeneratorNode.h"

#include "ast/identifier.h"
#include "ast/program.h"
#include "generatorForProgramNode.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

QString GeneratorForGeneratorNode::generatedResult(QSharedPointer<Generator> generatorNode
			, GeneratorConfigurer generatorConfigurer)
{
	auto generatorNameNode = qrtext::as<Identifier>(generatorNode->identifier());
	auto generatorName = generatorNameNode->name();

	auto programNode = qrtext::as<Program>(generatorNode->program());

	if (generatorConfigurer.currentScope().currentGeneratorName() == generatorName) {
		return GeneratorForProgramNode::generatedResult(programNode, generatorConfigurer);
	} else {
		return "";
	}
}

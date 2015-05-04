#include "generatorForGeneratorNode.h"

#include "ast/identifier.h"
#include "ast/program.h"
#include "generatorForProgramNode.h"

using namespace generationRules::generator;

QString GeneratorForGeneratorNode::generatedResult(QSharedPointer<simpleParser::ast::Generator> generatorNode
			, GeneratorConfigurer generatorConfigurer
			, const QString &wantedGeneratorName
			, const qReal::Id elementId
			, const QString &basicElementType
			, const QString &basicElementName
			)
{
	auto generatorNameNode = qrtext::as<simpleParser::ast::Identifier>(generatorNode->identifier());
	auto generatorName = generatorNameNode->name();

	auto programNode = qrtext::as<simpleParser::ast::Program>(generatorNode->program());

	if (wantedGeneratorName == generatorName) {
		return GeneratorForProgramNode::generatedResult(programNode, generatorConfigurer,
				generatorName, elementId, basicElementType, basicElementName);
	} else {
		// TODO: throw exception
		return "Fail!!";
	}
}

#include "generatorForIfNode.h"

#include "ast/condition.h"
#include "ast/program.h"

#include "auxiliaryGenerators/generatorForCondition.h"
#include "generatorForProgramNode.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

QString GeneratorForIfNode::generatedResult(
		const QSharedPointer<IfNode> &ifNode
		, const GeneratorConfigurer &generatorConfigurer
		, ScopeInfo &scopeInfo)
{
	const auto condition = qrtext::as<Condition>(ifNode->condition());
	const auto conditionSatisfied = ConditionGenerator::conditionSatisfied(condition, generatorConfigurer
			, scopeInfo);

	const auto trueBranch = qrtext::as<Program>(ifNode->trueBranch());
	const auto falseBranch = qrtext::as<Program>(ifNode->falseBranch());

	if (conditionSatisfied) {
		return GeneratorForProgramNode::generatedResult(trueBranch, generatorConfigurer, scopeInfo);
	} else {
		if (falseBranch) {
			return GeneratorForProgramNode::generatedResult(falseBranch, generatorConfigurer
					, scopeInfo);
		}

		return "";
	}
}

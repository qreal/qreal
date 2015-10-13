/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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

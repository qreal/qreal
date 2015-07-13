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

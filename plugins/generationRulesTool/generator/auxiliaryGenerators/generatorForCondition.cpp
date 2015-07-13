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

#include "generatorForCondition.h"

#include <plugins/pluginManager/editorManagerInterface.h>

#include "ast/complexIdentifier.h"
#include "ast/equal.h"
#include "ast/notEqual.h"
#include "ast/text.h"
#include "generator/generatorForComplexIdentifierNode.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

bool ConditionGenerator::conditionSatisfied(const QSharedPointer<Condition> &conditionNode
		, const GeneratorConfigurer &generatorConfigurer
		, ScopeInfo &scopeInfo)
{
	const auto complexIdentifierNode = qrtext::as<ComplexIdentifier>(conditionNode->firstPart());

	// TODO: find property type in metamodel and convert from QVariant into this type.
	const auto generatedResultForComplexIdentifier = GeneratorForComplexIdentifierNode::generatedResult(
				complexIdentifierNode, generatorConfigurer, scopeInfo);
	const auto comparedTextNode = qrtext::as<Text>(conditionNode->secondPart());

	// it is bool identifier
	if (!comparedTextNode) {
		return generatedResultForComplexIdentifier.toBool();
	}

	const auto comparedString = comparedTextNode->text();

	const auto stringRepresentation = generatedResultForComplexIdentifier.toString();
	const auto comparator = conditionNode->comparator();

	if (comparator->is<Equal>()) {
		return stringRepresentation == comparedString;
	} else {
		return stringRepresentation != comparedString;
	}
}

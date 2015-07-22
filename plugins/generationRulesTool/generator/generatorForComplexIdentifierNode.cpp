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

#include "generatorForComplexIdentifierNode.h"
#include "auxiliaryGenerators/generatorForElementIdentifierNode.h"

#include "ast/identifier.h"
#include "ast/elementIdentifier.h"
#include "ast/transitionEnd.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

QVariant GeneratorForComplexIdentifierNode::generatedResult(
		const QSharedPointer<ComplexIdentifier> &complexIdentifierNode
		, const GeneratorConfigurer &generatorConfigurer
		, ScopeInfo &scopeInfo)
{
	const auto identifierPart = qrtext::as<ElementIdentifier>(complexIdentifierNode->identifierPart());
	const auto elementId = GeneratorForElementIdentifierNode::neededElementId(identifierPart
			, generatorConfigurer, scopeInfo);
	const auto propertyPart = qrtext::as<Identifier>(complexIdentifierNode->propertyPart());

	const auto logicalModelInterface = generatorConfigurer.logicalModelInterface();
	return logicalModelInterface->propertyByRoleName(elementId, propertyPart->name());
}

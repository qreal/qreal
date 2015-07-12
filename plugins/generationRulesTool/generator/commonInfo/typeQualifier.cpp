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

#include "typeQualifier.h"

#include "ast/identifier.h"
#include "ast/transitionEnd.h"
#include "ast/transitionStart.h"

using namespace simpleParser::ast;
using namespace generationRules::generator;

QString TypeQualifier::elementIdentifierType(
		const QSharedPointer<ElementIdentifier> &elementIdentifier
		, const GeneratorConfigurer &generatorConfigurer
		, ScopeInfo &scopeInfo)
{
	const auto identifierPart = qrtext::as<Identifier>(elementIdentifier->identifierPart());
	const auto optionalLinkPart = elementIdentifier->optionalTransitionPart();

	const auto identifierName = identifierPart->name();

	if (!optionalLinkPart) {
		return scopeInfo.variablesTable().typeByName(identifierName);
	} else {
		auto linkId = scopeInfo.variablesTable().currentId(identifierName);

		if (optionalLinkPart->is<TransitionEnd>()) {
			const auto transitionEndId = generatorConfigurer.logicalModelInterface()->to(linkId);

			return transitionEndId.element();
		} else {
			const auto transitionStartId = generatorConfigurer.logicalModelInterface()->from(linkId);

			return transitionStartId.element();
		}
	}
}

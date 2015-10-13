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

#include "generatorForElementIdentifierNode.h"

#include <ast/this.h>
#include <ast/transitionEnd.h>
#include <ast/transitionStart.h>
#include <ast/identifier.h>

using namespace generationRules::generator;
using namespace simpleParser::ast;

qReal::Id GeneratorForElementIdentifierNode::neededElementId(
		const QSharedPointer<ElementIdentifier> &elementIdentifierNode
		, const GeneratorConfigurer &generatorConfigurer
		, ScopeInfo &scopeInfo)
{
	const auto identifierPart = elementIdentifierNode->identifierPart();
	const auto optionalTransitionPart = elementIdentifierNode->optionalTransitionPart();

	const auto logicalModelInterface = generatorConfigurer.logicalModelInterface();

	qReal::Id elementId;
	if (identifierPart->is<Identifier>()) {
		elementId = scopeInfo.variablesTable().currentId(qrtext::as<Identifier>(identifierPart)->name());
	} else {
		elementId = scopeInfo.currentScope().currentId();
	}

	if (optionalTransitionPart) {
		// we have to return transitionEnd or transitionStart for this element
		if (optionalTransitionPart->is<TransitionEnd>()) {
			return logicalModelInterface->to(elementId);
		} else if (optionalTransitionPart->is<TransitionStart>()) {
			return logicalModelInterface->from(elementId);
		} else {
			// TODO: throw exception
			qDebug() << "ERROR";
			return qReal::Id::rootId();
		}
	} else {
		return elementId;
	}
}

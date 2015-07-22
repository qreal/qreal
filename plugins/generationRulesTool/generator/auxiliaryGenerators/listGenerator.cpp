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

#include "listGenerator.h"

#include "ast/identifier.h"
#include "ast/incomingLinks.h"
#include "ast/outcomingLinks.h"
#include "ast/elementIdentifier.h"

#include "incomingLinksListGenerator.h"
#include "outcomingLinksListGenerator.h"
#include "simpleTypeListGenerator.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

qReal::IdList ListGenerator::listOfIds(const QSharedPointer<List> &listNode
		, qReal::LogicalModelAssistInterface *logicalModelInterface
		, VariablesTable &variablesTable
		, const CurrentScope &currentScope)
{
	const auto mainPartOfListIdentifier = qrtext::as<ElementIdentifier>(listNode->identifier());
	// first part can be identifier or "this"
	const auto firstPart = mainPartOfListIdentifier->identifierPart();
	const auto optionalLinkPart = listNode->optionalLinkPart();

	if (!optionalLinkPart) {
		// listNode is element node => listNode is identifier node (without transitionEnd or TransitionStart)
		const auto identifierPart = qrtext::as<Identifier>(firstPart);
		return SimpleTypeListGenerator::generatedList(identifierPart, logicalModelInterface);
	} else {
		if (optionalLinkPart->is<IncomingLinks>()) {
			const auto linkType = qrtext::as<Identifier>(qrtext::as<IncomingLinks>(optionalLinkPart)->linkType());

			return IncomingLinksListGenerator::generatedList(firstPart, linkType, logicalModelInterface
					, variablesTable, currentScope);
		} else {
			const auto linkType = qrtext::as<Identifier>(qrtext::as<OutcomingLinks>(optionalLinkPart)->linkType());

			return OutcomingLinksListGenerator::generatedList(firstPart, linkType, logicalModelInterface
					, variablesTable, currentScope);
		}
	}
}

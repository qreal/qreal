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

#include "identifierTypeGenerator.h"

#include "ast/identifier.h"
#include "ast/incomingLinks.h"
#include "ast/outcomingLinks.h"
#include "ast/elementIdentifier.h"
#include "ast/this.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

QString IdentifierTypeGenerator::variableType(const QSharedPointer<List> &listNode)
{
	const auto mainPartOfListIdentifier = qrtext::as<ElementIdentifier>(listNode->identifier());

	const auto identifierPart = mainPartOfListIdentifier->identifierPart();
	const auto optionalLinkPart = listNode->optionalLinkPart();

	if (identifierPart->is<Identifier>()) {
		const auto asIdentifier = qrtext::as<Identifier>(identifierPart);

		if (!optionalLinkPart) {
			return asIdentifier->name();
		}
	}

	if (optionalLinkPart->is<IncomingLinks>()) {
		const auto linkTypeNode = qrtext::as<IncomingLinks>(optionalLinkPart)->linkType();
		const auto linkTypeIdentifier = qrtext::as<Identifier>(linkTypeNode);

		return linkTypeIdentifier->name();
	} else {
		if (optionalLinkPart->is<OutcomingLinks>()) {
			const auto linkTypeNode = qrtext::as<OutcomingLinks>(optionalLinkPart)->linkType();
			const auto linkTypeIdentifier = qrtext::as<Identifier>(linkTypeNode);

			return linkTypeIdentifier->name();
		} else {
			return "";
		}
	}
}

#include "identifierTypeGenerator.h"

#include "ast/identifier.h"
#include "ast/incomingLinks.h"
#include "ast/outcomingLinks.h"
#include "ast/elementIdentifier.h"
#include "ast/this.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

QString IdentifierTypeGenerator::variableType(QSharedPointer<List> listNode)
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

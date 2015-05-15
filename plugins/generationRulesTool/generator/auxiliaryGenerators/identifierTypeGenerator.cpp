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
	auto mainPartOfListIdentifier = qrtext::as<ElementIdentifier>(listNode->identifier());

	auto identifierPart = mainPartOfListIdentifier->identifierPart();
	auto optionalLinkPart = listNode->optionalLinkPart();

	if (identifierPart->is<Identifier>()) {
		auto asIdentifier = qrtext::as<Identifier>(identifierPart);

		if (!optionalLinkPart) {
			return asIdentifier->name();
		}
	}

	// TODO: rewrite this code.
	if (optionalLinkPart->is<IncomingLinks>()) {
		auto linkTypeNode = qrtext::as<IncomingLinks>(optionalLinkPart)->linkType();
		auto linkTypeIdentifier = qrtext::as<Identifier>(linkTypeNode);

		return linkTypeIdentifier->name();
	} else {
		if (optionalLinkPart->is<OutcomingLinks>()) {
			auto linkTypeNode = qrtext::as<OutcomingLinks>(optionalLinkPart)->linkType();
			auto linkTypeIdentifier = qrtext::as<Identifier>(linkTypeNode);

			return linkTypeIdentifier->name();
		}
	}
}

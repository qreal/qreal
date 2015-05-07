#include "typeQualifier.h"

#include "ast/identifier.h"
#include "ast/transitionEnd.h"
#include "ast/transitionStart.h"

using namespace simpleParser::ast;
using namespace generationRules::generator;

QString TypeQualifier::elementIdentifierType(QSharedPointer<ElementIdentifier> elementIdentifier, GeneratorConfigurer generatorConfigurer)
{
	auto identifierPart = qrtext::as<Identifier>(elementIdentifier->identifierPart());
	auto optionalLinkPart = elementIdentifier->optionalTransitionPart();

	auto identifierName = identifierPart->name();

	if (!optionalLinkPart) {
		return generatorConfigurer.variablesTable().typeByName(identifierName);
	} else {
		auto linkId = generatorConfigurer.variablesTable().currentId(identifierName);

		if (optionalLinkPart->is<TransitionEnd>()) {
			auto transitionEndId = generatorConfigurer.logicalModelInterface()->to(linkId);

			return transitionEndId.element();
		} else {
			auto transitionStartId = generatorConfigurer.logicalModelInterface()->from(linkId);

			return transitionStartId.element();
		}
	}
}

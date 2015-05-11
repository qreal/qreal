#include "generatorForComplexIdentifierNode.h"
#include "generatorForElementIdentifierNode.h"

#include "ast/identifier.h"
#include "ast/elementIdentifier.h"
#include "ast/transitionEnd.h"

#include "generatorForElementIdentifierNode.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

QString GeneratorForComplexIdentifierNode::generatedResult(QSharedPointer<ComplexIdentifier> complexIdentifierNode
		, GeneratorConfigurer generatorConfigurer)
{
	auto identifierPart = qrtext::as<ElementIdentifier>(complexIdentifierNode->identifierPart());
	auto elementId = GeneratorForElementIdentifierNode::neededElementId(identifierPart, generatorConfigurer);
	auto propertyPart = qrtext::as<Identifier>(complexIdentifierNode->propertyPart());

	auto logicalModelInterface = generatorConfigurer.logicalModelInterface();
	return logicalModelInterface->propertyByRoleName(elementId, propertyPart->name()).toString();
}

QString GeneratorForComplexIdentifierNode::generatedResultForNodeWithBasicName(
		qReal::Id const elementId
		, qReal::LogicalModelAssistInterface *logicalModelInterface
		, const QString &property
		)
{
	return logicalModelInterface->propertyByRoleName(elementId, property).toString();
}

QString GeneratorForComplexIdentifierNode::generatedResultForNodeWithUniqueType(
		qReal::LogicalModelAssistInterface *logicalModelInterface
		, const QString &type
		, const QString &propertyName)
{
	QList<qReal::Id> listOfElementIds;

	for (const qReal::Id elementId : logicalModelInterface->children(qReal::Id::rootId())) {
		if (elementId.element() == type) {
			listOfElementIds << elementId;
		}
	}

	if (listOfElementIds.size() != 1) {
		// TODO: throw exception
		qDebug() << "Element of type " << type << "is not unique!";
		return "";
	} else {
		return logicalModelInterface->propertyByRoleName(listOfElementIds.first(), propertyName).toString();
	}
}

QString GeneratorForComplexIdentifierNode::generatedResultForOutcomingLink(
		const qReal::Id linkId
		, qReal::LogicalModelAssistInterface *logicalModelInterface
		, const QString &propertyName)
{
	qReal::Id transitionEndId = logicalModelInterface->to(linkId);
	return logicalModelInterface->propertyByRoleName(transitionEndId, propertyName).toString();
}

QString GeneratorForComplexIdentifierNode::generatedResultForOtherVariable(
		const QString &variableName
		, qReal::LogicalModelAssistInterface *logicalModelInterface
		, const QString &propertyName)
{
	return "";
}

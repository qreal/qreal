#include "generatorForComplexIdentifierNode.h"

#include "ast/identifier.h"
#include "ast/transitionEnd.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

QString GeneratorForComplexIdentifierNode::generatedResult(QSharedPointer<ComplexIdentifier> complexIdentifierNode
		, qrRepo::LogicalRepoApi *metamodelRepoApi
		, qrRepo::RepoControlInterface *modelRepo
		, qReal::LogicalModelAssistInterface *logicalModelInterface
		, VariablesTable tableOfVariables
		, const qReal::Id elementId
		, const QString &elementType
		, const QString &elementName)
{
	Q_UNUSED(metamodelRepoApi);
	Q_UNUSED(modelRepo);

	auto typeNode = qrtext::as<Identifier>(complexIdentifierNode->firstPartOfComplexIdentifier());
	auto currentElementName = typeNode->name();

	auto secondNode = complexIdentifierNode->secondPartOfComplexIdentifier();

	if (elementName == currentElementName) {
		return generatedResultForCorrectNode(typeNode, secondNode, logicalModelInterface, elementId);
	} else {
		QList<qReal::Id> listOfElementIds;
		for (const qReal::Id elementId : logicalModelInterface->children(qReal::Id::rootId())) {
			if (elementId.element() == currentElementName) {
				listOfElementIds << elementId;
			}
		}

		qReal::Id correctId = listOfElementIds.first();
		return generatedResultForCorrectNode(typeNode, secondNode, logicalModelInterface, correctId);
	}
}

QString GeneratorForComplexIdentifierNode::generatedResultForCorrectNode(
		QSharedPointer<simpleParser::ast::Identifier> typeNode
		, QSharedPointer<simpleParser::ast::Node> secondNode
		, qReal::LogicalModelAssistInterface *logicalModelInterface
		, const qReal::Id elementId)
{
	if (secondNode->is<Identifier>()) {
		auto propertyNode =
				qrtext::as<Identifier>(secondNode);
		auto propertyName = propertyNode->name();

		if (elementId != qReal::Id::rootId()) {
			return logicalModelInterface->propertyByRoleName(elementId, propertyName).toString();
		} else {
			// TODO: wtf
			QList<qReal::Id> listOfElementIds;
			for (const qReal::Id elementId : logicalModelInterface->children(qReal::Id::rootId())) {
				if (elementId.element() == typeNode->name()) {
					listOfElementIds << elementId;
				}
			}

			return logicalModelInterface->propertyByRoleName(listOfElementIds.first(), propertyName).toString();
		}
	} else {
		// typeNode.transitionEnd.identifierName

		auto transitionEndNode = qrtext::as<TransitionEnd>(secondNode);
		auto propertyName = qrtext::as<Identifier>(transitionEndNode->firstIdentifier())->name();

		return logicalModelInterface->propertyByRoleName(logicalModelInterface->to(elementId), propertyName).toString();
	}
}

#include "generatorForComplexIdentifierNode.h"

#include "ast/identifier.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

QString GeneratorForComplexIdentifierNode::generatedResult(QSharedPointer<ComplexIdentifier> complexIdentifierNode
		, qrRepo::LogicalRepoApi *metamodelRepoApi
		, qrRepo::RepoControlInterface *modelRepo
		, qReal::LogicalModelAssistInterface *logicalModelInterface
		, const qReal::Id elementId)
{
	Q_UNUSED(metamodelRepoApi);
	Q_UNUSED(modelRepo);

	QSharedPointer<Identifier> typeNode = qrtext::as<Identifier>(complexIdentifierNode->firstPartOfComplexIdentifier());

	QSharedPointer<Identifier> propertyNode =
			qrtext::as<Identifier>(complexIdentifierNode->secondPartOfComplexIdentifier());
	QString propertyName = propertyNode->name();
	qDebug() << propertyName << logicalModelInterface->name(elementId);

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
}

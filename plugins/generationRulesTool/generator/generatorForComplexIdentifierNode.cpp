#include "generatorForComplexIdentifierNode.h"

#include "ast/identifier.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

QString GeneratorForComplexIdentifierNode::generatedResult(QSharedPointer<ComplexIdentifier> complexIdentifierNode
		, qrRepo::LogicalRepoApi *metamodelRepoApi
		, const qReal::Id elementId)
{
	QSharedPointer<Identifier> typeNode = qrtext::as<Identifier>(complexIdentifierNode->firstPartOfComplexIdentifier());

	QSharedPointer<Identifier> propertyNode =
			qrtext::as<Identifier>(complexIdentifierNode->secondPartOfComplexIdentifier());
	QString propertyName = propertyNode->name();

	return metamodelRepoApi->property(elementId, propertyName).toString();
}

#include "auxiliaryMethods.h"

using namespace constraints::generator::generatorUtils;
using namespace qReal;
using namespace qrRepo;

QString AuxiliaryMethods::disjunctionExpression(QList<QString> listOfBooleanExpressions)
{
	return formedExpression(listOfBooleanExpressions, "false", " || ");
}

QString AuxiliaryMethods::conjunctionExpression(QList<QString> listOfBooleanExpressions)
{
	return formedExpression(listOfBooleanExpressions, "true", " && ");
}

IdList AuxiliaryMethods::neighborNodesWithGivenType(const Id &element, const QString &type, const LogicalRepoApi &api)
{
	IdList inNodes;

	for (const Id &inLink : api.incomingLinks(element)) {
		if (inLink.element() == type) {
			inNodes.push_back(api.from(inLink));
		}
	}

	IdList outNodes;
	for (Id const &outLink : api.outgoingLinks(element)) {
		if (outLink.element() == type) {
			outNodes.push_back(api.to(outLink));
		}
	}

	return inNodes << outNodes;
}

bool AuxiliaryMethods::linkWithGivenTypeExists(
		const Id &element
		, const QString &type
		, const LogicalRepoApi &api)
{
	for (const Id &link : api.links(element)) {
		if (link.element() == type) {
			return true;
		}
	}

	return false;
}

QString AuxiliaryMethods::generateExistsProperty(
		const QString &resElementName
		, const QString &elementName
		, const Id &constraint
		, const int depth
		, const QString &additionalString
		, const LogicalRepoApi &api)
{
	QString resultString = "";
	const QString exists = api.property(constraint, "exists").toString();

	resultString += additionalString + "bool " + resElementName + "_" + QString::number(depth) + " = ";

	if (exists == "true" || exists == "false") {
		const QString comparisonSymbol = (exists == "true") ? "!=" : "==";
		resultString += "(" + elementName + "_" + QString::number(depth) + " " + comparisonSymbol + " qReal::Id::rootId());\n";
	} else {
		resultString += "true;\n";
	}

	return resultString;
}

QString AuxiliaryMethods::formedExpression(
		QList<QString> listOfBooleanExpressions
		, const QString &firstElement
		, const QString &connectingSymbol)
{
	if (listOfBooleanExpressions.empty()) {
		listOfBooleanExpressions.push_back(firstElement);
	}

	QString resultExpression = "";

	if (listOfBooleanExpressions.count() == 1) {
		resultExpression += listOfBooleanExpressions.first();
	} else {
		resultExpression += "(" + listOfBooleanExpressions.first();
		listOfBooleanExpressions.pop_front();

		for (const QString &currentExpression : listOfBooleanExpressions) {
			resultExpression += connectingSymbol + currentExpression;
		}

		resultExpression += ")";
	}

	return resultExpression;
}

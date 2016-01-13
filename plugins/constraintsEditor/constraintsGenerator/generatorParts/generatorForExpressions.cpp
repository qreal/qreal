#include "generatorForExpressions.h"

using namespace constraints::generator::generatorParts;

QString GeneratorForExpressions::disjunctionExpression(QList<QString> listOfBooleanExpressions)
{
	return formedExpression(listOfBooleanExpressions, "false", " || ");
}

QString GeneratorForExpressions::conjunctionExpression(QList<QString> listOfBooleanExpressions)
{
	return formedExpression(listOfBooleanExpressions, "true", " && ");
}

QString GeneratorForExpressions::formedExpression(
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

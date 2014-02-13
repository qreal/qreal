#include "methodsCheckerForTravis.h"
#include "convertingMethods.h"

using namespace editorPluginTestingFramework;

int MethodsCheckerForTravis::calculateResult(
		QList<QPair<QString, QPair<QString, QString> > > qrxcAndQrmcResult
		, QList<QPair<QString, QPair<QString, QString> > > qrxcAndInterpreterResult)
{
	bool booleanResult = (calculateResultForOneList(qrxcAndQrmcResult)
			&& calculateResultForOneList(qrxcAndInterpreterResult));
	int result = booleanResult ? 0 : 1;
	return result;
}

bool MethodsCheckerForTravis::calculateResultForOneList(QList<QPair<QString, QPair<QString, QString> > > listOfResults)
{
	typedef QPair<QString, QPair<QString, QString> > StringTriplet;

	bool result = true;
	foreach (StringTriplet const &stringTriplet, listOfResults) {
		QString const &firstResult = stringTriplet.second.first;
		QString const &secondResult = stringTriplet.second.second;
		result = (result && calculateResultForOneMethod(firstResult, secondResult));
	}
	return result;
}

bool MethodsCheckerForTravis::calculateResultForOneMethod(const QString &firstResult, const QString &secondResult)
{
	QSet<QString> firstMethodParsed = ConvertingMethods::resultToCompare(firstResult);
	QSet<QString> secondMethodParsed = ConvertingMethods::resultToCompare(secondResult);

	if (firstMethodParsed == secondMethodParsed) {
		return true;
	}
	return false;
}

#include "methodsCheckerForTravis.h"
#include "convertingMethods.h"

#include <QtCore/QDebug>

using namespace editorPluginTestingFramework;

int MethodsCheckerForTravis::calculateResult(
		QList<QPair<QString, QPair<QString, QString> > > qrxcAndQrmcResult
		, QList<QPair<QString, QPair<QString, QString> > > qrxcAndInterpreterResult)
{
	bool resultForQrxcAndQrmc = calculateResultForOneList(qrxcAndQrmcResult);
	bool resultForQrxcAndInterpreter = calculateResultForOneList(qrxcAndInterpreterResult);

	qDebug() << "Comparison between qrxc and qrmc: " << resultForQrxcAndQrmc
		<< ", Comparison between qrxc and interpreter: " << resultForQrxcAndInterpreter;

	bool booleanResult = (resultForQrxcAndQrmc && resultForQrxcAndInterpreter);
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
		bool currentMethodResult = calculateResultForOneMethod(firstResult, secondResult);
		result = (result && currentMethodResult);
	}
	return result;
}

bool MethodsCheckerForTravis::calculateResultForOneMethod(QString const &firstResult, QString const &secondResult)
{
	QSet<QString> firstMethodParsed = ConvertingMethods::resultToCompare(firstResult);
	QSet<QString> secondMethodParsed = ConvertingMethods::resultToCompare(secondResult);

	if (firstMethodParsed == secondMethodParsed) {
		return true;
	}
	return false;
}

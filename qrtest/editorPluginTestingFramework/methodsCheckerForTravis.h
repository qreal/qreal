#pragma once

#include <QtCore/QString>
#include <QtCore/QPair>

namespace editorPluginTestingFramework {

class MethodsCheckerForTravis
{
public:

	/// returns 0, if all results are correct, and 1, if some of results are incorrect
	static int calculateResult(
			QList<QPair<QString, QPair<QString, QString> > > qrxcAndQrmcResult,
			QList<QPair<QString, QPair<QString, QString> > > qrxcAndInterpreterResult
			);

private:
	/// calculates result for one list
	static bool calculateResultForOneList(QList<QPair<QString, QPair<QString, QString> > > listOfResults);

	/// takes two strings and returns 0, if they are equal, and 1, if they are not equal
	static bool calculateResultForOneMethod(QString const &firstResult, QString const &secondResult);
};

}


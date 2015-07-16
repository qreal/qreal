#pragma once

#include <QtCore/QString>
#include <QtCore/QPair>

#include "methodsTester.h"

namespace editorPluginTestingFramework {

class MethodsCheckerForTravis
{
public:

	/// returns 0, if all results are correct, and 1, if some of results are incorrect
	static int calculateResult(
			QList<MethodsTester::ResultOfGenerating> qrxcAndQrmcResult,
			QList<MethodsTester::ResultOfGenerating> qrxcAndInterpreterResult
			);

private:
	/// calculates result for one list
	static bool calculateResultForOneList(QList<MethodsTester::ResultOfGenerating> listOfResults);

	/// takes two strings and returns 0, if they are equal, and 1, if they are not equal
	static bool calculateResultForOneMethod(const QString &firstResult, const QString &secondResult);
};

}


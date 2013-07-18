#include "methodsTester.h"
#include "defs.h"

using namespace editorPluginTestingFramework;
using namespace qReal;

void MethodsTester::fillMethodsToTestList(QString const &fileName)
{
	QFile methodsToTest(pathToSourceCode + "/" + fileName);
	if (!methodsToTest.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return;
	}

	QTextStream methodsToTestStream(&methodsToTest);

	while (!methodsToTestStream.atEnd()) {
		QString const &method = methodsToTestStream.readLine();
		mMethodsToTest.append(method);
	}
}

QPair<QString, QPair<QString, QString> > MethodsTester::testMethodIfExistsInList(
		UnifiedStringGenerator const &stringGenerator
		, QString const &method)
{
	QPair<QString, QPair<QString, QString> > resultPair;

	if (mMethodsToTest.contains(method)) {
		resultPair = generateOutputForOneMethod(stringGenerator);
	}

	return resultPair;
}

QPair<QString, QPair<QString, QString> > MethodsTester::generateOutputForOneMethod(UnifiedStringGenerator const &stringGenerator)
{
	UnifiedStringGenerator * firstGenerator = initGeneratorWithFirstInterface(stringGenerator);
	QString const &methodName = firstGenerator->methodName();
	QString const &firstResult = firstGenerator->generateString();

	UnifiedStringGenerator * secondGenerator = initGeneratorWithSecondInterface(stringGenerator);
	QString const &secondResult = secondGenerator->generateString();

	QPair<QString, QString> methodsPair = qMakePair(firstResult, secondResult);
	QPair<QString, QPair<QString, QString> > resultPair = qMakePair(methodName, methodsPair);

	return resultPair;
}

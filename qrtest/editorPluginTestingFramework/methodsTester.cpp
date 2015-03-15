#include "methodsTester.h"
#include "defs.h"

using namespace editorPluginTestingFramework;
using namespace qReal;

void MethodsTester::fillMethodsToTestList(QString const &fileName)
{
	QFile methodsToTest(fileName);
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
		AbstractStringGenerator const &stringGenerator
		, QString const &method)
{
	QPair<QString, QPair<QString, QString> > resultPair;

	if (mMethodsToTest.contains(method)) {
		resultPair = generateOutputForOneMethod(stringGenerator);
	}

	return resultPair;
}

QStringList generateOutputTimeForOneMethod(AbstractStringGenerator const &stringGenerator)
{
	AbstractStringGenerator * firstGenerator = initGeneratorWithFirstInterface(stringGenerator);
	QString const &methodName = firstGenerator->methodName();
	QStringList ololo = firstGenerator->ololo();
	QString const &firstResult = firstGenerator->generateStringTime();
	QStringList ololo1 = firstGenerator->ololo();


	AbstractStringGenerator * secondGenerator = initGeneratorWithSecondInterface(stringGenerator);
	QString const &secondResult = secondGenerator->generateStringTime();
	return ololo;
}



QPair<QString, QPair<QString, QString> > MethodsTester::generateOutputForOneMethod(AbstractStringGenerator const &stringGenerator)
{
	AbstractStringGenerator * firstGenerator = initGeneratorWithFirstInterface(stringGenerator);
	QString const &methodName = firstGenerator->methodName();
	QStringList ololo = firstGenerator->ololo();
	QString const &firstResult = firstGenerator->generateString();
	QString qwerty = firstGenerator->generateStringTime();
	QStringList ololo1 = firstGenerator->ololo();


	AbstractStringGenerator * secondGenerator = initGeneratorWithSecondInterface(stringGenerator);
	QString const &secondResult = secondGenerator->generateString();

	QPair<QString, QString> methodsPair = qMakePair(firstResult, secondResult);
	QPair<QString, QPair<QString, QString> > resultPair = qMakePair(methodName, methodsPair);

	return resultPair;
}

#include "methodsTester.h"
#include "defs.h"

using namespace editorPluginTestingFramework;
using namespace qReal;

void MethodsTester::fillMethodsToTestList(const QString &fileName)
{
	QFile methodsToTest(fileName);
	if (!methodsToTest.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return;
	}

	QTextStream methodsToTestStream(&methodsToTest);

	while (!methodsToTestStream.atEnd()) {
		const QString &method = methodsToTestStream.readLine();
		mMethodsToTest.append(method);
	}
}

QPair<QString, QPair<QString, QString> > MethodsTester::testMethodIfExistsInList(
		AbstractStringGenerator const &stringGenerator
		, const QString &method)
{
	QPair<QString, QPair<QString, QString> > resultPair;

	if (mMethodsToTest.contains(method)) {
		resultPair = generateOutputForOneMethod(stringGenerator);
	}

	return resultPair;
}

QList<QPair<QString, QPair<QString, QString> >> MethodsTester::generateTimeResult()
{
	return mTimeResult;
}

QPair<QString, QPair<QString, QString> > MethodsTester::generateOutputForOneMethod(AbstractStringGenerator const &stringGenerator)
{
	AbstractStringGenerator * firstGenerator = initGeneratorWithFirstInterface(stringGenerator);
	const QString &methodName = firstGenerator->methodName();
	const QString &firstResult = firstGenerator->generateString();
	const QString &firstResultTime = firstGenerator->generateStringTime();

	AbstractStringGenerator * secondGenerator = initGeneratorWithSecondInterface(stringGenerator);
	const QString &secondResult = secondGenerator->generateString();
	const QString &secondResultTime = secondGenerator->generateStringTime();
	QPair<QString, QString> methodsPairTime = qMakePair(firstResultTime, secondResultTime);
	QPair<QString, QPair<QString, QString> > resultPairTime = qMakePair(methodName, methodsPairTime);
	mTimeResult.append(resultPairTime);

	QPair<QString, QString> methodsPair = qMakePair(firstResult, secondResult);
	QPair<QString, QPair<QString, QString> > resultPair = qMakePair(methodName, methodsPair);

	return resultPair;
}

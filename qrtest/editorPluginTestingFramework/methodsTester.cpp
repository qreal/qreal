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


QList<QPair<QString, QPair<QString, QString> >> MethodsTester::generateTimeResult()
{
	return mTimeResult;
}



void MethodsTester::generateOutputTimeForOneMethod(AbstractStringGenerator const &stringGenerator)
{
	AbstractStringGenerator * firstGenerator = initGeneratorWithFirstInterface(stringGenerator);
	QString const &methodName = firstGenerator->methodName();
	QString const &firstResult = firstGenerator->generateStringTime();

	AbstractStringGenerator * secondGenerator = initGeneratorWithSecondInterface(stringGenerator);

	//return resultPair;
}



QPair<QString, QPair<QString, QString> > MethodsTester::generateOutputForOneMethod(AbstractStringGenerator const &stringGenerator)
{
	AbstractStringGenerator * firstGenerator = initGeneratorWithFirstInterface(stringGenerator);
	QString const &methodName = firstGenerator->methodName();
	QString const &firstResult = firstGenerator->generateString();
	QString const &firstResultTime = firstGenerator->generateStringTime();


	AbstractStringGenerator * secondGenerator = initGeneratorWithSecondInterface(stringGenerator);
	QString const &secondResult = secondGenerator->generateString();
	QString const &secondResultTime = secondGenerator->generateStringTime();
	QPair<QString, QString> methodsPairTime = qMakePair(firstResultTime, secondResultTime);
	QPair<QString, QPair<QString, QString> > resultPairTime = qMakePair(methodName, methodsPairTime);
	mTimeResult.append(resultPairTime);


	QPair<QString, QString> methodsPair = qMakePair(firstResult, secondResult);

	QPair<QString, QPair<QString, QString> > resultPair = qMakePair(methodName, methodsPair);

	return resultPair;
}

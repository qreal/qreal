#include "methodsTester.h"
#include "defs.h"

using namespace editorPluginTestingFramework;
using namespace qReal;


MethodsTester::ResultOfGenerating::ResultOfGenerating(
		const QString &name, const QString &first, const QString &second)
		: methodName(name), firstResult(first), secondResult(second)
{
}

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

MethodsTester::ResultOfGenerating MethodsTester::testMethodIfExistsInList(
		const AbstractStringGenerator &stringGenerator
		, const QString &method)
{
	MethodsTester::ResultOfGenerating result = MethodsTester::ResultOfGenerating("", "", "");
	if (mMethodsToTest.contains(method)) {
		result = generateOutputForOneMethod(stringGenerator);
	}

	return result;
}

QList<MethodsTester::ResultOfGenerating> MethodsTester::generateTimeResult()
{
	return mTimeResult;
}

MethodsTester::ResultOfGenerating MethodsTester::generateOutputForOneMethod(const AbstractStringGenerator &stringGenerator)
{
	AbstractStringGenerator *firstGenerator = initGeneratorWithFirstInterface(stringGenerator);
	const QString methodName = firstGenerator->methodName();
	const QString firstResult = firstGenerator->generateString();
	const QString firstResultTime = firstGenerator->generateStringTime();

	AbstractStringGenerator *secondGenerator = initGeneratorWithSecondInterface(stringGenerator);
	const QString secondResult = secondGenerator->generateString();
	const QString secondResultTime = secondGenerator->generateStringTime();

	ResultOfGenerating result = ResultOfGenerating(methodName, firstResult, secondResult);
	ResultOfGenerating resultOfTime = ResultOfGenerating(methodName, firstResultTime, secondResultTime);
	mTimeResult.append(resultOfTime);
	return result;
}

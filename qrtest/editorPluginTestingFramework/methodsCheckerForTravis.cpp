/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "methodsCheckerForTravis.h"
#include "convertingMethods.h"

#include <QtCore/QDebug>

using namespace editorPluginTestingFramework;

int MethodsCheckerForTravis::calculateResult(QList<MethodsTester::ResultOfGenerating> qrxcAndQrmcResult
		, QList<MethodsTester::ResultOfGenerating> qrxcAndInterpreterResult)
{
	const bool resultForQrxcAndQrmc = calculateResultForOneList(qrxcAndQrmcResult);
	const bool resultForQrxcAndInterpreter = calculateResultForOneList(qrxcAndInterpreterResult);

	qDebug() << "Comparison between qrxc and qrmc: " << resultForQrxcAndQrmc
		<< ", Comparison between qrxc and interpreter: " << resultForQrxcAndInterpreter;

	const bool booleanResult = (resultForQrxcAndQrmc && resultForQrxcAndInterpreter);
	const int result = booleanResult ? 0 : 1;
	return result;
}

bool MethodsCheckerForTravis::calculateResultForOneList(QList<MethodsTester::ResultOfGenerating> listOfResults)
{
	bool result = true;
	for (const MethodsTester::ResultOfGenerating &element : listOfResults) {
		const QString &firstResult = element.firstResult;
		const QString &secondResult = element.secondResult;
		const bool currentMethodResult = calculateResultForOneMethod(firstResult, secondResult);
		result = (result && currentMethodResult);
	}

	return result;
}

bool MethodsCheckerForTravis::calculateResultForOneMethod(const QString &firstResult, const QString &secondResult)
{
	const QSet<QString> firstMethodParsed = ConvertingMethods::resultToCompare(firstResult);
	const QSet<QString> secondMethodParsed = ConvertingMethods::resultToCompare(secondResult);

	return firstMethodParsed == secondMethodParsed;
}

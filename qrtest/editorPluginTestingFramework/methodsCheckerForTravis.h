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


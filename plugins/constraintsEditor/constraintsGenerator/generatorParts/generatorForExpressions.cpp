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

#include "generatorForExpressions.h"

using namespace constraints::generator::generatorParts;

QString GeneratorForExpressions::disjunctionExpression(QList<QString> listOfBooleanExpressions)
{
	return formedExpression(listOfBooleanExpressions, "false", " || ");
}

QString GeneratorForExpressions::conjunctionExpression(QList<QString> listOfBooleanExpressions)
{
	return formedExpression(listOfBooleanExpressions, "true", " && ");
}

QString GeneratorForExpressions::formedExpression(
		QList<QString> listOfBooleanExpressions
		, const QString &firstElement
		, const QString &connectingSymbol)
{
	if (listOfBooleanExpressions.empty()) {
		listOfBooleanExpressions.push_back(firstElement);
	}

	QString resultExpression = "";

	if (listOfBooleanExpressions.count() == 1) {
		resultExpression += listOfBooleanExpressions.first();
	} else {
		resultExpression += "(" + listOfBooleanExpressions.first();
		listOfBooleanExpressions.pop_front();

		for (const QString &currentExpression : listOfBooleanExpressions) {
			resultExpression += connectingSymbol + currentExpression;
		}

		resultExpression += ")";
	}

	return resultExpression;
}

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

#include "convertingMethods.h"

using namespace editorPluginTestingFramework;
using namespace qReal;

QStringList ConvertingMethods::convertQListExplosionDataIntoStringList(const QList<Metamodel::ExplosionData> &explDataList)
{
	QStringList resultList;
	for (const Metamodel::ExplosionData &element : explDataList) {
		resultList.append(element.targetElement);
	}

	return resultList;
}

QStringList ConvertingMethods::convertingQPairListIntoStringList(const QList<QPair<QString, QString>> &qPairList)
{
	QStringList resultList;
	for (const auto &element : qPairList ) {
		resultList.append(element.first);
		resultList.append(element.second);
	}

	return resultList;
}

QStringList ConvertingMethods::convertIdListIntoStringList(const IdList &idList)
{
	QStringList resultList;

	for (const Id &id : idList) {
		resultList += id.toString();
	}

	return resultList;
}

QStringList ConvertingMethods::convertStringIntoStringList(const QString &string)
{
	QStringList result;
	result.append(string);
	return result;
}

QStringList ConvertingMethods::convertBoolIntoStringList(const bool &boolValue)
{
	QString stringRepresentation = (boolValue) ? "true" : "false";
	return convertStringIntoStringList(stringRepresentation);
}

QStringList ConvertingMethods::convertIdIntoStringList(const Id &id)
{
	QString stringRepresentation = id.toString();
	return convertStringIntoStringList(stringRepresentation);
}

QStringList ConvertingMethods::convertIntIntoStringList(const int &integer)
{
	QString stringRepresentation = (integer == 1) ? "1" : "0";
	return convertStringIntoStringList(stringRepresentation);
}

QStringList ConvertingMethods::convertExplosionListIntoStringList(const QList<Explosion> &explosionList)
{
	QStringList result;
	for (const Explosion &explosion : explosionList) {
		const QString &target = explosion.target().toString();
		result.append(target);
	}
	return result;
}

QString ConvertingMethods::transformateOutput(
		const QStringList &output
		, const Id &id
		, const QString &name
		)
{
	QString result;
	if (name.isEmpty()) {
		result.append(id.toString() + "-");
	} else {
		result.append(name + "-");
	}

	for (const auto &outputElement : output) {
		result.append(outputElement + ",");
	}

	return result;
}

QSet<QString> ConvertingMethods::resultToCompare(const QString &method)
{
	const QStringList methodOutput = method.split("|");

	QStringList result;
	for (const QString &string : methodOutput) {
		const QString output = string.split("-").last();
		const QStringList outputToList = output.split(",");

		result.append(outputToList);
	}

	const QSet<QString> methodParsed = result.toSet();
	return methodParsed;
}

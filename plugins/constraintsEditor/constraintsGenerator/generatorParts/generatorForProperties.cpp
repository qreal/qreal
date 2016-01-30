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

#include "generatorForProperties.h"

using namespace constraints::generator::generatorParts;
using namespace qReal;
using namespace qrRepo;

QPair<QString, QString> GeneratorForProperties::countPropertyCharacteristicForConstraintElement(
		const Id &constraint
		, const QString &characteristicName
		, const QString &defaultValue
		, const QString &property
		, QString sign
		, QString value
		, const QString &elementName
		, const int depth
		, const QString &additionalString
		, ErrorReporterInterface &errorReporter)
{
	QString resultString;

	if (sign == "=") {
		sign += "=";
	}

	sign.replace("&lt;", "<");

	resultString += QString("%1bool %2Res_%3 = %4;\n")
			.arg(additionalString)
			.arg(characteristicName)
			.arg(depth)
			.arg(defaultValue);

	if (property.compare("TYPE", Qt::CaseSensitive) == 0) {
		resultString += QString("%1if (%2 != qReal::Id::rootId()) {\n"
				"%1	%3Res_%4 = (%2.element() %5 \"%6\");\n"
				"%1}\n")
				.arg(additionalString)
				.arg(elementName)
				.arg(characteristicName)
				.arg(depth)
				.arg(sign)
				.arg(value);
	} else if (property.compare("METATYPE", Qt::CaseInsensitive) == 0) {
		resultString += QString("%1if (%2 != qReal::Id::rootId()) {\n"
				"%1	qReal::EditorManagerInterface::MetaType isNodeOrEdge = mEditorManager->metaTypeOfElement(%2);\n")
				.arg(additionalString)
				.arg(elementName);

		if (value.compare("node", Qt::CaseInsensitive) == 0) {
			value = "qReal::EditorManagerInterface::node";
		} else if (value.compare("edge", Qt::CaseInsensitive) == 0) {
			value = "qReal::EditorManagerInterface::edge";
		} else {
			errorReporter.addCritical(QObject::tr("Metatype \"%1\" does not exist. Select \"node\" or \"edge\".")
					.arg(value), constraint);
		}

		resultString += QString("%1	%2Res_%3 = (isNodeOrEdge %4 %5);\n"
				"%1}\n")
				.arg(additionalString)
				.arg(characteristicName)
				.arg(depth)
				.arg(sign)
				.arg(value);
	} else {
		bool valueIsInt = false;
		int intValue = value.toInt(&valueIsInt);
		Q_UNUSED(intValue);

		resultString += QString("%1if (logicalApi.hasProperty(%2, \"%3\")) {\n"
				"%1	%4Res_%5 = (logicalApi.property(%2, \"%3\").")
				.arg(additionalString)
				.arg(elementName)
				.arg(property)
				.arg(characteristicName)
				.arg(depth);

		if (!valueIsInt) {
			resultString += QString("toString() %1  \"%2\");\n")
					.arg(sign)
					.arg(value);
		} else {
			resultString += QString("toInt() %1 %2);\n")
					.arg(sign)
					.arg(value);
		}

		resultString += additionalString + "}\n";
	}

	const QString listOfBooleanExpressions = QString("%1Res_%2")
			.arg(characteristicName)
			.arg(depth);

	return {resultString, listOfBooleanExpressions};
}

QString GeneratorForProperties::generateExistsProperty(
		const QString &resElementName
		, const QString &elementName
		, const Id &constraint
		, const int depth
		, const QString &additionalString
		, const LogicalRepoApi &api)
{
	QString resultString = "";

	const QString exists = api.property(constraint, "exists").toString();

	resultString += QString("%1bool %2_%3 = ")
			.arg(additionalString)
			.arg(resElementName)
			.arg(depth);

	if (exists == "true" || exists == "false") {
		const QString comparisonSymbol = (exists == "true") ? "!=" : "==";

		resultString += QString("(%1_%2 %3 qReal::Id::rootId());\n")
				.arg(elementName)
				.arg(depth)
				.arg(comparisonSymbol);
	} else {
		resultString += "true;\n";
	}

	return resultString;
}

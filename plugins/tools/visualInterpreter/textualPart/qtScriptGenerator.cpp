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

#include "qtScriptGenerator.h"

using namespace qReal;

QtScriptGenerator::QtScriptGenerator(LogicalModelAssistInterface &logicalModelApi
		, GraphicalModelAssistInterface &graphicalModelApi
		, gui::MainWindowInterpretersInterface &interpretersInterface)
		: TextCodeGenerator(logicalModelApi, graphicalModelApi, interpretersInterface)
{
}

QString QtScriptGenerator::createProperInitAndOutput(QString const &code, bool const isApplicationCondition) const
{
	QString init = "";
	QString output = "''";
	foreach (QString const &elemName, mPropertiesUsage.keys()) {
		foreach (QString const &propertyName, *mPropertiesUsage.value(elemName)) {
			QString const variable = elemName + delimeter + propertyName;
			QString const curPropertyValue = property(mMatch.value(idByName(elemName)), propertyName);

			bool isStringProp = isStringProperty(mMatch.value(idByName(elemName)), propertyName);
			QString const propertyValue = isStringProp ? "'" + escape(curPropertyValue) + "'" : curPropertyValue;
			QString const representationOfProperty = "'\\'' + String(" + variable + ") + '\\''";

			init += variable + "=" + propertyValue + "; ";
			output += " + '" + variable + "=' + " + representationOfProperty + " + ';'";
		}
	}
	if (!isApplicationCondition) {
		return init + "\n\n" + code + "\n\n" + output;
	} else {
		return init + "\n\n" + code;
	}
}

QString QtScriptGenerator::createBehaviourFunction(QString const &elementName, QString const &propertyName) const
{
	QString result = properElementProperty(elementName, propertyName);
	result = "function " + elementName + delimeter + propertyName + "() {\n" + result + "}\n\n";

	return result;
}

QString QtScriptGenerator::properElementProperty(QString const &elementName, QString const &propertyName) const
{
	QString propertyValue = property(mMatch.value(idByName(elementName)), propertyName);

	propertyValue.replace("this.", elementName + ".");
	propertyValue.replace("this@", elementName + "@");
	propertyValue.replace("\n", "\n  ");

	return propertyValue;
}
